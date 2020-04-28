#include <list>
#include <iterator>
#include "Order.hpp"
#include "Warehouse.hpp"

class OrderQueue{
    private:
        std::list<Order::BaseOrder> orders_;

    public:
        OrderQueue(); //vamos usar argumentos para o construtor?
        ~OrderQueue();
        bool AddOrder(Order::BaseOrder order_to_add);
        bool RemoveOrder(Order::BaseOrder order_to_remove);
        Order::BaseOrder GetNextOrder();
        bool update();
};