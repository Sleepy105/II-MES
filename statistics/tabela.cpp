#include <iostream>

#define TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#define TEXTTABLE_USE_EN_US_UTF8

#include "TextTable.hpp"

int main()
{
    const char* dir = "factory.db"; // Definir path da DB
	int c;
	puts("Statistics: \nPress m for Machine \nPress d for Dispatch \nPress o for Orders");

	while (1)
	{
		c = getchar();
		if (c == 'm')
		{
			TextTable machine('-', '|', '+');
			TextTable* ptr;
			ptr = &machine;
			printStatisticsMachine(dir, ptr);

			ptr->setAlignment(2, TextTable::Alignment::RIGHT);
			std::cout << *ptr;
			puts("Statistics: \nPress m for Machine \nPress d for Dispatch \nPress o for Orders");
		}
		else if (c == 'd')
		{
			TextTable dispatch('-', '|', '+');
			TextTable* ptr;
			ptr = &dispatch;
			printStatisticsDisptach(dir, ptr);
			ptr->setAlignment(2, TextTable::Alignment::RIGHT);
			std::cout << *ptr;
			puts("Statistics: \nPress m for Machine \nPress d for Dispatch \nPress o for Orders");
		}
		else if (c == 'o')
		{
			TextTable orderExec('-', '|', '+');
			TextTable orderWait('-', '|', '+');
			TextTable* ptr, *ptr2;
			ptr = &orderExec;
			printStatisticsOrderExecuting(dir, ptr);
			ptr->setAlignment(2, TextTable::Alignment::RIGHT);
			std::cout << *ptr;

			TextTable t('-', '|', '+');
			printStatisticsOrderFinished(dir, &t);
			t.setAlignment(2, TextTable::Alignment::RIGHT);
			std::cout << t;

			ptr2 = &orderWait;
			printStatisticsOrderWait(dir, ptr2);
			ptr2->setAlignment(2, TextTable::Alignment::RIGHT);
			std::cout << *ptr2;
			puts("Statistics: \nPress m for Machine \nPress d for Dispatch \nPress o for Orders");
		}
	}
	/* SELECT time(200, 'unixepoch')
	SELECT time(strftime('%s','now', 'localtime') - strftime('%s','11:10:56'), 'unixepoch'), datetime('now', 'localtime')
	SELECT Order_Number As ID, Type, State , Total_Number_Pieces AS Produced, time(strftime('%s', Deadline) - strftime('%s', End_Time), 'unixepoch') 
	AS Remaining FROM ORDERS Where State = 'Finished' AND (Type = 'Dispatch' OR Type = 'Transformation');

	SELECT Order_Number As ID, Type, State , Total_Number_Pieces AS Produced, t1.cont FROM ORDERS t
   JOIN (SELECT COUNT(ID) as cont, ID_Order FROM Piece Group BY ID_Order) t1 ON t1.ID_Order =  t.Order_Number
   Where State = 'Executing' AND (Type = 'Dispatch' OR Type = 'Transformation')

	time_t rawtime, rawtime1;
	struct tm timeinfo, timeinfo1;

	timeinfo.tm_year = 2016;
	timeinfo.tm_mon = 03;
	timeinfo.tm_mday = 02;
	timeinfo.tm_hour = 10;
	timeinfo.tm_min = 17;
	timeinfo.tm_sec = 10;
	rawtime = mktime(&timeinfo);


	timeinfo1.tm_year = 2016;
	timeinfo1.tm_mon = 03;
	timeinfo1.tm_mday = 02;
	timeinfo1.tm_hour = 10;
	timeinfo1.tm_min = 18;
	timeinfo1.tm_sec = 15;
	rawtime1 = mktime(&timeinfo1);

	std::cout << rawtime1 << std::endl;
	std::cout << "Diff: " << difftime(rawtime1, rawtime)<<std::endl;
	*/
	return 0;
}
int printStatisticsOrderFinished(const char* s, TextTable *t)
{
	sqlite3* DB;
	sqlite3_open(s, &DB);
	t->add("Order Number");
	t->add("Type");
	t->add("State");
	t->add("Produced");
	t->add("Entry Time");
	t->add("Execution Start");
	t->add("End Time");
	t->add("Remaining Time");
	t->endOfRow();
	std::string sql= ("SELECT ID, Type, State, Produced, Entry_Time, Execution_Start, End_Time, "\
		"CASE "\
		"WHEN(falta) < 0 THEN '-' || time(abs(falta), 'unixepoch') "\
		"ELSE time(abs(falta), 'unixepoch') "\
		"END AS Remaining_Time "\
		"FROM "\
		" (SELECT Order_Number As ID, Type, State, Total_Number_Pieces AS Produced, strftime('%s', Deadline), strftime('%s', End_Time) "\
		" AS Remaining, End_Time, Execution_Start, Entry_Time, Deadline, strftime('%s', Deadline) - strftime('%s', End_Time) as falta"\
		" FROM ORDERS Where State = 'Finished' AND(Type = 'Dispatch' OR Type = 'Transformation')) ORDER BY ID ASC;");
	sqlite3_exec(DB, sql.c_str(), callback3, t, NULL);
	sqlite3_close(DB);
	return 0;
}
int callback3(void* rt, int argc, char** argv, char** azColName)
{
	TextTable* c = (TextTable*)(rt);
	for (int i = 0; i < argc; i++) {
		//column name and valu
		if (argv[i] == NULL)
			c->add(" ");
		else
		{
			c->add(argv[i]);
		}
	}
	c->endOfRow();
	return 0;
}
int printStatisticsOrderWait(const char* s, TextTable* t)
{
	sqlite3* DB;
	sqlite3_open(s, &DB);
	t->add("Order Number");
	t->add("Type");
	t->add("State");
	t->add("Waiting");
	t->add("Entry Time");
	t->add("Remaining Time");
	t->endOfRow();
	std::string sql = ("SELECT ID, Type, State, Total_Number_Pieces AS Waiting, Entry_Time, "\
		"CASE "\
		"WHEN(falta) < 0 THEN '-' || time(abs(falta), 'unixepoch') "\
		"ELSE time(abs(falta), 'unixepoch') "\
		"END AS Remaining_Time "\
		"FROM(SELECT Order_Number As ID, Type, State, Total_Number_Pieces, Entry_Time, Execution_Start, strftime('%s', Deadline) - strftime('%s', datetime('now', 'localtime')) as falta "\
		"FROM ORDERS Where(State = 'Waiting') AND(Type = 'Dispatch' OR Type = 'Transformation')) t;); ");
	sqlite3_exec(DB, sql.c_str(), callback2, t, NULL);
	return 0;
}
int printStatisticsOrderExecuting(const char* s, TextTable* t)
{
	sqlite3* DB;
	sqlite3_open(s, &DB);
	t->add("Order Number");
	t->add("Type");
	t->add("State");
	t->add("Executing");
	t->add("Produced");
	t->add("Waiting");
	t->add("Entry Time");
	t->add("Execution Start");
	t->add("Remaining Time");
	t->endOfRow();
	std::string sql = ("SELECT Order_ID, Type, State, Case When t1.cont IS NULL Then 0 ELSE t1.cont END AS Executing, Case When t2.cont IS NULL Then 0 ELSE t2.cont END AS Produced, (Total_Number_Pieces - (Case When t1.cont IS NULL Then 0 ELSE t1.cont END) - (Case When t2.cont IS NULL Then 0 ELSE t2.cont END)) AS Waiting, Entry_Time, Execution_Start, "\
		"CASE "\
		"WHEN(falta) < 0 THEN '-' || time(abs(falta), 'unixepoch') "\
		"ELSE time(abs(falta), 'unixepoch') "\
		"END AS Remaining_Time "\
		"FROM(SELECT Order_Number As Order_ID, ID, Type, State, Total_Number_Pieces, Entry_Time, Execution_Start, strftime('%s', Deadline) - strftime('%s', datetime('now', 'localtime')) as falta "\
		"FROM ORDERS Where(State = 'Executing') AND(Type = 'Dispatch' OR Type = 'Transformation')) t "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece "\
		"Where Execution_End is NULL Group By Id_Order) t1 ON t1.ID_Order = t.ID "\
		"LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece Where Execution_End is NOT NULL Group By Id_Order) t2 ON t2.ID_Order = t.ID "\
		"Where State = 'Executing' AND(Type = 'Dispatch' OR Type = 'Transformation') ORDER BY ID ASC;); ");
	sqlite3_exec(DB, sql.c_str(), callback2, t, NULL);
	sqlite3_close(DB);
	return 0;
}
int callback2(void* t, int argc, char** argv, char** azColName)
{
	TextTable* c = (TextTable*)(t);
	for (int i = 0; i < argc; i++) {
		//column name and valu
		if (argv[i] == NULL)
		{
			if (!strcmp(azColName[i], "Remaining_Time"))
				c->add(" ");
			else
				c->add("0");
		}
		else
			c->add(argv[i]);
	}
	c->endOfRow();
	return 0;
}
int callback(void* t, int argc, char** argv, char** azColName)
{
	TextTable* c = (TextTable*)(t);
	for (int i = 0; i < argc; i++) {
		//column name and value
		c->add(argv[i]);
		//std::cout << argv[i] << std::endl;
	}
	c->endOfRow();
	return 0;
}
int printStatisticsDisptach(const char* s, TextTable *t)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	t->add("Zone");
	t->add("Total Pieces");
	t->add("P1");
	t->add("P2");
	t->add("P3");
	t->add("P4");
	t->add("P5");
	t->add("P6");
	t->add("P7");
	t->add("P8");
	t->add("P9");
	t->endOfRow();
	std::string sql = (" SELECT t.Zone, SUM(Quantity) AS TotalPieces, t1.P1, t2.P2, t3.P3, t4.P4, t5.P5, t6.P6, t7.P7, t8.P8, t9.P9 FROM Dispatch t "\
		"INNER JOIN " \
		"(SELECT Quantity as P1, Zone FROM Dispatch WHERE PieceType = 'P1') t1 ON t1.Zone = t.Zone "\
		"INNER JOIN "
		" (SELECT Quantity as P2, Zone FROM Dispatch WHERE PieceType = 'P2') t2 ON t2.Zone = t.Zone "\
		"INNER JOIN "
		"(SELECT Quantity as P3, Zone FROM Dispatch WHERE PieceType = 'P3') t3 ON t3.Zone = t.Zone "\
		"INNER JOIN " \
		"(SELECT Quantity as P4, Zone FROM Dispatch WHERE PieceType = 'P4') t4 ON t4.Zone = t.Zone "\
		"INNER JOIN "\
		"(SELECT Quantity as P5, Zone FROM Dispatch WHERE PieceType = 'P5') t5 ON t5.Zone = t.Zone "\
		"INNER JOIN "\
		"(SELECT Quantity as P6, Zone FROM Dispatch WHERE PieceType = 'P6') t6 ON t6.Zone = t.Zone "\
		"INNER JOIN "\
		"(SELECT Quantity as P7, Zone FROM Dispatch WHERE PieceType = 'P7') t7 ON t7.Zone = t.Zone "\
		"INNER JOIN "\
		"(SELECT Quantity as P8, Zone FROM Dispatch WHERE PieceType = 'P8') t8 ON t8.Zone = t.Zone "\
		"INNER JOIN "\
		"(SELECT Quantity as P9, Zone FROM Dispatch WHERE PieceType = 'P9') t9 ON t9.Zone = t.Zone "\
		"Group BY t.Zone "\
		);
	exit = sqlite3_exec(DB, sql.c_str(), callback, t, NULL);
	sqlite3_close(DB);
	return 0;
}
int printStatisticsMachine(const char* s, TextTable *t)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	t->add("");
	t->add("Total Pieces");
	t->add("Execution");
	t->add("P1");
	t->add("P2");
	t->add("P6");
	t->endOfRow();
	std::string sql = ("SELECT t.MachineType, SUM(Quantity) AS TotalPieces, SUM(ProductionTime) AS TotalTime, t1.P1, t2.P2, t3.P6 FROM Machine t "\
		"INNER JOIN " \
		"(SELECT Quantity as P1, MachineType FROM Machine WHERE PieceType = 'P1') t1 ON t1.MachineType = t.MachineType "\
		"INNER JOIN "
		"(SELECT Quantity as P2, MachineType FROM Machine WHERE PieceType = 'P2') t2 ON t2.MachineType = t.MachineType "\
		"INNER JOIN " \
		"(SELECT Quantity as P6, MachineType FROM Machine WHERE PieceType = 'P6') t3 ON t3.MachineType = t.MachineType " \
		"Group BY t.MachineType");
	exit = sqlite3_exec(DB, sql.c_str(), callback, t, NULL);
	t->endOfRow();
	t->add("");
	t->add("Total Pieces");
	t->add("Execution");
	t->add("P1");
	t->add("P3");
	t->add("P7");
	t->endOfRow();
	sql = ("SELECT t.MachineType, SUM(Quantity) AS TotalPieces, SUM(ProductionTime) AS TotalTime, t1.P1, t2.P3, t3.P7 FROM Machine t "\
		"INNER JOIN " \
		"(SELECT Quantity as P1, MachineType FROM Machine WHERE PieceType = 'P1') t1 ON t1.MachineType = t.MachineType "\
		"INNER JOIN "
		"(SELECT Quantity as P3, MachineType FROM Machine WHERE PieceType = 'P3') t2 ON t2.MachineType = t.MachineType "\
		"INNER JOIN " \
		"(SELECT Quantity as P7, MachineType FROM Machine WHERE PieceType = 'P7') t3 ON t3.MachineType = t.MachineType " \
		"Group BY t.MachineType");
	exit = sqlite3_exec(DB, sql.c_str(), callback, t, NULL);
	t->endOfRow();
	t->add("");
	t->add("Total Pieces");
	t->add("Execution");
	t->add("P1");
	t->add("P4");
	t->add("P8");
	t->endOfRow();
	sql = ("SELECT t.MachineType, SUM(Quantity) AS TotalPieces, SUM(ProductionTime) AS TotalTime, t1.P1, t2.P4, t3.P8 FROM Machine t "\
		"INNER JOIN " \
		"(SELECT Quantity as P1, MachineType FROM Machine WHERE PieceType = 'P1') t1 ON t1.MachineType = t.MachineType "\
		"INNER JOIN "
		"(SELECT Quantity as P4, MachineType FROM Machine WHERE PieceType = 'P4') t2 ON t2.MachineType = t.MachineType "\
		"INNER JOIN " \
		"(SELECT Quantity as P8, MachineType FROM Machine WHERE PieceType = 'P8') t3 ON t3.MachineType = t.MachineType " \
		"Group BY t.MachineType");
	exit = sqlite3_exec(DB, sql.c_str(), callback, t, NULL);
	sqlite3_close(DB);
	return 0;
}
/* 
Executing 

SELECT ID, Type, State , t1.cont AS Executing, t2.cont AS Produced, (Total_Number_Pieces - t1.cont - t2.cont) AS Waiting, Entry_Time, Execution_Start,
						CASE
		WHEN(falta) < 0 THEN '-' || time(abs(falta), 'unixepoch')
		ELSE time(abs(falta), 'unixepoch')
		END AS Remaining_Time
		FROM  (SELECT Order_Number As ID, Type, State, Total_Number_Pieces, Entry_Time, Execution_Start, strftime('%s', Deadline) - strftime('%s', datetime('now', 'localtime')) as falta
		 FROM ORDERS Where (State = 'Executing') AND(Type = 'Dispatch' OR Type = 'Transformation')) t

					   LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece
		Where Execution_End is NULL Group By Id_Order) t1 ON t1.ID_Order = t.ID
		LEFT JOIN(SELECT COUNT(Distinct ID) as cont, ID_Order, Execution_END FROM Piece Where Execution_End is NOT NULL Group By Id_Order) t2 ON t2.ID_Order = t.ID
		Where State = 'Executing' AND(Type = 'Dispatch' OR Type = 'Transformation') ORDER BY ID ASC;

Waiting
SELECT ID, Type, State , Total_Number_Pieces AS Waiting, Entry_Time,
		 CASE
		WHEN(falta) < 0 THEN '-' || time(abs(falta), 'unixepoch')
		ELSE time(abs(falta), 'unixepoch')
		END AS Remaining_Time
		FROM  (SELECT Order_Number As ID, Type, State, Total_Number_Pieces, Entry_Time, Execution_Start, strftime('%s', Deadline) - strftime('%s', datetime('now', 'localtime')) as falta
		 FROM ORDERS Where (State = 'Waiting') AND(Type = 'Dispatch' OR Type = 'Transformation')) t ;

*/