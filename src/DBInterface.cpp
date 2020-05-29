/**
 * @file DBInterface.cpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "DBInterface.hpp"

int insertDataPiece(const char* s, int Order_ID)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql = ("INSERT INTO Piece(ID_ORDER, Execution_Start) VALUES(" \
		+ std::to_string(Order_ID) + " ," \
		+ " datetime('now','localtime') "  \
		+ ");");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
		exit = -1;
	}
	else
	{
		meslog(INFO) << "Records created Successfully" << std::endl;
		exit = getPiece_ID(s);
	}
	sqlite3_close(DB);
	return exit;
}

int getPiece_ID(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	int id = 0;
	std::string sql = "SELECT ID FROM Piece ORDER BY ID DESC LIMIT 1;";;

	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &id, NULL);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in select statement " << std::endl;
	}
	else {
		std::cerr << "Records returned" << std::endl;
	}
	sqlite3_close(DB);
	return id;
}


int updateDataPiece(const char* s, int Piece_ID)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql = ("UPDATE Piece SET Execution_END = datetime('now','localtime') WHERE ID = " + std::to_string(Piece_ID));

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	sqlite3_close(DB);
	return 0;
}


int getOrder_ID(const char* s, std::string type, int Order_Number) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	int id = 0;
	std::string sql;
	if (type == "Transformation" || type == "Dispatch")
	{
		sql = "SELECT ID FROM ORDERS WHERE Order_Number = " \
			+ std::to_string(Order_Number) + " AND Type = '" + type + "';";
	}
	else if (type == "Incoming")
	{
		sql = "SELECT ID FROM ORDERS WHERE Type = 'Incoming' ORDER BY ID DESC LIMIT 1;";
	}
	/* An open database, SQL to be evaluated, callback function, 1st argument to callback, error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &id, NULL);
	if (exit != SQLITE_OK) {
		meslog(ERROR) << "Error in select statement " << std::endl;
	}
	else {
		meslog(INFO) << "Records returned" << std::endl;
	}
	sqlite3_close(DB);
	return id;
}


int callback_id(void* id, int argc, char** argv, char** azColName) {
	int *c = (int *)(id);
	*c = atoi(argv[0]);
	return 0;
}


// note that callback is called each time with the information of row
// if we have 6 rows, callback it will be called 6 times
// retrieve contens of database used by selectData()
/* argc: holds the number of results, azColName: holds each column returned in array, argv: holds each value in array*/
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		//column name and value
		meslog(INFO) << azColName[i] << ": " << argv[i] << std::endl;
	}
	return 0;
}


int updateOrder(const char* s, std::string State, int Order_ID) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql1;
	std::string sql = ("UPDATE ORDERS SET State = '" + State + "' WHERE ID = " + std::to_string(Order_ID));

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	if (State == "Finished")
	{
		sql1 = ("UPDATE ORDERS SET End_Time = datetime('now', 'localtime')  WHERE ID = " + std::to_string(Order_ID));
	}
	else if (State == "Executing")
	{
		sql1 = ("UPDATE ORDERS SET Execution_Start = datetime('now', 'localtime') WHERE ID = " + std::to_string(Order_ID));
	}
	exit = sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	sqlite3_close(DB);
	return 0;
}


int deleteData(const char* s) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	std::string sql1 = "DELETE FROM Piece;";
	sqlite3_exec(DB, sql1.c_str(), callback, NULL, NULL);
	std::string sql = "DELETE FROM ORDERS;";
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
	std::string sql2 = "DELETE FROM Warehouse;";
	sqlite3_exec(DB, sql2.c_str(), callback, NULL, NULL);
	std::string sql3 = "DELETE FROM Machine;";
	sqlite3_exec(DB, sql3.c_str(), callback, NULL, NULL);
	std::string sql4 = "DELETE FROM Dispatch;";
	sqlite3_exec(DB, sql4.c_str(), callback, NULL, NULL);
	sqlite3_close(DB);
	return 0;
}
/*
Vai buscar a hora e a data atual e retorna no formato DD-MM-YYY HH:MM:SS
*/

