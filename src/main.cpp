#include <stdlib.h>
#include <stdio.h>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
//#include "OPC-UA.cpp"
#include "DBInterface.hpp"

#include "helpers.h"

int main (int argc, char const *argv[]) {

    XMLParser XMLParser;

    UDPManager UDPManager(54321);
    std::thread udp_worker = UDPManager.spawn_worker(XMLParser.parseString);

    FILE* f = fopen("opc-ua-id.txt", "r");
    if (!f) {
        log(ERROR) << "Couldn't open opc-ua-id.txt" << std::endl;
    }
    char OpcUa_id[100] = {0};
    int result = fread (OpcUa_id,1,100,f);

    /*OPCUA_Manager myManager("opc.tcp://127.0.0.1:4840", OpcUa_id, 4);

    if (myManager.Is_Connected()) {

        piece_object peca;
        peca.id_piece = 1;
        peca.transformation = 4;
        peca.type_piece = 6;
        for (uint16_t i = 0; i < 59; i++) {
            peca.path[i] = 0;
        }
        peca.path[0] = RIGHT;
        peca.path[1] = RIGHT;
        peca.path[2] = DOWN;
        peca.path[3] = DOWN;
        peca.path[4] = RIGHT;
        peca.path[5] = LEFT;
        peca.path[6] = DOWN;
        peca.path[7] = DOWN;
        peca.path[8] = DOWN;
        peca.path[9] = DOWN;
        peca.path[10] = LEFT;
        peca.path[11] = LEFT;

        myManager.SendPieceOPC_UA(peca.path, peca.transformation, peca.id_piece, peca.type_piece, 1);
        std::cout << "Press ENTER to send next piece..." << std::endl;
        std::string aux;

        peca.path[0] = RIGHT;
        peca.path[1] = RIGHT;
        peca.path[2] = DOWN;
        peca.path[3] = DOWN;
        peca.path[4] = LEFT;
        peca.path[5] = RIGHT;
        peca.path[6] = DOWN;
        peca.path[7] = DOWN;
        peca.path[8] = DOWN;
        peca.path[9] = DOWN;
        peca.path[10] = LEFT;
        peca.path[11] = LEFT;
        std::getline(std::cin, aux);
        myManager.SendPieceOPC_UA(peca.path, peca.transformation, peca.id_piece, peca.type_piece, 1);
    }
    else {
        std::cout << "!!!Failed to Connect to OPC-UA Master!!!" << std::endl;
    }*/

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
