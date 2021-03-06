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
    const uint8_t NULL_ORDER = 4;
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
    bool is_not_executing;

public:
    BaseOrder();
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
    std::list<Piece> *GetPieces();
    Piece *GetLastPiece();
    void SetPK(int pk);
    void AddPiece(Piece piece_to_add);
    bool IsNotExecuting();
    void SetExecuting();

    void print();



    bool operator == (BaseOrder &ordr) const {return order_id == ordr.GetID();}
    bool operator != (BaseOrder &ordr) const {return !operator == (ordr);}
};


/**
 * @brief Stores all information related to a Path to be travelled by a Piece
 * 
 */
typedef struct {
    uint8_t moves[59] = {0};
    uint8_t transformations[12] = {0};
    uint8_t machine_transformations[9] = {0};
} Path;

class Order::Piece {
private:
    uint32_t PieceID;
    Path* path_ = NULL;

public:
    Piece(uint32_t id);
    uint32_t GetID();
    Path* GetPath();
    uint8_t* GetMoves();
    uint8_t *GetTransformations();
    uint8_t *GetMachines();

    /**
     * @brief Set the Path object
     * 
     * @param path 
     */
    void SetPath(Path* new_path);

    void print();

};

#endif