int checkDB(const char* s) {
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open_v2(s, &DB, SQLITE_OPEN_READONLY, NULL); 
	if (exit) {
		meslog(ERROR) << "Does not exist " << sqlite3_errmsg(DB) << std::endl;
		return 0;
	}
	else {
		meslog(INFO) << "Database exists!" << std::endl;
		return 1;
	}
	sqlite3_close(DB);
}


int createDB(const char* s) {
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(s, &DB);
	if (exit) {
		meslog(ERROR) << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		meslog(INFO) << "Opened Database Successfully!" << std::endl;
	sqlite3_close(DB);

	return 0;
}


int createTable(const char* s) {
	sqlite3* DB;
	std::string sql = ("CREATE TABLE IF NOT EXISTS ORDERS(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
		"Order_Number INT UNIQUE, " \
		"Type TEXT NOT NULL CHECK(Type ='Transformation' OR Type ='Dispatch' OR Type = 'Incoming'), " \
		"State TEXT NOT NULL DEFAULT 'Waiting' CHECK( State='Waiting' OR State ='Executing' OR State = 'Finished'), " \
		"Initial_Piece CHAR(3) NOT NULL CHECK(Initial_Piece ='P1' OR Initial_Piece ='P2' OR Initial_Piece ='P3' OR Initial_Piece ='P4' OR Initial_Piece ='P6' OR Initial_Piece ='P7' OR Initial_Piece ='P8' OR Initial_Piece = 'P9'), " \
		"Final_Piece CHAR(3) NOT NULL CHECK(Final_Piece ='P1' OR Final_Piece ='P2' OR Final_Piece ='P3' OR Final_Piece ='P4' OR Final_Piece ='P5' OR Final_Piece ='P6' OR Final_Piece ='P7' OR Final_Piece ='P8' OR Final_Piece ='P9'), " \
		"Total_Number_Pieces INT NOT NULL, " \
		"Deadline TEXT, " \
		"Entry_Time TEXT NOT NULL, " \
		"End_Time TEXT, "\
		"Execution_Start TEXT);" \
		"CREATE TABLE IF NOT EXISTS Piece(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"ID_ORDER INTEGER, "\
		"Execution_Start TEXT NOT NULL, " \
		"Execution_End TEXT, FOREIGN KEY(ID_ORDER) REFERENCES ORDERS(ID));" \
		"CREATE TABLE IF NOT EXISTS Warehouse(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"PieceType CHAR(3) NOT NULL, "\
		"Quantity INT NOT NULL DEFAULT 0);"\
		"CREATE TABLE IF NOT EXISTS Dispatch(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"Zone INTEGER NOT NULL, "\
		"PieceType CHAR(3) NOT NULL, "\
		"Quantity INT NOT NULL DEFAULT 0);" \
		"CREATE TABLE IF NOT EXISTS Machine("\
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"MachineType CHAR(3) NOT NULL, "\
		"ProductionTime INTEGER NOT NULL DEFAULT 0, " \
		"PieceType CHAR(3) NOT NULL, " \
		"Quantity INT NOT NULL DEFAULT 0);"\
		);
	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);

		char* messageError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK)
		{
			meslog(ERROR) << "Error Create Table" << std::endl;
			sqlite3_free(messageError);
		}
		else
			meslog(INFO) << "Table created Successfully" << std::endl;
		sqlite3_close(DB);
	}
	catch (const std::exception e)
	{
		meslog(ERROR) << e.what();
	}
	return 0;
}


