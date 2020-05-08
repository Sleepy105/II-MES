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

    OrderQueue *order_queue;
    Warehouse *warehouse;


    UA_Client* ServerConnect(const char* endpointURL) const;
    void ConvIntToString(char* string, uint16_t value);

public:
    OPCUA_Manager(const char* URL, const char* BaseID, int16_t index, OrderQueue *order_queue_reference, Warehouse *warehouse_reference);

    bool Is_Connected();

    bool SendPieceOPC_UA (Order::BaseOrder order);

    bool CheckPiecesFinished();

    bool CheckIncomingPieces();

    bool warehouseOutCarpetIsFree();
};

#endif
