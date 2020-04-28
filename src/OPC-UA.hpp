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
#include "Order.hpp"
#include "PathFinder.hpp"
#include "Warehouse.hpp"

class OPCUA_Manager {
private:
    UA_Client *client_;
    const char* BaseNodeID_;
    int16_t nodeIndex_;
    bool connected_;
    PathFinder pathfinder;

    UA_Client* ServerConnect(const char* endpointURL) const;
    void ConvIntToString(char* string, uint16_t value);

public:
    OPCUA_Manager(const char* URL, const char* BaseID, int16_t index = 4);

    bool Is_Connected() const;

    //bool SendPieceOPC_UA(uint16_t path[], uint16_t transformation, uint16_t id_piece, uint16_t type_piece, uint16_t object_index);
    bool SendPieceOPC_UA (Order::BaseOrder order);
};

#endif