int insertDataOrder(const char* s, int Order_Number, std::string Type, std::string State, std::string Initial_Piece, std::string Final_Piece, int Total_Pieces, std::string Deadline)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql;
	if (Type == "Incoming")
	{
		sql = ("INSERT INTO ORDERS (Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Entry_Time, Execution_Start)" \
			" VALUES( '" \
			+ Type + "'" + " ," \
			+ "'" + "Executing" + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ std::to_string(Total_Pieces) + " ," \
			+ " datetime('now', 'localtime') " + ", " \
			+ " datetime('now','localtime') "  \
			+ ");");
	}
	else if (Type == "Transformation")
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Deadline, Entry_Time)" \
			" VALUES(" \
			+ std::to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + State + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ std::to_string(Total_Pieces) + " ," \
			+ "'" + Deadline + "' " + " ," \
			+ " datetime('now','localtime') " + \
			+ ");");
	}
	else
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Entry_Time)" \
			" VALUES(" \
			+ std::to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + State + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ std::to_string(Total_Pieces) + " ," \
			+ " datetime('now','localtime') " + \
			+ ");");
	}
	//meslog(INFO) << sql << std::endl;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
		exit = -1;
	}
	else
	{
		meslog(INFO) << "Records created Successfully" << std::endl;
		exit = getOrder_ID(s, Type, Order_Number);
	}
	sqlite3_close(DB);
	return exit;
}
int initvalues(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	meslog(INFO) << "ENTREI" << std::endl;
	char* messageError1;
	
	std::string sql = ("INSERT INTO Warehouse (PieceType, Quantity) VALUES('P1', 54), ('P2', 54),('P3', 54),('P4', 54),('P5', 54),('P6', 54),('P7',54 ), ('P8',54) , ('P9',54);");
	int exit1 = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError1);
	sql = ("INSERT INTO Machine (MachineType, PieceType) VALUES ('A1', 'P1'), ('A1', 'P2'), ('A1', 'P6'), ('A2', 'P1'), ('A2', 'P2'), ('A2', 'P6'), ('A3', 'P1'), ('A3', 'P2'), ('A3', 'P6'), ('B1', 'P1'), ('B1', 'P3'), ('B1', 'P7'),('B2', 'P1'), ('B2', 'P3'), ('B2', 'P7'),('B3', 'P1'), ('B3', 'P3'), ('B3', 'P7'),('C1', 'P1'), ('C1', 'P4'), ('C1', 'P8'),('C2', 'P1'), ('C2', 'P4'), ('C2', 'P8'),('C3', 'P1'), ('C3', 'P4'), ('C3', 'P8'); ");
	exit1 = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError1);
	sql = ("INSERT INTO Dispatch (Zone, PieceType) VALUES "\
		"('Zone1', 'P1'), ('Zone1', 'P2'), ('Zone1', 'P3'), ('Zone1', 'P4'), ('Zone1', 'P5'), ('Zone1', 'P6'), ('Zone1', 'P7'), ('Zone1', 'P8'), ('Zone1', 'P9'), "\
		"('Zone2', 'P1'), ('Zone2', 'P2'), ('Zone2', 'P3'), ('Zone2', 'P4'), ('Zone2', 'P5'), ('Zone2', 'P6'), ('Zone2', 'P7'), ('Zone2', 'P8'), ('Zone2', 'P9'), "\
		"('Zone3', 'P1'), ('Zone3', 'P2'), ('Zone3', 'P3'), ('Zone3', 'P4'), ('Zone3', 'P5'), ('Zone3', 'P6'), ('Zone3', 'P7'), ('Zone3', 'P8'), ('Zone3', 'P9'); "\
		");");
	exit1 = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError1);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError1);
		exit = -1;
	}
	else
	{
		meslog(INFO) << "Records created Successfully" << std::endl;
		exit = 0;
	}
	sqlite3_close(DB);
	return exit;
}
int callback_warehouse(void* v, int argc, char** argv, char** azColName)
{
	struct_values* temp = (struct_values*)(v);
	temp->values[temp->count] = atoi(argv[0]);
	temp->count++;
	if (temp->count == 9)
	{
		temp->count = 0;
	}
	return 0;
}
int getWarehouseInformation(const char* s, int* values)
{
	struct_values v;
	v.count = 0;
	v.values = values;
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	std::string sql = "SELECT Quantity FROM Warehouse";
	exit = sqlite3_exec(DB, sql.c_str(), callback_warehouse, &v, NULL);
	sqlite3_close(DB);
	return 0;
}

