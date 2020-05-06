#include <stdlib.h>
#include <stdio.h>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

int main (int argc, char const *argv[]) {

    OrderQueue *order_queue = new OrderQueue();
    Warehouse *warehouse = new Warehouse();

    XMLParser XMLParser(&OrderQueue);

    UDPManager UDPManager(54321);
<<<<<<< HEAD
    std::thread udp_worker = UDPManager.spawn_worker(XMLParser.parseString);

    // algures aqui teria de se retirar a informação obtida por XML
    time_t now;
    localtime(&now);
    Order::BaseOrder order_from_xml(1, Order::ORDER_TYPE_TRANSFORMATON, 1, 1, 2, now);
    
    order_queue->AddOrder(order_from_xml);
=======
    std::thread udp_worker = UDPManager.spawn_worker(&XMLParser);
>>>>>>> f91193dd2ded03ed76bbe75c9cc4124b622bffaf

    FILE* f = fopen("opc-ua-id.txt", "r");
    if (!f) {
        meslog(ERROR) << "Couldn't open opc-ua-id.txt" << std::endl;
    }
    char OpcUa_id[100] = {0};
    int result = fread (OpcUa_id,1,100,f);

    OPCUA_Manager myManager("opc.tcp://127.0.0.1:4840", OpcUa_id, 4, order_queue, warehouse);

    if (myManager.Is_Connected()) {
        myManager.SendPieceOPC_UA(order_queue->GetNextOrder());
        myManager.CheckIncomingPieces();
    }
    else {
        meslog(ERROR) << "!!!Failed to Connect to OPC-UA Master!!!" << std::endl;
    }

    const char* dir = "factory.db"; // Definir path da DB
	checkDB(dir);
	createDB(dir);
	createTable(dir); // fazer isto na primeira vez para criar a base de dados
    initvalues(dir);
    // Wait for threads to close
    udp_worker.join();
    return 0;
}
