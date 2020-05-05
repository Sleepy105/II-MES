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

BaseOrder::BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t quantity, uint8_t from_part_type, uint8_t to_part_type) : order_id(order_id), order_type(order_type), quantity(quantity), from_part_type(from_part_type), to_part_type(to_part_type) {
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;

    // TODO Verify Transformation
    // TODO Deal with max_delay
}

BaseOrder::~BaseOrder() {
}

bool BaseOrder::is_valid() {
    // TODO check validity
    return true;
}

uint8_t BaseOrder::GetID(){
    return order_id;
}

uint8_t BaseOrder::GetType(){
    return order_type;
}