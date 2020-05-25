/**
 * @file OPC-UA.hpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _OPC_UA_HPP_
#define _OPC_UA_HPP_

//#define DEBUG_UA
#define ASCII_NUMERAL_OFFSET 48

#define RIGHT   1
#define DOWN    2
#define LEFT    3
#define UP      4
#define PLACEHOLDER 0

#include "open62541/plugins/include/open62541/client_config_default.h"
#include "open62541/include/open62541/client_highlevel.h"
#include "open62541/include/open62541/client_subscriptions.h"
#include "open62541/plugins/include/open62541/plugin/log_stdout.h"
#include <string.h>
#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <iterator>
#include "Order.hpp"
#include "OrderQueue.hpp"
#include "PathFinder.hpp"
#include "Warehouse.hpp"

class OPCUA_Manager {
private:
    UA_Client *client_;
    const char* BaseNodeID_;
    char URL_[128];
    int16_t nodeIndex_;
    bool connected_;
    std::queue <Order::Piece> pusher_queue[3]; // Estas Queues sao Read-Only, alterar estas Pieces nao altera a versao original da Piece!
    uint16_t pusher_queue_size[3]; // Isto NAO sao quantas pecas estao alocadas, mas sim fisicamente presentes no slider
    uint8_t machine_tools_in_use[3][3];
    uint16_t piece_id_being_processed[3][3];
    uint16_t last_piece_id_processed[3][3];

    OrderQueue *order_queue;
    Warehouse *warehouse;


    UA_Client* ServerConnect(const char* endpointURL) const;
    void ConvIntToString(char* string, uint16_t value);

public:
    OPCUA_Manager(const char* URL, const char* BaseID, OrderQueue *order_queue_reference, Warehouse *warehouse_reference);
    OPCUA_Manager(const char* URL, const char* BaseID, OrderQueue *order_queue_reference, Warehouse *warehouse_reference, uint16_t index);

    bool Is_Connected();
    
    bool Reconnect();

    bool SendPiece (Order::BaseOrder *order);

    bool CheckPiecesFinished();

    bool CheckIncomingPieces();

    bool warehouseOutCarpetIsFree();

    bool CheckOutgoingPieces();
    
    void UpdateMachineInfo();

    void UpdatePiecesProcessedInMachines();

    /**
     * @brief Get piece allocation for specific pusher
     * 
     * @param pusher_number pusher identifier, from which the piece allocation will be retrieved and
     * should be either 1,2 or 3.
     * @return number of allocated pieces for specified pusher. Returns 0 if it fails
     */
    unsigned int GetPieceAllocInPusher(uint8_t pusher_number);
    /**
     * @brief Get current tool for specific machine from specific cell
     * 
     * @param machine_type identifier for machine type, from which the currently equipped tool will be retrieved and
     * should be either 1,2 or 3, for A,B and C respectively.
     * @param cell_number identifier for cell, from which the currently equipped tool will be retrieved and
     * should be either 1,2 or 3.
     * @return number of currently equipped tool (1,2 or 3). There's no "fail" return;
     */
    unsigned int GetCurrentToolInMachine(uint8_t machine_type, uint8_t cell_number);

    /**
     * @brief Get current piece id at top carpet (the one for pieces on hold)
     * 
     * @param cell_number identifier for cell, from which the currently equipped tool will be retrieved and
     * should be either 1,2 or 3.
     * @return id of piece that is currently present at specified cell_number's holding carpet;
     */
    uint16_t GetTopCarpetPieceID(uint8_t cell_number);

    /**
     * @brief Get id of piece that was last transformed in a given machine (not the currently present piece)
     * 
     * @param machine_type identifier for machine type, from which the last completed piece id will be retrieved and
     * should be either 1,2 or 3, for A,B and C respectively.
     * @param cell_number identifier for cell, from which the last completed piece id will be retrieved and
     * should be either 1,2 or 3.
     * @return id of piece that was last processed in a given machine;
     */
    uint16_t GetLastMadePieceIDInMachine(uint8_t machine_type, uint8_t cell_number);

    /**
     * @brief Get id either of current piece being processed or last piece, in case there's no piece present.
     * 
     * @param machine_type identifier for machine type, from which the last completed piece id will be retrieved and
     * should be either 1,2 or 3, for A,B and C respectively.
     * @param cell_number identifier for cell, from which the last completed piece id will be retrieved and
     * should be either 1,2 or 3.
     * @return id of piece that is currently being processed in a given machine;
     */
    uint16_t GetCurrentPieceIDInMachine(uint8_t machine_type, uint8_t cell_number);
};

#endif
