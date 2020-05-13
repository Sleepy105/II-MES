#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "UDPManager.hpp"
#include "XMLParser.hpp"
#include "OPC-UA.hpp"
#include "DBInterface.hpp"
#include "OrderQueue.hpp"

#include "helpers.h"

int main (int argc, char const *argv[]) {

    OrderQueue *order_queue = new OrderQueue();
    Warehouse *warehouse = new Warehouse();

    UDPManager UDPManager(54321);
    XMLParser XMLParser(order_queue, &UDPManager);
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

    OPCUA_Manager opc_ua("opc.tcp://127.0.0.1:4840", OpcUa_id, 4, order_queue, warehouse);


    // Inserir algumas orders na queue. Assim que se queira testar as orders a chegar por UDP apaga-se isto
    order_queue->AddOrder(Order::BaseOrder(1, Order::ORDER_TYPE_TRANSFORMATION, 10, 1, 2, 200));
    order_queue->AddOrder(Order::BaseOrder(2, Order::ORDER_TYPE_TRANSFORMATION, 10, 1, 3, 400));
    order_queue->AddOrder(Order::BaseOrder(3, Order::ORDER_TYPE_UNLOAD, 10, 1, 1, "doesn't matter"));
    order_queue->print();
    // ideia: usar o final_piece da order como tapete de destino no caso de ser do tipo unload (visto que final piece nao e usado nesse caso)
    

    // Setup de variaveis para o ciclo de controlo principal
    Order::BaseOrder *next_order;

    //Ciclo de Controlo Principal (threadless, com a excessao do UDPManager)
    // while (1){
        if (!opc_ua.Is_Connected()){
            meslog(ERROR) << "Couldn't Connect to OPC-UA Master" << std::endl;
            // break;
        }


    //     // envia peca das orders de load e transformation
    //     try{
    //         if (opc_ua.warehouseOutCarpetIsFree()){
    //             next_order = order_queue->GetNextOrder();
    //             if (!(opc_ua.SendPieceOPC_UA(*next_order))){
    //                 next_order->DecreaseCount();
    //             }
    //         }
    //     }
    //     catch(const char *msg){
    //         meslog(INFO) << "No orders to execute/all orders are unexecutable" << std::endl;
    //     }
    //     // verifica se recebeu pecas
    //     if (opc_ua.CheckIncomingPieces()){
    //         meslog(INFO) << "Incoming piece in factory floor" << std::endl;
    //     }
    //     // verifica se chegaram pecas a warehouse
    //     if (opc_ua.CheckPiecesFinished()){
    //         meslog(INFO) << "Piece(s) finished in factory floor" << std::endl;
    //     }
    // }

    // next_order = new Order::BaseOrder(55, Order::ORDER_TYPE_TRANSFORMATION, 10, 1, 2, 200);
    // Order::Piece *new_piece = new Order::Piece(666);
    // uint8_t my_path[59] = {1, 1, 2, 2, 1, 3, 2, 2, 2, 2, 3, 3, 0};
    // new_piece->SetPath(my_path);
    // new_piece->print();
    // next_order->GetPieces().push_back(new_piece);
    // Order::Piece *yet_another_piece = next_order->GetPieces().front();
    // yet_another_piece->print();
    // opc_ua.SendPieceOPC_UA(*next_order);


    // Wait for threads to close
    udp_worker.join();
    return 0;
}
