#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

#define CYCLE_DELAY_IN_MILLISECONDS 1000

int main (int argc, char const *argv[]) {

    Warehouse warehouse;
    OrderQueue order_queue(&warehouse);

    UDPManager UDPManager(54321);
    XMLParser XMLParser(&order_queue, &UDPManager, &warehouse);
    // Iniciar thread para UDP
    std::thread udp_worker = UDPManager.spawn_worker(&XMLParser);


    // Iniciar Base de Dados
    const char* dir = DBFILE; // Definir path da DB
	int check = checkDB(dir);
    if(!check) {
        createDB(dir);
        createTable(dir); // fazer isto na primeira vez para criar a base de dados
        initvalues(dir);
    }

    if(false) {

        // Restore da informação presente no armazem
        int warehouse_quantity[9];
        getWarehouseInformation(dir, warehouse_quantity);

        warehouse.RestoreStatus(warehouse_quantity);

        Load_Unload RestoreOrders = RestoreMeshOrders(dir);
        // Restore das ordens de Load e Unload
        for(int i = 0; i < RestoreOrders.vectorPositionDispatchIncoming; i++) {
            order_queue.RestoreLoadUnload(RestoreOrders.RestoreDispatch_Incoming[i]);
        }

        // Restore das ordens de Transformacao
        for(int i = 0; i < RestoreOrders.vectorPositionTransformation; i++) {
            order_queue.RestoreTrans(RestoreOrders.RestoreTransformation[i]);
        }
        order_queue.print();
    }

    deleteData(dir); // usado so para teste

    // Configurar OPC-UA
    std::string OpcUa_id = "|var|CODESYS Control Win V3 x64.Application.",
        OpcUa_conn = "opc.tcp://127.0.0.1:4840";
    std::ifstream infile("opc-ua-conf.txt");
    if (infile) {
        std::getline(infile, OpcUa_id);
        std::getline(infile, OpcUa_conn);
    }
    else {
        meslog(ERROR) << "Couldn't open opc-ua-conf.txt, using default ID, Protocol, IP Address and Port......" << std::endl;
    }
    
    OPCUA_Manager opc_ua(OpcUa_conn.c_str(), OpcUa_id.c_str(), &order_queue, &warehouse);

    // Setup de variaveis para o ciclo de controlo principal
    Order::BaseOrder *next_order;
    bool order_buffered = false;

    //Ciclo de Controlo Principal (threadless, com a excepcao do UDPManager)
    while (true){
        while(!opc_ua.Reconnect()){ // if no connection, reconnect (Reconnect() does both the checking and the reconnecting)
            meslog(ERROR) << "Couldn't connect to OPC-UA Master, waitting for connection..." << std::endl;
            std::this_thread::sleep_for(std::chrono::nanoseconds(CYCLE_DELAY_IN_MILLISECONDS/10*1000000)); // 0,1 s
        }
        meslog(INFO) << "Running cycle..." << std::endl;

        //envia peca das orders de load e transformation
        try{
            if (opc_ua.warehouseOutCarpetIsFree()){
                next_order = order_queue.GetNextOrder(); // this generates an exception if no orders found (will jump to "catch()", below)
                // This code will only run if the previous call to GetNextOrder() didn't generate an exception:
                opc_ua.SendPieceOPC_UA(next_order);
                next_order->DecreaseCount();
                meslog(INFO) << "Piece Sent. Current count for order " << next_order->GetID() << ": "<< next_order->GetCount() << std::endl;
            }
        }catch(const char *msg){
            // Run this if no orders were found. msg already holds the error message, in case we want to display it, like so: "meslog(ERROR) << msg << std::endl"

        }
        //verifica se recebeu pecas
        if (opc_ua.CheckIncomingPieces()){
            meslog(INFO) << "Incoming piece in factory floor" << std::endl;
        }
        // verifica se chegaram pecas a warehouse
        if (opc_ua.CheckPiecesFinished()){
            meslog(INFO) << "Piece(s) finished in factory floor" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(CYCLE_DELAY_IN_MILLISECONDS*1000000)); // 1 s
    } meslog(ERROR) << "Disconnected from OPC-UA Master" << std::endl;


    // Wait for threads to close
    udp_worker.join();
    return 0;
}
