#include "OrderQueue.hpp"

OrderQueue::OrderQueue(Warehouse* warehouse) : warehouse(warehouse){

}

OrderQueue::~OrderQueue(){
    
}

/*
	Adiciona order_to_add com base na sua prioridade perante as outras Orders.
	Orders de Carregar e Descarregar vao sempre para o topo.
	Retorna pk da order se tiver sucesso. -1 se falhar. Se for uma order de load, retorna id (pk) da peca.
*/
int OrderQueue::AddOrder(Order::BaseOrder order_to_add)
{
	mtx.lock();

	if (!order_to_add.is_valid()){
		meslog(ERROR) << "Order is invalid (doesn't belong in queue)" << std::endl;
		mtx.unlock();
		return -1; // Request stores não precisam de ser guardadas na DB, nem na order queue
	}

	std::string type_string;
	std::string state_string;
	std::string initPiece_string = "P"+std::to_string(order_to_add.GetInitialPiece());
	std::string finalPiece_string = "P"+std::to_string(order_to_add.GetFinalPiece());
	std::string deadline_string = order_to_add.GetDeadline();
	int total_pieces = order_to_add.GetCount();
	uint8_t order_type = order_to_add.GetType();
	time_t enddeadline;

	int return_value;
	bool load_order = false;

	// Adicionar na base de dados

	switch (order_type)
	{
	case Order::ORDER_TYPE_TRANSFORMATION:
		type_string = "Transformation";
		state_string = "Waiting";
		break;
	case Order::ORDER_TYPE_LOAD:
		type_string = "Incoming";
		state_string = "Executing"; //As operações de carga nunca estao "Waiting", começam logo a executar
		load_order = true;
		break;
	case Order::ORDER_TYPE_UNLOAD:
		type_string = "Dispatch";
		state_string = "Waiting";
		break;
	}
	// adiciona order à base de dados
	
	return_value = insertDataOrder(DBFILE, 
						(int) order_to_add.GetID(), 
						type_string, 
						state_string, 
						initPiece_string, 
						finalPiece_string, 
						total_pieces, 
						deadline_string);

	order_to_add.SetPK(return_value);

	// se for uma order de carga, adiciona piece também
	if (load_order){
		return_value = insertDataPiece(DBFILE, return_value);
		order_to_add.AddPiece((uint32_t)return_value);
	}


	// Adicionar na list do MES
	
	std::list<Order::BaseOrder>::iterator destination;
	time_t tempdeadline;
	//significa que tem prioridade máxima e tem de ser colocada antes das primeira vez que aparece Transformation
	if((order_type == Order::ORDER_TYPE_LOAD) || (order_type == Order::ORDER_TYPE_UNLOAD)) {
		for (destination = orders_.begin(); destination != orders_.end(); ++destination)
		{
			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATION)
			{
				break;
			}
		}
	}
	

	// ser do tipo transformation entao vou ter de comparar com os diferentes deadline das orders do tipo transformation
	else{
		enddeadline = GetDataTime(order_to_add.GetDeadline());
		for (destination = orders_.begin(); destination != orders_.end(); ++destination) {

			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATION){

				tempdeadline = OrderQueue::GetDataTime((*destination).GetDeadline());
				if (difftime(enddeadline, tempdeadline) < 0){
					break;
				}
			}
		}
	}
	// colocar o insert apos o ciclo for, em vez de dentro, garante que, mesmo que nao hajam orders de transformacao ou 
	// orders de todo, a nova order e adicionada na lista. Os ciclos apenas determinam o destination aonde inserir a order
	orders_.insert(destination, order_to_add);
	meslog(INFO) << "Order " << order_to_add.GetID() << " added!" << std::endl;
	
	mtx.unlock();
	return return_value;
}
bool OrderQueue::RestoreLoadUnload(InformationDisInc LoadUndload)
{
	uint8_t type_int = 0;

	if(LoadUndload.Type == "Incoming") {
		type_int = Order::ORDER_TYPE_LOAD;
	}
		
	else {
		type_int = Order::ORDER_TYPE_UNLOAD;
	}
		
	Order::BaseOrder aux = Order::BaseOrder(LoadUndload.order_pk, type_int, LoadUndload.count, LoadUndload.initialPiece, LoadUndload.finalPiece);
	orders_.push_back(aux);
	for(int i = 0; i < LoadUndload.vectorPiecePosition; i++) {
		aux.AddPiece(Order::Piece(LoadUndload.pieces[i].id_piece));
	}
	return false;
}

bool OrderQueue::RestoreTrans(Transformation temp)
{
	Order::BaseOrder aux = Order::BaseOrder(temp.order_pk, Order::ORDER_TYPE_TRANSFORMATION, temp.count, temp.initialPiece, temp.finalPiece, temp.Deadline);
	orders_.push_back(aux);
	for(int i = 0; i < temp.vectorPiecePosition; i++) {
		aux.AddPiece(Order::Piece(temp.pieces[i].id_piece));
	}
	return false;
}

/*
	Remove order_to_remove diretamente, sem nenhum criterio.
	Retorna true se tiver sucesso (em principio nao vai ser usado).
*/
bool OrderQueue::RemoveOrder(Order::BaseOrder order_to_remove)
{
	mtx.lock();
	std::list<Order::BaseOrder>::iterator orders_iter_ = orders_.begin();

	for (orders_iter_ = orders_.begin(); orders_iter_ != orders_.end(); orders_iter_++){
		if ((*orders_iter_).GetID() == order_to_remove.GetID()){
			updateOrder(DBFILE, "Finished", (int) order_to_remove.GetID());
			// update order in database before deleting localy.
			orders_.erase(orders_iter_);
		}
	}
	mtx.unlock();
	return false; // end of function reached only if order was not found
}


/*
	Remove primeira peca que de match com o target_id, independentemente da order (nao verifica se ha pecas duplicadas).
	Se esta peca for a ultima peca da order, remove a order também.
	Se conseguir encontrar e remover a peca retorna tipo de peça removida, senao retorna 0.
*/
uint8_t OrderQueue::RemovePiece(uint32_t target_id){
	meslog(INFO) << "Erasing piece " << target_id << std::endl;

	uint8_t removed_piece_type;

	std::list<Order::BaseOrder>::iterator orders_iter_;
	std::list<Order::Piece>::iterator pieces_iter_;
	std::list<Order::Piece> *piece_list;

	for (orders_iter_ = orders_.begin(); orders_iter_ != orders_.end(); orders_iter_++){
	// for each order
		piece_list = orders_iter_->GetPieces(); // just to avoid writting orders_iter_->GetPieces() over and over
		for (pieces_iter_ = piece_list->begin(); pieces_iter_ != piece_list->end(); pieces_iter_++){
		// for each piece
			if (pieces_iter_->GetID() == target_id){
				updateDataPiece(DBFILE, (int) target_id); // update piece finish time in database
				removed_piece_type = orders_iter_->GetFinalPiece();
				piece_list->erase(pieces_iter_);
				// piece has been deleted. If there are no more pieces on hold and no pieces in factory floor, remove order
				if ((orders_iter_->GetCount() == 0) && (piece_list->size() == 0)){
					meslog (ERROR) << "REMOVE ORDER!!!!!!" << std::endl;
					RemoveOrder((*orders_iter_));
				}
				meslog(INFO) << "Piece " << target_id << " erased!" << std::endl;
				print();
				return removed_piece_type; // a piece was found and deleted, return true
			}
		}
	}
	meslog(ERROR) << "Couldn't find Piece " << target_id << " in Order Queue!" << std::endl;
	return 0; // end of function reached only if piece was not found
}

/*
        /// STUB (i.e. so serve para ser utilizável no main) ///
	Devolve a próxima Order a executar com base na prioridade (Orders do topo primeiro)
    e com as peças disponíveis em Armazém. No caso de ser descarga, se os pushers estiverem
	cheios nao podemos enviar. Devolve NULL se nao houver orders para enviar (nao ha orders,
	estao todas a executar, ou nenhuma esta em condicoes de comecar a executar)
*/
Order::BaseOrder *OrderQueue::GetNextOrder(){
	mtx.lock();

	for (std::list<Order::BaseOrder>::iterator orders_iter_ = orders_.begin(); orders_iter_ != orders_.end(); orders_iter_++){
		Order::BaseOrder& order = *orders_iter_;
		
		// if it's an unload/transformation order, count is bigger than 0 and there are still pieces of desired type in warehouse
		if ((order.GetType() != Order::ORDER_TYPE_UNLOAD) && (order.GetType() != Order::ORDER_TYPE_TRANSFORMATION)) {
			continue;
		}
		
		if (order.GetCount() <= 0) {
			// Order Invalid: No parts left to insert
			continue;
		}

		if (warehouse->GetPieceCount(order.GetInitialPiece()) <= 0) {
			// Order Invalid: No parts available in the warehouse to insert
			continue;
		}

		/*** Valid Order Found ***/

		// Find path for new part
		Path* path = pathfinder.FindPath(order);
		if (!path) {
			// Order Invalid: At the moment, a valid path has not been found
			continue;
		}

		// Add Part to the Database and receive its ID
		int part_id = insertDataPiece(DBFILE,order.GetPK());
		if (part_id < 0) {
			// Error obtaining a ID for the part
			//continue;
			return NULL;
		}
		
		Order::Piece part(part_id);
		part.SetPath(path);

		order.AddPiece(part);
		
		mtx.unlock(); // Unlock order list mutex
		return &order;
	}

	mtx.unlock(); // Unlock order list mutex
	throw "No orders found!";
}


time_t OrderQueue::GetDataTime(std::string datatime)
{
	time_t rawtime1;
	struct tm  timeinfo1;
	size_t pos = 0;
	int posicao = 0;
	std::string token;
	std::string delimiter;

	delimiter = "-";

	while ((pos = datatime.find(delimiter)) != std::string::npos) {
		token = datatime.substr(0, pos);
		if (posicao == 0)
			timeinfo1.tm_year = stoi(token);
		else
			timeinfo1.tm_mon = stoi(token);

		posicao++;
		datatime.erase(0, pos + delimiter.length());
	}

	delimiter = " ";
	while ((pos = datatime.find(delimiter)) != std::string::npos) {
		token = datatime.substr(0, pos);
		timeinfo1.tm_mday = stoi(token);
		posicao++;
		datatime.erase(0, pos + delimiter.length());
	}

	delimiter = ":";
	while ((pos = datatime.find(delimiter)) != std::string::npos) {
		token = datatime.substr(0, pos);
		if (posicao == 3)
			timeinfo1.tm_hour = stoi(token);
		else
			timeinfo1.tm_min = stoi(token);
		posicao++;
		datatime.erase(0, pos + delimiter.length());
	}
	timeinfo1.tm_sec = stoi(datatime);
	rawtime1 = mktime(&timeinfo1);
	return rawtime1;
}


void OrderQueue::print(){
	if (orders_.size() == 0){
		meslog(INFO) << "Order Queue has no orders." << std::endl;
		return;
	}
	meslog(INFO) << "Order Queue has " << orders_.size() << " orders:" << std::endl;
	std::list<Order::BaseOrder>::iterator iter;
	for (iter = orders_.begin(); iter != orders_.end(); iter++){
		iter->print();
	}
}