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
    if (order_type == Order::ORDER_TYPE_LOAD){
        order_id = -1;
    }
    is_not_executing = true;
    Deadline = deadline;
}
BaseOrder::BaseOrder(uint8_t order_id, 
                     uint8_t order_type, 
                     uint32_t count,
                     uint8_t initialPiece,
                     uint8_t finalPiece,
                     int deadline) : order_id(order_id), order_type(order_type), count(count), initialPiece(initialPiece), finalPiece(finalPiece) {
    meslog(INFO) << "ORDER " << std::to_string(order_id) << " created." << std::endl;
    if (order_type == Order::ORDER_TYPE_LOAD){
        order_id = -1;
    }
    is_not_executing = true;
    // deadline (letras minusculas) esta em segundos
    Deadline = DateTime(DBFILE, std::to_string(deadline));
}
BaseOrder::BaseOrder(uint8_t order_id, uint8_t order_type): order_id(order_id), order_type(order_type){
    is_not_executing = true;
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
void BaseOrder::SetPK(int pk){
    order_pk = pk;
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
        if (count == 0 && order_type == ORDER_TYPE_UNLOAD){
            updateOrder(DBFILE, "Finished", order_pk);
        }
        return true;
    }
    return false;
}
std::string BaseOrder::GetCreationTime(){
    return CreationTime;
}
void BaseOrder::SetCreationTime(std::string creationtime){
    CreationTime = DateTime(DBFILE, "0");
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
std::list<Piece> *BaseOrder::GetPieces(){
    return &pieces;
}
void BaseOrder::AddPiece(Piece piece_to_add){
    pieces.push_back(piece_to_add);
}
Piece *BaseOrder::GetLastPiece(){
    return &(pieces.back());
}

bool BaseOrder::IsNotExecuting(){
    return is_not_executing;
}

void BaseOrder::SetExecuting(){
    is_not_executing = false;
}


void BaseOrder::print(){
    std::string type;
    switch (order_type)
    {
    case ORDER_TYPE_LOAD:
        type = "Load";
        break;
    case ORDER_TYPE_TRANSFORMATION:
        type = "Transformation";
        break;
    case ORDER_TYPE_UNLOAD:
        type = "Unload";
        break;
    default:
        type = "Undefined";
        break;
    }
    if (pieces.size() == 0){
        std::cout << "\t" << type << "Order " << order_id << " has no pieces in factory floor." << std::endl;
        return;
    }
    std::cout << "\t" << type << "Order " << order_id << " has " << pieces.size() << " piece(s) in factory floor:" << std::endl;
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

Path* Piece::GetPath() {
    return path_;
}

uint8_t* Piece::GetMoves() {
    Path* path = GetPath();
    return path ? path->moves : NULL;
}
uint8_t *Piece::GetTransformations(){
    Path* path = GetPath();
    return path ? path->transformations : NULL;
}
uint8_t *Piece::GetMachines(){
    Path* path = GetPath();
    return path ? path->machine_transformations : NULL;
}

void Piece::SetPath(Path* new_path){
    path_ = new_path;
}

void Piece::print(){
    uint8_t* moves = GetMoves();
    if (!moves) {
        std::cout << "\t\tPiece " << GetID() << " has no path recorded in MES" << std::endl;
        return;
    }
    std::cout << "\t\tPiece " << GetID() << " has path: {" << (int)moves[0] << ", " << (int)moves[1] << ", " << (int)moves[2] << ", " << (int)moves[3] << "...}" << std::endl;
}