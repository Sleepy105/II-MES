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
<<<<<<< HEAD
                     uint8_t finalPiece,
                     time_t deadline) : order_id(order_id), order_type(order_type), count(quantity), initialPiece(initialPiece), finalPiece(finalPiece) {
=======
                     uint8_t finalPiece) : order_id(order_id), order_type(order_type), count(quantity), initialPiece(initialPiece), finalPiece(finalPiece) {
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
    if (order_type == Order::ORDER_TYPE_UNLOAD){
        order_id = -1;
    }
    Deadline = deadline;
}
BaseOrder::~BaseOrder() {
}
<<<<<<< HEAD


bool BaseOrder::is_valid() {
    // TODO check this shizzz
    return true;
}
=======
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf
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
<<<<<<< HEAD
    piece_on_hold = true;
=======
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf
}
uint32_t Piece::GetID(){
    return PieceID;
}
uint8_t *Piece::GetPath(){
    return Path;
}
<<<<<<< HEAD
bool Piece::isOnHold(){
    return piece_on_hold;
}
void Piece::setPieceAsNotOnHold(){
    piece_on_hold = false;
}
=======
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf

