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

int insertDataPiece(const char* s, int Order_ID, int Order_Number, std::string Execution_Start) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql = ("INSERT INTO Piece(ID_ORDER, Order_Number, Execution_Start) VALUES(" \
		+ std::to_string(Order_ID) + " ," \
		+ std::to_string(Order_Number) + " ," \
		+ "'" + Execution_Start + "'" \
		+ ");");
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


int updateDataPiece(const char* s, int Piece_ID, std::string Execution_END) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql = ("UPDATE Piece SET Execution_END = '" + Execution_END + "' WHERE ID = " + std::to_string(Piece_ID));

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
	return id;
}


int callback_id(void* id, int argc, char** argv, char** azColName) {
	int *c = static_cast<int *>(id);
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


int updateData(const char* s, std::string State, int Order_ID, std::string Time) {
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
		sql1 = ("UPDATE ORDERS SET End_Time = '" + Time + "' WHERE ID = " + std::to_string(Order_ID));
	}
	else if (State == "Executing")
	{
		sql1 = ("UPDATE ORDERS SET Execution_Start = '" + Time + "' WHERE ID = " + std::to_string(Order_ID));
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
	return 0;
}


/*
Vai buscar a hora e a data atual e retorna no formato DD-MM-YYY HH:MM:SS
*/
std::string getDateTime() {
	std::string Date;
	time_t current_time;
	struct tm  local_time;

	time(&current_time);
	//localtime_s(&local_time, &current_time);
	localtime_r(&current_time, &local_time);

	int Year = local_time.tm_year + 1900;
	int Month = local_time.tm_mon + 1;
	int Day = local_time.tm_mday;

	int Hour = local_time.tm_hour;
	int Min = local_time.tm_min;
	int Sec = local_time.tm_sec;
	Date = std::to_string(Day) + "-" + std::to_string(Month) + "-" + std::to_string(Year) + " " + std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);
	return Date;
}


int checkDB(const char* s) {
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open_v2(s, &DB, SQLITE_OPEN_READONLY, NULL); 
	if (exit) {
		meslog(ERROR) << "Does not exist " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		meslog(INFO) << "Database exists!" << std::endl;
	sqlite3_close(DB);

	return 0;
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
		"Order_Number INT, " \
		"Type TEXT NOT NULL CHECK(Type ='Transformation' OR Type ='Dispatch' OR Type = 'Incoming'), " \
		"State TEXT NOT NULL DEFAULT 'Waiting' CHECK( State='Waiting' OR State ='Executing' OR State = 'Finished'), " \
		"Initial_Piece CHAR(3) NOT NULL, " \
		"Final_Piece CHAR(3) NOT NULL, " \
		"Total_Number_Pieces INT NOT NULL, " \
		"Deadline TEXT, " \
		"Entry_Time TEXT NOT NULL, " \
		"End_Time TEXT, "\
		"Execution_Start TEXT, "\
		"Remaining_Time TEXT);" \
		"CREATE TABLE IF NOT EXISTS Piece(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"ID_ORDER INT REFERENCES ORDERS(ID) ON DELETE CASCADE ON UPDATE CASCADE, "\
		"Order_Number INT REFERENCES ORDERS(Order_Number) ON DELETE CASCADE ON UPDATE CASCADE, " \
		"Execution_Start TEXT NOT NULL, " \
		"Execution_End TEXT);");
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


int insertDataOrder(const char* s, int Order_Number, std::string Type, std::string State,  std::string Initial_Piece, std::string Final_Piece, int Total_Pieces, int Deadline, std::string Entry_Time) {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	std::string sql;
	if (Type == "Incoming")
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Deadline, Entry_Time, Execution_Start)" \
			" VALUES(" \
			+ std::to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + "Executing" + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ std::to_string(Total_Pieces) + " ," \
			+ "'" + std::to_string(Deadline) + "'" + " ," \
			+ "'" + Entry_Time + "' ," \
			+ "'" + Entry_Time + "'" \
			+ ");");
	}
	else
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Deadline, Entry_Time)" \
			" VALUES(" \
			+ std::to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + State + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ std::to_string(Total_Pieces) + " ," \
			+ "'" + std::to_string(Deadline) + "'" + " ," \
			+ "'" + Entry_Time + "'" \
			+ ");");
	}
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