int updateWarehouse(const char* s, std::string Type, int Quantity)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	int value = 0;

	std::string sql = "SELECT Quantity FROM Warehouse WHERE PieceType = " \
		"'" + Type + "';";
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &value, NULL);
	if (exit != SQLITE_OK) {
		meslog(ERROR) << "Error in select statement " << std::endl;
	}
	else {
		meslog(INFO) << "Records returned" << std::endl;
	}
	value = value + Quantity;

	sql = ("UPDATE Warehouse SET Quantity = " + std::to_string(value) + " WHERE PieceType = '" + Type + "'");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	sqlite3_close(DB);
	return 0;
}
int updateDispatch(const char* s, std::string Zone, std::string PieceType, int Quantity)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	int value = 0;

	std::string sql = "SELECT Quantity FROM Dispatch WHERE PieceType = " \
		"'" + PieceType + "' AND Zone = '" + Zone + "';";
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &value, NULL);
	if (exit != SQLITE_OK) {
		meslog(ERROR) << "Error in select statement " << std::endl;
	}
	else {
		meslog(INFO) << "Records returned" << std::endl;
	}
	value = value + Quantity;
	sql = ("UPDATE Dispatch SET Quantity = " + std::to_string(value) + " WHERE PieceType = '" + PieceType + "' AND Zone = '" + Zone + "'");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	sqlite3_close(DB);
	return 0;
}
int updateMachine(const char* s, std::string Machine, std::string PieceType, int ProductionTime, int Quantity)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	int value = 0;
	int time_p = 0;

	std::string sql = "SELECT Quantity FROM Machine WHERE MachineType = " \
		"'" + Machine + "' AND PieceType = '" + PieceType + "';";
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &value, NULL);
	if (exit != SQLITE_OK) {
		meslog(ERROR) << "Error in select statement " << std::endl;
	}
	else {
		meslog(INFO) << "Records returned" << std::endl;
	}
	value = value + Quantity;

	sql = "SELECT ProductionTime FROM Machine WHERE MachineType = " \
		"'" + Machine + "' AND PieceType = '" + PieceType + "';";
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &time_p, NULL);
	if (exit != SQLITE_OK) {
		meslog(ERROR) << "Error in select statement " << std::endl;
	}
	else {
		meslog(INFO) << "Records returned" << std::endl;
	}
	time_p = time_p + ProductionTime;

	sql = ("UPDATE Machine SET Quantity = " + std::to_string(value) + ", ProductionTime = " + std::to_string(time_p) \
		+ " WHERE PieceType = '" + PieceType + "' AND MachineType = '" + Machine + "'");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		meslog(ERROR) << "Error Insert" << std::endl;
		sqlite3_free(messageError);
	}
	else
		meslog(INFO) << "Records created Successfully" << std::endl;
	sqlite3_close(DB);
	return 0;
}

std::string DateTime(const char* s, std::string Deadline)
{
	std::string temp;
	char DateTim[30];
	sqlite3* DB;
	sqlite3_open(s, &DB);
	//std::string sql = "SELECT datetime('now', 'localtime')";
	std::string sql = "SELECT datetime('now', 'localtime' , '+" + Deadline + " seconds')";
	sqlite3_exec(DB, sql.c_str(), callback_hour, DateTim, NULL);
	temp = DateTim;
	sqlite3_close(DB);
	return temp;
}
int callback_hour(void* DateTim, int argc, char** argv, char** azColName)
{
	strcpy((char*)(DateTim), argv[0]);
	return 0;
}

//// Funcoes da DB para restore da informacao toda das orderns para o mesh ////
//// Para dar restore a informacao do warehouse é usar a funcao getWarehouseInformation ////
Load_Unload RestoreOrders;

