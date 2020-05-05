#include "OrderQueue.hpp"

OrderQueue::OrderQueue(){

}

OrderQueue::~OrderQueue(){
    
}

/*
        /// STUB (i.e. so serve para ser utilizável no main) ///
	Adiciona order_to_add com base na sua prioridade perante as outras Orders.
	Orders de Carregar e Descarregar vao sempre para o topo.
	Retorna true se tiver sucesso (em principio nao vai ser usado).
*/
bool OrderQueue::AddOrder(Order::BaseOrder order_to_add)
{
	if (!order_to_add.is_valid())
		return false;

	orders_.push_back(order_to_add);
	return true;
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
    e com as peças disponíveis em Armazém.
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
