/**
 * @file Order.hpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _ORDER_HPP_
#define _ORDER_HPP_

#include <stdlib.h>
#include <iostream>
#include <chrono>

#include "helpers.h"

namespace Order {
    class BaseOrder;
    const uint8_t ORDER_TYPE_TRANSFORMATON = 0;
    const uint8_t ORDER_TYPE_UNLOAD = 1;
    const uint8_t ORDER_TYPE_REQUESTSTORES = 2;
};

class Order::BaseOrder {
private:
    uint8_t order_id = 0;
    uint8_t order_type = 0;
    uint32_t quantity = 0;
    uint64_t creation_epoch = 0;
    uint64_t deadline = 0;
public:
    BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t quantity = 0);
    ~BaseOrder();

    bool is_valid();
    uint8_t GetID();
    uint8_t GetType();
    bool operator == (Order::BaseOrder &ordr) const {return order_id == ordr.GetID();}
    bool operator != (Order::BaseOrder &ordr) const {return !operator == (ordr);}
};

#endif