Load_Unload RestoreMeshOrders(const char* s) {
	int count_disp[56] = { 0 }; //auxiliar para ir buscar quantas pecas ainda faltam executar em cada ordem
	int count_transf[56] = { 0 };
	int pos = 0;
	//vai buscar as orders
	RestoreIncomingDispatch(s);
	RestoreTransformation(s);

	//atualiza quantas pecas ainda faltavam ser mandadas para a fábrica
	RestorecountDispatch(s, count_disp);
	for (int i = 0; i < RestoreOrders.vectorPositionDispatchIncoming; i++) {
		if (RestoreOrders.RestoreDispatch_Incoming[i].Type == "Dispatch") {
			RestoreOrders.RestoreDispatch_Incoming[i].count = count_disp[pos];
			pos++;
		}
	}
	RestorecountTransformation(s, count_transf);
	for (int i = 0; i < RestoreOrders.vectorPositionTransformation; i++) {
		RestoreOrders.RestoreTransformation[i].count = count_transf[i];
	}

	//atualiza a estrutura de pecas inerente a cada order
	for (int i = 0; i < RestoreOrders.vectorPositionDispatchIncoming; i++) {
		if (RestoreOrders.RestoreDispatch_Incoming[i].State == "Executing") {
			getPieceInformation(s, i, RestoreOrders.RestoreDispatch_Incoming);
		}
	}
	for (int i = 0; i < RestoreOrders.vectorPositionTransformation; i++) {
		if (RestoreOrders.RestoreTransformation[i].State == "Executing") {
			getPieceInformationTrans(s, i, RestoreOrders.RestoreTransformation);
		}
	}
	return RestoreOrders;
}
int global_position = 0; //usado para saber a posicao atual dos vetores InofmrationDiscInc e Transformation

