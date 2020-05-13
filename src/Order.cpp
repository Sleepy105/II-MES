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
    meslog(INFO) << "\ttype:" << std::to_string(order_type) << std::endl;
    meslog(INFO) << "\tquantity:" << std::to_string(count) << std::endl;
    meslog(INFO) << "\tinitialPiece:" << std::to_string(initialPiece) << std::endl;
    meslog(INFO) << "\tfinalPiece:" << std::to_string(finalPiece) << std::endl;
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
void BaseOrder::print(){
    if (pieces.size() == 0){
        std::cout << "\tOrder " << order_id << " has no pieces in factory floor." << std::endl;
        return;
    }
    std::cout << "\tOrder " << order_id << "has " << pieces.size() << " pieces in factory floor:" << std::endl;
    std::list<Piece>::iterator iter;
    for (iter = pieces.begin(); iter != pieces.end(); iter++){
        iter->print();
    }
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
uint8_t *Piece::GetTransformations(){
    return Transformations;
}
uint8_t *Piece::GetMachines(){
    return Machines;
}

void Piece::SetPath(uint8_t path_to_copy[]){
    int i;
    for (i = 0; i<59; i++){
        if (path_to_copy[i] == 0){
            return;
        }
        Path[i] = path_to_copy[i];
    }
}
void Piece::SetTransformations(uint8_t path_to_copy[]){
    int i;
    for (i = 0; i<12; i++){
        if (path_to_copy[i] == 0){
            return;
        }
        Transformations[i] = path_to_copy[i];
    }
}
void Piece::SetMachines(uint8_t path_to_copy[]){
    int i;
    for (i = 0; i<9; i++){
        if (path_to_copy[i] == 0){
            return;
        }
        Machines[i] = path_to_copy[i];
    }
}

void Piece::print(){
    std::cout << "\t\tPiece " << PieceID << " has path: {" << Path[0] << ", " << Path[1] << ", " << Path[2] << ", " << Path[3] << "...}" << std::endl;
}