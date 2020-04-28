#include <stdlib.h>
#include <stdio.h>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

int main (int argc, char const *argv[]) {

    OrderQueue OrdrQueue;

    XMLParser XMLParser;

    UDPManager UDPManager(54321);
    std::thread udp_worker = UDPManager.spawn_worker(XMLParser.parseString);

    // algures aqui teria de se retirar a informação obtida por XML
    Order::BaseOrder order_from_xml(1, 4, 1);
    
    OrdrQueue.AddOrder(order_from_xml);

    FILE* f = fopen("opc-ua-id.txt", "r");
    if (!f) {
        meslog(ERROR) << "Couldn't open opc-ua-id.txt" << std::endl;
    }
    char OpcUa_id[100] = {0};
    int result = fread (OpcUa_id,1,100,f);

    OPCUA_Manager myManager("opc.tcp://127.0.0.1:4840", OpcUa_id, 4);

    if (myManager.Is_Connected()) {
        myManager.SendPieceOPC_UA(OrdrQueue.GetNextOrder());
    }
    else {
        meslog(ERROR) << "!!!Failed to Connect to OPC-UA Master!!!" << std::endl;
    }

    const char* dir = "factory.db"; // Definir path da DB
	checkDB(dir);
	createDB(dir);
	createTable(dir);
	deleteData(dir);
	int num_Order = 4;
	std::string Type = "Transformation";
	std::string State = "Waiting";
	char Initial_Piece[3] = "P1";
	char Final_Piece[3] = "P5";
	int Total_Pieces = 40;
	int Deadline = 400;
	std::string NewState = "Executing";
	insertDataOrder(dir, num_Order, Type, State, Initial_Piece, Final_Piece, Total_Pieces, Deadline, getDateTime());
	insertDataPiece(dir, getOrder_ID(dir, Type, 4), 4, getDateTime());
	updateData(dir, NewState, getOrder_ID(dir, Type, num_Order), getDateTime());

    // Wait for threads to close
    udp_worker.join();
    return 0;
}