void getPieceInformationTrans(const char* s, int position, Transformation* Transform)
{
	global_position = position;
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = ("SELECT ID, ID_ORDER FROM Piece Where Execution_End IS NULL AND ID_ORDER = "\
		+ std::to_string(Transform[position].order_pk) + " ORDER BY Execution_Start ASC");
	int exit = sqlite3_exec(DB, sql.c_str(), callbackPieceTransform, Transform, NULL);
}
int callbackPieceTransform(void* NotUsed, int argc, char** argv, char** azColName)
{
	RestoreOrders.RestoreTransformation[global_position].pieces[RestoreOrders.RestoreTransformation[global_position].vectorPiecePosition].id_piece = atoi(argv[0]);
	RestoreOrders.RestoreTransformation[global_position].pieces[RestoreOrders.RestoreTransformation[global_position].vectorPiecePosition].fk_order_id = atoi(argv[1]);
	RestoreOrders.RestoreTransformation[global_position].vectorPiecePosition++;
	return 0;
}
void getPieceInformation(const char* s, int position, InformationDisInc *DispInc){
	global_position = position;
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = ("SELECT ID, ID_ORDER FROM Piece Where Execution_End IS NULL AND ID_ORDER = "\
		+ std::to_string(DispInc[position].order_pk) + " ORDER BY Execution_Start ASC");
	int exit = sqlite3_exec(DB, sql.c_str(), callbackPieceIncomingDispatch, NULL, NULL);
}
int callbackPieceIncomingDispatch(void* NotUsed, int argc, char** argv, char** azColName)
{
	RestoreOrders.RestoreDispatch_Incoming[global_position].pieces[RestoreOrders.RestoreDispatch_Incoming[global_position].vectorPiecePosition].id_piece = atoi(argv[0]);
	RestoreOrders.RestoreDispatch_Incoming[global_position].pieces[RestoreOrders.RestoreDispatch_Incoming[global_position].vectorPiecePosition].fk_order_id = atoi(argv[1]);
	RestoreOrders.RestoreDispatch_Incoming[global_position].vectorPiecePosition++;
	return 0;
}
void RestoreIncomingDispatch(const char* s)
{
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = "SELECT ID, Type, State, Initial_Piece, Final_Piece, Execution_Start FROM ORDERS WHERE END_TIME IS NULL AND (Type = 'Dispatch' OR Type = 'Incoming') ORDER BY Execution_Start ASC";
	sqlite3_exec(DB, sql.c_str(), callbackRestoreIncomingDispatch, NULL, NULL);
	sqlite3_close(DB);
}
int callbackRestoreIncomingDispatch(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		//column name and value
		if (i == 0)
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].order_pk = atoi(argv[i]);
		else if (i == 1)
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].Type = argv[i];
		else if (i == 2)
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].State = argv[i];
		else if (i == 3)
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].initialPiece = argv[i][1] - '0';
		else if (i == 4)
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].finalPiece = argv[i][1] - '0';
		else
			RestoreOrders.RestoreDispatch_Incoming[RestoreOrders.vectorPositionDispatchIncoming].CreationTime = argv[i];
	}
	RestoreOrders.vectorPositionDispatchIncoming++;
	return 0;
}
void RestoreTransformation(const char* s)
{
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = "SELECT ID, Type, State, Initial_Piece, Final_Piece, Deadline FROM ORDERS WHERE END_TIME IS NULL AND (Type = 'Transformation') ORDER BY Deadline ASC";
	sqlite3_exec(DB, sql.c_str(), callbackRestoreTransformation, NULL, NULL);
	sqlite3_close(DB);
}
int callbackRestoreTransformation(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		//column name and value
		if (i == 0)
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].order_pk = atoi(argv[i]);
		else if (i == 1)
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].Type = argv[i];
		else if (i == 2)
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].State = argv[i];
		else if (i == 3)
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].initialPiece = argv[i][1] - '0';
		else if (i == 4)
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].finalPiece = argv[i][1] - '0';
		else
			RestoreOrders.RestoreTransformation[RestoreOrders.vectorPositionTransformation].Deadline = argv[i];
	}
	RestoreOrders.vectorPositionTransformation++;
	return 0;
}
int callbackCount(void* v, int argc, char** argv, char** azColName)
{
	struct_values* temp = (struct_values*)(v);
	temp->values[temp->count] = atoi(argv[0]);
	temp->count++;
	return 0;
}
void RestorecountDispatch(const char *s, int *temp) {
	struct_values v;
	v.count = 0;
	v.values = temp;
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = ("SELECT (Total_Number_Pieces - (Case When t1.cont IS NULL Then 0 ELSE t1.cont END) - (Case When t2.cont IS NULL Then 0 ELSE t2.cont END)) AS Waiting "\
		"FROM ORDERS t "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece "\
		"Where Execution_End is NULL Group By Id_Order) t1 ON t1.ID_Order = t.ID "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece Where Execution_End is NOT NULL Group By Id_Order) t2 ON t2.ID_Order = t.ID "\
		"Where(State = 'Waiting' OR State = 'Executing') AND(Type = 'Dispatch') ORDER BY Entry_Time ASC; ");
	int exit = sqlite3_exec(DB, sql.c_str(), callbackCount, &v, NULL);
	sqlite3_close(DB);
}
void RestorecountTransformation(const char* s, int *temp) {
	struct_values v;
	v.count = 0;
	v.values = temp;
	sqlite3* DB;
	sqlite3_open(s, &DB);
	std::string sql = ("SELECT (Total_Number_Pieces - (Case When t1.cont IS NULL Then 0 ELSE t1.cont END) - (Case When t2.cont IS NULL Then 0 ELSE t2.cont END)) AS Waiting "\
		"FROM ORDERS t "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece "\
		"Where Execution_End is NULL Group By Id_Order) t1 ON t1.ID_Order = t.ID "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece Where Execution_End is NOT NULL Group By Id_Order) t2 ON t2.ID_Order = t.ID "\
		"Where(State = 'Waiting' OR State = 'Executing') AND(Type = 'Transformation') ORDER BY Deadline ASC;); ");
	int exit = sqlite3_exec(DB, sql.c_str(), callbackCount, &v, NULL);
	sqlite3_close(DB);
}