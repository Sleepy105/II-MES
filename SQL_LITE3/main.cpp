/* 
C++ SQLITE DEMO
*/

#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

using namespace std;

static int createDB(const char* s);
static int createTable(const char* s);
static int insertData(const char* s);
static int selectData(const char* s);
static int callback(void* Notused, int argc, char** argv, char** azColName);
static int updateData(const char* s);
static int deleteData(const char* s);

int main()
{
	const char* dir = "C:\\Users\\andre\\Desktop\\sqltesteLIBGEN\\STUDENTS.db";
	sqlite3* DB;
	createDB(dir);
	createTable(dir);
	deleteData(dir);
	insertData(dir);
	//updateData(dir);
	selectData(dir);
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
	string sql = "CREATE TABLE IF NOT EXISTS GRADES(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
		"NAME TEXT NOT NULL, " \
		"LNAME TEXT NOT NULL, " \
		"AGE INT NOT NULL, " \
		"ADDRESS CHAR(50), " \
		"GRADE CHAR(1) );";
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
static int insertData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	int exit = sqlite3_open(s, &DB);
	string sql = ("INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('ALICE', 'CHAPA', 35, 'TAMPA', 'A');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('BOB', 'LEE', 20, 'DALLAS', 'B');"
		"INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES('FRED', 'COOPER', 24, 'NEW YORK', 'C');");
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
static int selectData(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	string sql = "SELECT * FROM GRADES;";

	/* An open database, SQL to be evaluated, callback function, 1st argument to callback, error msg written here*/
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

	return 0;
}

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
static int updateData(const char* s)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);

	string sql = ("UPDATE GRADES SET GRADE = 'A' WHERE LNAME = 'COOPER'");
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
static int deleteData(const char* s)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);

	string sql = "DELETE FROM GRADES;";
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
	return 0;
}