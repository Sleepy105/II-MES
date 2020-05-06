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
	Retorna pk da order se tiver sucesso. -1 se falhar. Se for uma order de load, retorna id da peca.
*/
int OrderQueue::AddOrder(Order::BaseOrder order_to_add)
{
	if (!order_to_add.is_valid()){
		return -1; // Request stores não precisam de ser guardadas na DB, nem na order queue
	}

	orders_.push_back(order_to_add); // adiciona order à queue

	std::string type_string;
	std::string state_string;
	std::string initPiece_string = std::to_string(order_to_add.GetInitialPiece());
	std::string finalPiece_string = std::to_string(order_to_add.GetFinalPiece());
	std::string deadline_string = std::to_string(order_to_add.GetDeadline());
	int total_pieces = order_to_add.GetCount();

	int return_value;
	bool load_order = false;

	switch (order_to_add.GetType())
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
						deadline_string);

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

/*
        /// POR IMPLEMENTAR ///
    Penso que isto supostamente atualiza a ordem das Orders?
    Retorna true se tiver sucesso(em principio nao vai ser usado)
*/
bool OrderQueue::update()
{
	return false;
}
