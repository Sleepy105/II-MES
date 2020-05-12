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
// 3 construtores possiveis:
//  -com deadline em string: mais rapido, envolve ter a string em YYYY-MM-DD HH:MM:SS
//  -com deadline em inteiro: mais lento (converte para string pela base de dados)
//  -sem deadline: no caso de request stores
BaseOrder::BaseOrder(uint8_t order_id, 
                     uint8_t order_type, 
                     uint32_t count,
                     uint8_t initialPiece,
                     uint8_t finalPiece,
                     std::string deadline) : order_id(order_id), order_type(order_type), count(count), initialPiece(initialPiece), finalPiece(finalPiece) {
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
    if (order_type == Order::ORDER_TYPE_UNLOAD){
        order_id = -1;
    }
    Deadline = deadline;
}
BaseOrder::BaseOrder(uint8_t order_id, 
                     uint8_t order_type, 
                     uint32_t count,
                     uint8_t initialPiece,
                     uint8_t finalPiece,
                     int deadline) : order_id(order_id), order_type(order_type), count(count), initialPiece(initialPiece), finalPiece(finalPiece) {
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
    if (order_type == Order::ORDER_TYPE_UNLOAD){
        order_id = -1;
    }
    // deadline (letras minusculas) esta em segundos
    Deadline = DateTime("factory.db", std::to_string(deadline));
}
BaseOrder::BaseOrder(uint8_t order_id, uint8_t order_type): order_id(order_id), order_type(order_type){
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
}

BaseOrder::~BaseOrder() {
    // A partida nao vai ser preciso. Variaveis std:: sao self managed (destroem-se e desreferenciam-se automaticamente, na maioria
    // dos casos) por isso nao deve ser preciso destruir a lista de pecas (que em principio estara vazia de qualquer forma)
}


bool BaseOrder::is_valid() {
    // TODO check this shizzz
    return true;
}
uint32_t BaseOrder::GetID(){
    return order_id;
}
int BaseOrder::GetPK(){
    return order_pk;
}
uint8_t BaseOrder::GetType(){
    return order_type;
}
uint32_t BaseOrder::GetCount(){
    return count;
}
bool BaseOrder::DecreaseCount(){
    if (count>0){
        count--;
        return true;
    }
    return false;
}
std::string BaseOrder::GetCreationTime(){
    return CreationTime;
}
void BaseOrder::SetCreationTime(std::string creationtime){
    CreationTime = DateTime("factory.db", "0");
}

std::string BaseOrder::GetDeadline(){
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
void Piece::SetPath(uint8_t path_to_copy[]){
    int i;
    for (i = 0; i<59; i++){
        Path[i] = path_to_copy[i];
    }
}