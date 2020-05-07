#include "OrderQueue.hpp"

OrderQueue::OrderQueue(){
	warehouse = new Warehouse(NULL);
}

OrderQueue::~OrderQueue(){
    
}

/*
        /// STUB (i.e. so serve para ser utilizável no main) ///
	Adiciona order_to_add com base na sua prioridade perante as outras Orders.
	Orders de Carregar e Descarregar vao sempre para o topo.
	Retorna pk da order se tiver sucesso. -1 se falhar. Se for uma order de load, retorna id (pk) da peca.
*/
int OrderQueue::AddOrder(Order::BaseOrder order_to_add)
{
	if (!order_to_add.is_valid()){
		return -1; // Request stores não precisam de ser guardadas na DB, nem na order queue
	}

	std::string type_string;
	std::string state_string;
	std::string initPiece_string = std::to_string(order_to_add.GetInitialPiece());
	std::string finalPiece_string = std::to_string(order_to_add.GetFinalPiece());
	std::string deadline_string = order_to_add.GetDeadline();
	int total_pieces = order_to_add.GetCount();
	uint8_t order_type = order_to_add.GetType();
	time_t enddeadline = GetDataTime(order_to_add.GetDeadline());

	int return_value;
	bool load_order = false;

	// Adicionar na list do MES
	
	std::list<Order::BaseOrder>::iterator destination;
	time_t tempdeadline;
	//significa que tem prioridade máxima e tem de ser colocada antes das primeira vez que aparece Transformation
	if((order_type == Order::ORDER_TYPE_LOAD) || (order_type == Order::ORDER_TYPE_UNLOAD)) {
		for (destination = orders_.begin(); destination != orders_.end(); ++destination)
		{
			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATON)
			{
				orders_.insert(destination, order_to_add)
				break;
			}
		}
	}
	// ser do tipo transformation entao vou ter de comparar com os diferentes deadline das orders do tipo transformation
	else{
		for (destination = orders_.begin(); destination != orders_.end(); ++destination) {

			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATON){

				tempdeadline = OrderQueue::GetDataTime((*destination).GetDeadline());
				if (difftime(enddeadline, tempdeadline) < 0){

					orders_.insert(destination, order_to_add);
					break;
				}
			}
		}
	}


	// Adicionar na base de dados

	switch (order_type)
	{
	case Order::ORDER_TYPE_TRANSFORMATON:
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
		type_string = "Waiting";
		break;
	}

	// adiciona order à base de dados
	return_value = insertDataOrder("factory.db", 
						(int) order_to_add.GetID(), 
						type_string, 
						state_string, 
						initPiece_string, 
						finalPiece_string, 
						total_pieces, 
						DateTime("factory.db", deadline_string);

	// se for uma order de carga, adiciona piece também
	if (load_order){
		return_value = insertDataPiece("factory.db", return_value);
	}

	return return_value;
}

/*
	Remove order_to_remove diretamente, sem nenhum criterio.
	Retorna true se tiver sucesso (em principio nao vai ser usado).
*/
bool OrderQueue::RemoveOrder(Order::BaseOrder order_to_remove)
{
	std::list<Order::BaseOrder>::iterator orders_iter_ = orders_.begin();
	while ((*orders_iter_).GetID() != order_to_remove.GetID()){
		if (orders_iter_ == orders_.end()){
			return false;
		}
		orders_iter_++;
	}
	orders_.erase(orders_iter_);
	return true;
}

/*
        /// STUB (i.e. so serve para ser utilizável no main) ///
	Devolve a próxima Order a executar com base na prioridade (Orders do topo primeiro)
    e com as peças disponíveis em Armazém. No caso de ser descarga, se os pushers estiverem
	cheios nao podemos enviar
*/
Order::BaseOrder OrderQueue::GetNextOrder()
{
	return orders_.front();
}


time_t OrderQueue::GetDataTime(std::string datatime)
{
	time_t rawtime1;
	struct tm  timeinfo1;
	size_t pos = 0;
	int posicao = 0;
	std::string token;
	std::string delimiter;

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
bool OrderQueue::update()
{
	std::string deadline_string;

	// visto que a nova ordem aponta sempre para o fim
	std::list<Order::BaseOrder>::iterator source = orders_.end();
	std::list<Order::BaseOrder>::iterator destination;
	time_t enddeadline = OrderQueue::GetDataTime((*source).GetDeadline());

	time_t tempdeadline;
	//significa que tem prioridade máxima e tem de ser colocada antes das primeira vez que aparece Transformation
	if(((*source).GetType() == Order::ORDER_TYPE_LOAD) OR ((*source).GetType() == Order::ORDER_TYPE_UNLOAD)) {
		for (destination = orders_.begin(); destination != orders_.end(); ++destination)
		{
			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATON)
			{
				orders_.insert(destination, order_to_add)
				break;
			}
		}
	}
	// ser do tipo transformation entao vou ter de comparar com os diferentes deadline das orders do tipo transformation
	else{
		for (destination = orders_.begin(); destination != orders_.end(); ++destination) {

			if ((*destination).GetType() == Order::ORDER_TYPE_TRANSFORMATON){

				time_t tempdeadline = OrderQueue::GetDataTime((*destination).GetDeadline());
				if (difftime(source, destination) < 0){

					orders_.insert(destination, order_to_add);
					break;
				}
			}
		}
	}
	
	return true;
}
/*time_t rawtime, rawtime1;
	struct tm timeinfo, timeinfo1;

	timeinfo.tm_year = 2016;
	timeinfo.tm_mon = 03;
	timeinfo.tm_mday = 02;
	timeinfo.tm_hour = 10;
	timeinfo.tm_min = 17;
	timeinfo.tm_sec = 10;
	rawtime = mktime(&timeinfo);


	timeinfo1.tm_year = 2016;
	timeinfo1.tm_mon = 03;
	timeinfo1.tm_mday = 02;
	timeinfo1.tm_hour = 10;
	timeinfo1.tm_min = 18;
	timeinfo1.tm_sec = 15;
	rawtime1 = mktime(&timeinfo1);

	std::cout << rawtime1 << std::endl;
	std::cout << "Diff: " << difftime(rawtime1, rawtime)<<std::endl;
	*/