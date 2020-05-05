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

BaseOrder::BaseOrder(uint8_t order_id, 
                     uint8_t order_type, 
                     uint32_t quantity,
                     uint8_t initialPiece,
                     uint8_t finalPiece) : order_id(order_id), order_type(order_type), count(quantity), initialPiece(initialPiece), finalPiece(finalPiece) {
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
}
BaseOrder::~BaseOrder() {
}
uint32_t BaseOrder::GetID(){
    return order_id;
}
uint32_t BaseOrder::GetPK(){
    return order_pk;
}
uint8_t BaseOrder::GetType(){
    return order_type;
}
uint32_t BaseOrder::GetCount(){
    return count;
}
time_t BaseOrder::GetCreationTime(){
    return CreationTime;
}
time_t BaseOrder::GetDeadline(){
    return Deadline;
}
uint8_t BaseOrder::GetInitialPiece(){
    return initialPiece;
}
uint8_t BaseOrder::GetFinalPiece(){
    return finalPiece;
}
std::list<Piece> BaseOrder::GetPieces(){
    return pieces;
}

Piece::Piece(uint32_t id){
    PieceID = id;
}
uint32_t Piece::GetID(){
    return PieceID;
}
uint8_t *Piece::GetPath(){
    return Path;
}

