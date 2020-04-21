/**
 * @file Order.cpp
 * @brief Stores all data regarding a request made by management.
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Order.hpp"
using namespace Order;

BaseOrder::BaseOrder(uint8_t order_id, uint8_t order_type) : order_id(order_id), order_type(order_type) {
    log(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
}

BaseOrder::~BaseOrder() {
}
