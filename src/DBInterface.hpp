/**
 * @file DBInterface.hpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _DBINTERFACE_HPP_
#define _DBINTERFACE_HPP_

/* 
C++ SQLITE
*/
#include <string>
#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
#include <string.h>
#include "helpers.h"

typedef struct { //struct usada para obter informacao do armazem
    int* values;
    int count;
} struct_values;
/*
Insere informacao de uma Ordem. 
Order_Number: Numero da ordem. Se a Ordem for carga de pe�a ent�o Order_Number = -1
Type: So pode ser: Transformation, Dispatch, Incoming
State: So pode ser: Waiting, Executing, Finished
Initial_Piece: E a peca inicial
Final_Piece: E a peca final, se for carga ou descarga representa a mesma peca que a inicial
Total_Pieces: Numero total de pecas em questao, se for carga e 1
DeadLine: Se for carga ou descarga e -1, este valor representa segundos
Entry_Time: Hora de entrada do pedido. Se for carga a ordem de entrada e comeco de execucao e o state e logo executing
Se for tipo Incoming, a seguir a esta funcao tem que se chamar a insertDataPiece
*/
int insertDataPiece(const char* s, int Order_ID, int Order_Number);
/*
Insere informacao quando uma peca vai para a fabrica, esta pode ser descarga, transformacao ou carga.
Se for carga, esta e chamada logo a seguir � inserDataOrder, logo o ID � o ultimo ID adicionado com tipo Incoming
*/
int updateDataPiece(const char* s, int Piece_ID);
/*
Insere a hora em que a pe�a saiu da fabrica, ou porque entrou no armaz�m, ou porque foi descarregada, � necessario saber o ID da peca em questao
*/
int getOrder_ID(const char* s, std::string type, int Order_Number);
/*
Procura o Order ID com um determinado numero de ordem, se for do tipo incoming retorna o ultimo id
*/
int callback_id(void* id, int argc, char** argv, char** azColName);
/*
Fun�ao necessaria para fazer interface com DB quando se pede um valor da DB
*/
int callback(void* Notused, int argc, char** argv, char** azColName);
int updateOrder(const char* s, std::string State, int Order_ID);
/*
Faz update da informacao: Se for incoming so pode ser para finished e escreve no tempo de fim 
Se o state for executing escreve no tempo de start executing
Se o state for finished escreve no tempo de end time.
*/
int deleteData(const char* s);

int getPiece_ID(const char* s);
int checkDB(const char* s); //Verifica se a DB ja existe
int createDB(const char* s);  // Cria DB
int createTable(const char* s); //Cria Tabelas
int insertDataOrder(const char* s, int Order_Number, std::string Type, std::string State, std::string Initial_Piece, std::string Final_Piece, int Total_Pieces, std::string Deadline);
int initvalues(const char* s); //Cria os dados a serem preenchidos do warehouse e das maquinas
int getWarehouseInformation(const char* s, int* values); // Preenche o vetor values[9] sendo que cada posicao corresponde ao numero do tipo de peca 0->P1 1->P2 ... 8 ->P9
int callback_warehouse(void* v, int argc, char** argv, char** azColName);
int updateWarehouse(const char* s, std::string Type, int Quantity); // Atualiza o valor do armazem acrescentando Quantity unidades, se for -1 ou -2 entao ele retira
int updateDispatch(const char* s, std::string Zone, std::string PieceType, int Quantity); //Aumenta a quantidade de um determinado tipo de peca de uma zona em Quantity unidades
int updateMachine(const char* s, std::string Machine, std::string PieceType, int ProductionTime, int Quantity);
// Aumenta o tempo de producao em ProductionTime e o numero de pecas para valor antigo + Quantity
std::string DateTime(const char* s, std::string Deadline); //retorna a data atual mais um deadline em segundos. Se for zero e so a data atual
int callback_hour(void* DateTim, int argc, char** argv, char** azColName);
#endif