/**
 * @file Order.cpp
 * @brief Stores all data regarding a request made by management.
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdlib.h>
#include <iostream>
#include <chrono>

namespace Order {
    class BaseOrder;
    uint8_t ORDER_TYPE_TRANSFORMATON = 0;
    uint8_t ORDER_TYPE_UNLOAD = 1;
    uint8_t ORDER_TYPE_REQUESTSTORES = 2;
};

using namespace Order;
class Order::BaseOrder {
private:
    uint8_t order_id = 0;
    uint8_t order_type = 0;
    uint32_t quantity = 0;
    uint64_t creation_epoch = 0;
    uint64_t deadline = 0;
public:
    BaseOrder(uint8_t order_id, uint8_t order_type);
    ~BaseOrder();

    bool is_valid();
};

BaseOrder::BaseOrder(uint8_t order_id, uint8_t order_type) : order_id(order_id), order_type(order_type) {
    std::cout << "ORDER " << std::to_string(order_id) << " created." << std::endl;
}

BaseOrder::~BaseOrder() {
}
