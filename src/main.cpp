#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

#define CYCLE_DELAY_IN_MILLISECONDS 200

int main (int argc, char const *argv[]) {

    OrderQueue order_queue;
    Warehouse warehouse;

    UDPManager UDPManager(54321);
    XMLParser XMLParser(&order_queue, &UDPManager);
    // Iniciar thread para UDP
    std::thread udp_worker = UDPManager.spawn_worker(&XMLParser);


    // Iniciar Base de Dados
    const char* dir = "factory.db"; // Definir path da DB
	checkDB(dir);
	createDB(dir);
    deleteData(dir);
	createTable(dir); // fazer isto na primeira vez para criar a base de dados
    initvalues(dir);

    // Configurar OPC-UA
    FILE* f = fopen("opc-ua-id.txt", "r");
    char OpcUa_id[100] = {0};
    if (!f) {
        meslog(ERROR) << "Couldn't open opc-ua-id.txt, using default ID..." << std::endl;
        strcpy(OpcUa_id, "|var|CODESYS Control Win V3 x64.Application.");
    }else {
        int result = fread (OpcUa_id,1,100,f);
    }

    OPCUA_Manager opc_ua("opc.tcp://127.0.0.1:4840", OpcUa_id, 4, &order_queue, &warehouse);


    // Inserir algumas orders na queue. Assim que se queira testar as orders a chegar por UDP apaga-se isto
    order_queue.AddOrder(Order::BaseOrder(1, Order::ORDER_TYPE_TRANSFORMATION, 1, 2, 6, 300));
    order_queue.AddOrder(Order::BaseOrder(2, Order::ORDER_TYPE_TRANSFORMATION, 2, 1, 9, 200));
    order_queue.AddOrder(Order::BaseOrder(3, Order::ORDER_TYPE_UNLOAD, 2, 1, 1, "doesn't matter"));

    // order_queue.print();
    // ideia: usar o final_piece da order como tapete de destino no caso de ser do tipo unload (visto que final piece nao e usado nesse caso)
    

    // Setup de variaveis para o ciclo de controlo principal
    Order::BaseOrder *next_order;
    bool order_buffered = false;

    if (!opc_ua.Is_Connected()){
        meslog(ERROR) << "Couldn't connect to OPC-UA Master, waitting for connection..." << std::endl;
        while (!opc_ua.Is_Connected()){
            opc_ua.Reconnect();
        }
        meslog(INFO) << "Connection established." << std::endl;
    }
    //Ciclo de Controlo Principal (threadless, com a excessao do UDPManager)
    while (opc_ua.Is_Connected()){

        //envia peca das orders de load e transformation
        try{
            if (opc_ua.warehouseOutCarpetIsFree()){
                next_order = order_queue.GetNextOrder();
                opc_ua.SendPieceOPC_UA(next_order);
                next_order->DecreaseCount();
                meslog(INFO) << "Current count for order " << next_order->GetID() << ": "<< next_order->GetCount() << std::endl;
                order_queue.print();
            }
        }catch(const char *msg){
            
        }
        //verifica se recebeu pecas
        if (opc_ua.CheckIncomingPieces()){
            meslog(INFO) << "Incoming piece in factory floor" << std::endl;
        }
        // verifica se chegaram pecas a warehouse
        if (opc_ua.CheckPiecesFinished()){
            meslog(INFO) << "Piece(s) finished in factory floor" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(CYCLE_DELAY_IN_MILLISECONDS*1000000)); // 0,2 s
    } meslog(ERROR) << "Disconnected from OPC-UA Master" << std::endl;


    // Wait for threads to close
    udp_worker.join();
    return 0;
}
/*
    Como dar restore ao armazem e as ordens por parte da DB
    
    int warehouse_quantity[9];
    getWarehouseInformation(dir, warehouse_quantity);

    Load_Unload RestoreOrders =	RestoreMeshOrders(dir);
*/