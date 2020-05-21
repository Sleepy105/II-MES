#ifndef _ORDERQUEUE_HPP_
#define _ORDERQUEUE_HPP_

#include <chrono>
#include <algorithm>
#include <list>
#include <iterator>
#include <string>
#include <mutex>
#include "Order.hpp"
#include "Warehouse.hpp"
#include "DBInterface.hpp"
#include "PathFinder.hpp"

class OrderQueue{
    private:
        std::list<Order::BaseOrder> orders_;
        Warehouse *warehouse;
        PathFinder::PathFinder* pathfinder;
        std::mutex mtx;
        

    public:
        OrderQueue(Warehouse* warehouse);
        ~OrderQueue();
        int AddOrder(Order::BaseOrder order_to_add);
        bool RemoveOrder(Order::BaseOrder order_to_remove);
        bool RemovePiece(uint32_t target_id);
        bool RestoreLoadUnload(InformationDisInc LoadUndload);
        bool RestoreTrans(Transformation temp);
        Order::BaseOrder *GetNextOrder();
        time_t GetDataTime(std::string datatime);
        
        void print();
};

#endif
