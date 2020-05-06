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
#include <list>
#include <iterator>
#include <string>

#include "helpers.h"

namespace Order {
    class BaseOrder;
    class Piece;
    const uint8_t ORDER_TYPE_TRANSFORMATON = 0;
    const uint8_t ORDER_TYPE_UNLOAD = 1;
    const uint8_t ORDER_TYPE_REQUESTSTORES = 2;
    const uint8_t ORDER_TYPE_LOAD = 3;
};

class Order::BaseOrder {
private:
    uint32_t order_id = 0;
    uint32_t order_pk = 0;
    uint8_t order_type = 0;
    uint32_t count = 0;
    time_t CreationTime;
    time_t Deadline;
    uint8_t initialPiece;
    uint8_t finalPiece;
    std::list<Piece> pieces;

public:
<<<<<<< HEAD
    BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t count, uint8_t initialPiece, uint8_t finalPiece, time_t deadline);
=======
    BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t quantity = 0, uint8_t initialPiece = 0, uint8_t finalPiece = 0);
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf
    ~BaseOrder();

    bool is_valid();
    uint32_t GetID();
    uint32_t GetPK();
    uint8_t GetType();
    uint32_t GetCount();
    time_t GetCreationTime();
    time_t GetDeadline();
    uint8_t GetInitialPiece();
    uint8_t GetFinalPiece();
    std::list<Piece> GetPieces();


    bool operator == (Order::BaseOrder &ordr) const {return order_id == ordr.GetID();}
    bool operator != (Order::BaseOrder &ordr) const {return !operator == (ordr);}
};

class Order::Piece {
private:
    uint32_t PieceID;
    uint8_t Path[59] = {0};
<<<<<<< HEAD
    bool piece_on_hold;
=======
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf

public:
    Piece(uint32_t id);
    uint32_t GetID();
    uint8_t *GetPath();
<<<<<<< HEAD
    bool isOnHold();
    void setPieceAsNotOnHold();
=======
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf

};

#endif
