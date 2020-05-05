#include <stdlib.h>
#include <stdio.h>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

int main (int argc, char const *argv[]) {

    OrderQueue OrderQueue;

    XMLParser XMLParser(&OrderQueue);

    UDPManager UDPManager(54321);
    std::thread udp_worker = UDPManager.spawn_worker(&XMLParser);

    FILE* f = fopen("opc-ua-id.txt", "r");
    if (!f) {
        meslog(ERROR) << "Couldn't open opc-ua-id.txt" << std::endl;
    }
    char OpcUa_id[100] = {0};
    int result = fread (OpcUa_id,1,100,f);

    OPCUA_Manager myManager("opc.tcp://127.0.0.1:4840", OpcUa_id, 4);

    if (myManager.Is_Connected()) {
        myManager.SendPieceOPC_UA(OrderQueue.GetNextOrder());
    }
    else {
        meslog(ERROR) << "!!!Failed to Connect to OPC-UA Master!!!" << std::endl;
    }

    const char* dir = "factory.db"; // Definir path da DB
	/*checkDB(dir);
	createDB(dir);
	createTable(dir); // fazer isto na primeira vez para criar a base de dados
    initvalues(dir);*/
    // Wait for threads to close
    udp_worker.join();
    return 0;
}
