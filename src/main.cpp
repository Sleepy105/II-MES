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
    UDPManager UDPManager(54321);
    OrderQueue order_queue(&warehouse, NULL);

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
    initvalues(dir); // tbm tem de ser usado este para quando se faz delete

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
    order_queue.setOPCpointer(&opc_ua);

    // Setup de variaveis para o ciclo de controlo principal
    Order::BaseOrder *next_order;
    bool order_buffered = false;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    long long nano_duration;
    long long wait_time;
    //Ciclo de Controlo Principal (threadless, com a excepcao do UDPManager)
    while (true){
        begin = std::chrono::steady_clock::now();

        while(!opc_ua.Reconnect()){ // if no connection, reconnect (Reconnect() does both the checking and the reconnecting)
            meslog(ERROR) << "Couldn't connect to OPC-UA Master, waitting for connection..." << std::endl;
            std::this_thread::sleep_for(std::chrono::nanoseconds(CYCLE_DELAY_IN_MILLISECONDS/10*1000000)); // 0,1 s
        }
        order_queue.print();

        opc_ua.UpdateAll();

        //envia peca das orders de load e transformation
        try{
            if (opc_ua.warehouseOutCarpetIsFree()){
                next_order = order_queue.GetNextOrder(); // this generates an exception if no orders found (will jump to "catch()", below)

                // This code will only run if the previous call to GetNextOrder() didn't generate an exception:
                if(opc_ua.SendPiece(next_order)){

                    next_order->DecreaseCount();
                    meslog(INFO) << "Piece Sent. Current count for order " << next_order->GetID() << ": "<< next_order->GetCount() << "." << std::endl;
                }
            }
        }catch(const char *msg){
            // Run this if no orders were found. msg already holds the error message, in case we want to display it, like so: 
            // meslog(ERROR) << msg << std::endl;

        }

        end = std::chrono::steady_clock::now();
        nano_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
        wait_time = CYCLE_DELAY_IN_MILLISECONDS*1000000 - nano_duration;
        if (wait_time > 0){
            std::this_thread::sleep_for(std::chrono::nanoseconds(wait_time)); // 1 s
        }
    }


    // Wait for threads to close
    udp_worker.join();
    return 0;
}
