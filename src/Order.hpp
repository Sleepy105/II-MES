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
#include "DBInterface.hpp"
#include "helpers.h"

namespace Order {
    class BaseOrder;
    class Piece;
    const uint8_t ORDER_TYPE_TRANSFORMATION = 0;
    const uint8_t ORDER_TYPE_UNLOAD = 1;
    const uint8_t ORDER_TYPE_REQUESTSTORES = 2;
    const uint8_t ORDER_TYPE_LOAD = 3;
};





class Order::BaseOrder {
private:
    uint32_t order_id = 0;
    int order_pk = 0;
    uint8_t order_type = 0;
    uint32_t count = 0;
    std::string CreationTime;
    std::string Deadline;
    uint8_t initialPiece;
    uint8_t finalPiece;
    std::list<Piece> pieces;

public:
    BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t count, uint8_t initialPiece, uint8_t finalPiece, std::string deadline);
    BaseOrder(uint8_t order_id, uint8_t order_type, uint32_t count, uint8_t initialPiece, uint8_t finalPiece, int deadline = 0);
    BaseOrder(uint8_t order_id, uint8_t order_type);
    ~BaseOrder();

    bool is_valid();
    uint32_t GetID();
    int GetPK();
    uint8_t GetType();
    uint32_t GetCount();
    bool DecreaseCount();
    std::string GetCreationTime();
    void SetCreationTime(std::string creationtime);
    std::string GetDeadline();
    uint8_t GetInitialPiece();
    uint8_t GetFinalPiece();
    std::list<Piece> GetPieces();
    Piece *GetLastPiece();
    void AddPiece(Piece piece_to_add);

    void print();



    bool operator == (BaseOrder &ordr) const {return order_id == ordr.GetID();}
    bool operator != (BaseOrder &ordr) const {return !operator == (ordr);}
};





class Order::Piece {
private:
    uint32_t PieceID;
    uint8_t Path[59] = {0};
    uint8_t Transformations[12] = {0};
    uint8_t Machines[9] = {0};


public:
    Piece(uint32_t id);
    uint32_t GetID();
    uint8_t *GetPath();
    uint8_t *GetTransformations();
    uint8_t *GetMachines();
    void SetPath(uint8_t[]);
    void SetTransformations(uint8_t[]);
    void SetMachines(uint8_t[]);

    void print();

};

#endif
