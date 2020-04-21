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

#include "helpers.h"

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
int insertDataPiece(const char* s, int Order_ID, int Order_Number, std::string Execution_Start);
/*
Insere informacao quando uma peca vai para a fabrica, esta pode ser descarga, transformacao ou carga.
Se for carga, esta e chamada logo a seguir � inserDataOrder, logo o ID � o ultimo ID adicionado com tipo Incoming
*/
int updateDataPiece(const char* s, int Piece_ID, std::string Execution_END);
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
int updateData(const char* s, std::string State, int Order_ID, std::string Time);
/*
Faz update da informacao: Se for incoming so pode ser para finished e escreve no tempo de fim 
Se o state for executing escreve no tempo de start executing
Se o state for finished escreve no tempo de end time.
*/
int deleteData(const char* s);
std::string getDateTime();

int checkDB(const char* s); //Verifica se a DB ja existe
int createDB(const char* s);  // Cria DB
int createTable(const char* s); //Cria Tabelas
int insertDataOrder(const char* s, int Order_Number, std::string Type, std::string State, std::string Initial_Piece, std::string Final_Piece, int Total_Pieces, int Deadline, std::string Entry_Time);


#endif