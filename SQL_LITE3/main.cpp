/* 
C++ SQLITE
*/
#include <string>
#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
using namespace std;

static int checkDB(const char* s); //Verifica se a DB já existe
static int createDB(const char* s);  // Cria DB
static int createTable(const char* s); //Cria Tabelas
static int insertDataOrder(const char* s, int Order_Number, string Type, string State, string Initial_Piece, string Final_Piece, int Total_Pieces, int Deadline, string Entry_Time);
/*
Insere informação de uma Ordem. 
Order_Number: Numero da ordem. Se a Ordem for carga de peça então Order_Number = -1
Type: Só pode ser: Transformation, Dispatch, Incoming
State: Só pode ser: Waiting, Executing, Finished
Initial_Piece: É a peça inicial
Final_Piece: É a peça final, se for carga ou descarga representa a mesma peça que a inicial
Total_Pieces: Número total de peças em questão, se for carga é 1
DeadLine: Se for carga ou descarga é -1, este valor representa segundos
Entry_Time: Hora de entrada do pedido. Se for carga a ordem de entrada e começo de execução e o state é logo executing
Se for tipo Incoming, a seguir a esta função tem que se chamar a insertDataPiece
*/
static int insertDataPiece(const char* s, int Order_ID, int Order_Number, string Execution_Start);
/*
Insere informação quando uma peça vai para a fábrica, esta pode ser descarga, transformaçao ou carga.
Se for carga, esta é chamada logo a seguir à inserDataOrder, logo o ID é o ultimo ID adicionado com tipo Incoming
*/
static int updateDataPiece(const char* s, int Piece_ID, string Execution_END);
/*
Insere a hora em que a peça saiu da fábrica, ou porque entrou no armazém, ou porque foi descarregada, é necessário saber o ID da peça em questão
*/
static int getOrder_ID(const char* s, string type, int Order_Number);
/*
Procura o Order ID com um determinado numero de ordem, se for do tipo incoming retorna o ultimo id
*/
static int callback_id(void* id, int argc, char** argv, char** azColName);
/*
Funçao necessaria para fazer interface com DB quando se pede um valor da DB
*/
static int callback(void* Notused, int argc, char** argv, char** azColName);
static int updateData(const char* s, string State, int Order_ID, string Time);
/*
Faz update da informaçao: Se for incoming so pode ser para finished e escreve no tempo de fim 
Se o state for executing escreve no tempo de start executing
Se o state for finished escreve no tempo de end time.
*/
static int deleteData(const char* s);
string getDateTime();
/*
Vai buscar a hora e a data atual e retorna no formato DD-MM-YYY HH:MM:SS
*/
int main()
{
	const char* dir = "C:\\Users\\andre\\Desktop\\sqltesteLIBGEN\\factory.db"; // Definir path da DB
	sqlite3* DB;
	checkDB(dir);
	createDB(dir);
	createTable(dir);
	deleteData(dir);
	int num_Order = 4;
	string Type = "Transformation";
	string State = "Waiting";
	char Initial_Piece[3] = "P1";
	char Final_Piece[3] = "P5";
	int Total_Pieces = 40;
	int Deadline = 400;
	string NewState = "Executing";
	insertDataOrder(dir, num_Order, Type, State, Initial_Piece, Final_Piece, Total_Pieces, Deadline, getDateTime());
	insertDataPiece(dir, getOrder_ID(dir, Type, 4), 4, getDateTime());
	updateData(dir, NewState, getOrder_ID(dir, Type, num_Order), getDateTime());
	//updateDataPiece(dir, 16, getDateTime());
	return 0;
}
string getDateTime()
{
	string Date;
	time_t current_time;
	struct tm  local_time;

	time(&current_time);
	localtime_s(&local_time, &current_time);

	int Year = local_time.tm_year + 1900;
	int Month = local_time.tm_mon + 1;
	int Day = local_time.tm_mday;

	int Hour = local_time.tm_hour;
	int Min = local_time.tm_min;
	int Sec = local_time.tm_sec;
	Date = to_string(Day) + "-" + to_string(Month) + "-" + to_string(Year) + " " + to_string(Hour) + ":" + to_string(Min) + ":" + to_string(Sec);
	return Date;
}
static int checkDB(const char* s) 
{
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open_v2(s, &DB, SQLITE_OPEN_READONLY, NULL); 
	if (exit) {
		std::cerr << "Does not exist " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		std::cout << "Database exists!" << std::endl;
	sqlite3_close(DB);

	return 0;
}
static int createDB(const char* s)
{
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(s, &DB);
	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl;
	sqlite3_close(DB);

	return 0;
}
static int createTable(const char* s)
{
	sqlite3* DB;
	string sql = ("CREATE TABLE IF NOT EXISTS ORDERS(" \
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
			cerr << "Error Create Table" << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}
	return 0;
}
static int insertDataOrder(const char* s, int Order_Number, string Type, string State,  string Initial_Piece, string Final_Piece, int Total_Pieces, int Deadline, string Entry_Time)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql;
	if (Type == "Incoming")
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Deadline, Entry_Time, Execution_Start)" \
			" VALUES(" \
			+ to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + "Executing" + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ to_string(Total_Pieces) + " ," \
			+ "'" + to_string(Deadline) + "'" + " ," \
			+ "'" + Entry_Time + "' ," \
			+ "'" + Entry_Time + "'" \
			+ ");");
	}
	else
	{
		sql = ("INSERT INTO ORDERS (Order_Number, Type, State, Initial_Piece, Final_Piece, Total_Number_Pieces, Deadline, Entry_Time)" \
			" VALUES(" \
			+ to_string(Order_Number) + " ," \
			+ "'" + Type + "'" + " ," \
			+ "'" + State + "'" + " ," \
			+ "'" + Initial_Piece + "'" + " ," \
			+ "'" + Final_Piece + "'" + " ," \
			+ to_string(Total_Pieces) + " ," \
			+ "'" + to_string(Deadline) + "'" + " ," \
			+ "'" + Entry_Time + "'" \
			+ ");");
	}
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records created Successfully" << endl; 
	sqlite3_close(DB);
	return 0;
}
static int insertDataPiece(const char* s, int Order_ID, int Order_Number, string Execution_Start)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql = ("INSERT INTO Piece(ID_ORDER, Order_Number, Execution_Start) VALUES(" \
		+ to_string(Order_ID) + " ," \
		+ to_string(Order_Number) + " ," \
		+ "'" + Execution_Start + "'" \
		+ ");");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records created Successfully" << endl;
	sqlite3_close(DB);
	return 0;
}
static int updateDataPiece(const char* s, int Piece_ID, string Execution_END)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql = ("UPDATE Piece SET Execution_END = '" + Execution_END + "' WHERE ID = " + to_string(Piece_ID));

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records created Successfully" << endl;
	sqlite3_close(DB);
	return 0;
}
static int getOrder_ID(const char* s, string type, int Order_Number)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	int id = 0;
	string sql;
	if (type == "Transformation" || type == "Dispatch")
	{
		sql = "SELECT ID FROM ORDERS WHERE Order_Number = " \
			+ to_string(Order_Number) + " AND Type = '" + type + "';";
	}
	else if (type == "Incoming")
	{
		sql = "SELECT ID FROM ORDERS WHERE Type = 'Incoming' ORDER BY ID DESC LIMIT 1;";
	}
	/* An open database, SQL to be evaluated, callback function, 1st argument to callback, error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback_id, &id, NULL);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in select statement " << endl;
	}
	else {
		std::cerr << "Records returned" << endl;
	}
	return id;
}
static int callback_id(void* id, int argc, char** argv, char** azColName)
{
	int *c = static_cast<int *>(id);
	*c = atoi(argv[0]);
	return 0;
}
// note that callback is called each time with the information of row
// if we have 6 rows, callback it will be called 6 times
// retrieve contens of database used by selectData()
/* argc: holds the number of results, azColName: holds each column returned in array, argv: holds each value in array*/
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		//column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}
	cout << endl;
	return 0;
}
static int updateData(const char* s, string State, int Order_ID, string Time)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql1;
	string sql = ("UPDATE ORDERS SET State = '" + State + "' WHERE ID = " + to_string(Order_ID));

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records created Successfully" << endl;
	if (State == "Finished")
	{
		sql1 = ("UPDATE ORDERS SET End_Time = '" + Time + "' WHERE ID = " + to_string(Order_ID));
	}
	else if (State == "Executing")
	{
		sql1 = ("UPDATE ORDERS SET Execution_Start = '" + Time + "' WHERE ID = " + to_string(Order_ID));
	}
	exit = sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records created Successfully" << endl;
	sqlite3_close(DB);
	return 0;
}
static int deleteData(const char* s)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	string sql1 = "DELETE FROM Piece;";
	sqlite3_exec(DB, sql1.c_str(), callback, NULL, NULL);
	string sql = "DELETE FROM ORDERS;";
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
	return 0;
}