// OPC-UA-Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


//#define DEBUG_UA
#define ASCII_NUMERAL_OFFSET 48

#define RIGHT   1
#define DOWN    2
#define LEFT    3
#define UP      4
#define PLACEHOLDER 0


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#include "lib/open62541/plugins/include/open62541/client_config_default.h"
#include "lib/open62541/include/open62541/client_highlevel.h"
#include "lib/open62541/include/open62541/client_subscriptions.h"
#include "lib/open62541/plugins/include/open62541/plugin/log_stdout.h"
#include <string.h>
#include <iostream>
#include <string>


class OPCUA_Manager {
private:
    UA_Client *_client;
    const char* _BaseNodeID;
    int16_t _nodeIndex;
    bool _connected;

    UA_Client* ServerConnect(const char* endpointURL) const {
        UA_Client* client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(client));

        UA_StatusCode retval = UA_Client_connect(client, endpointURL);
        if (retval != UA_STATUSCODE_GOOD) {
            UA_Client_delete(client);
            return NULL;
        }
        return client;
    }

    void ConvIntToString(char* string, uint16_t value) {
        int aux = value, size, i;
        for (size = 1; aux > 9; size = size * 10) {
            aux = aux / 10;
        }

        for (i = 0; value > 9; i++) {
            string[i] = (value / size) + ASCII_NUMERAL_OFFSET;
            value = value - ((value / size) * size);
            size = size / 10;
        }
        string[i] = (value % 10) + ASCII_NUMERAL_OFFSET;
        string[i + 1] = 0;
    }

public:
    OPCUA_Manager(const char* URL, const char* BaseID, int16_t index = 4) {
        _client = ServerConnect(URL);
        _nodeIndex = index;
        _BaseNodeID = BaseID;
        if (!_client) {
            _connected = false;
        }
        else {
            _connected = true;
        }
    }

    bool Is_Connected() const {
        return _connected;
    }

    bool SendPieceOPC_UA(uint16_t path[], uint16_t transformation, uint16_t id_piece, uint16_t type_piece, uint16_t object_index) {

#ifdef DEBUG_UA
    printf("\nWriting OBJECT type to master node (4, \"%s\")...\n", _BaseNodeID);
#endif

    uint16_t pathIDcounter = 1;

    // Converter dados em string identificadora do no
    char NodeID[128];
    char NodeID_backup[128];
    char aux[20];
    strcpy(NodeID, _BaseNodeID);
    strcat(NodeID, "GVL.OBJECT[");
    ConvIntToString(aux, object_index);
    strcat(NodeID, aux);
    strcat(NodeID, "].");
    strcpy(NodeID_backup, NodeID); // Copiar backup, vamos voltar a esta string varias vezes

    // escrever transformacao
    UA_WriteRequest wReq;
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcat(NodeID, "transformation");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &transformation;
    UA_WriteResponse wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;

    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever tipo de peca
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "type_piece");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever valor inicial do path_id_counter
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path_id_counter");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &pathIDcounter;
    wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever ID da peca
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "id_piece");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);//|var|CODESYS Control Win V3 x64.Application.GVL.OBJECT[1].transformation
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &id_piece;
    wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever caminho que a pe�a vai percorrer
    UA_Int16* path_UA = (UA_Int16*)UA_Array_new(59, &UA_TYPES[UA_TYPES_UINT16]);
    for (uint16_t i = 0; i < 59; i++) {
        path_UA[i] = path[i];
    }

    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.arrayLength = 59;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    wReq.nodesToWrite[0].value.value.data = path_UA;
    wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\nWritting to Warehouse piece selector\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever tipo de pe�a na vari�vel da Warehouse para despoletar a sa�da da pe�a
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, _BaseNodeID);
    strcat(NodeID, "GVL.AT1_tp");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);//|var|CODESYS Control Win V3 x64.Application.GVL.OBJECT[1].transformation
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(_client, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    return true;
}

};


typedef struct {
    uint16_t path[59];
    uint16_t transformation;
    uint16_t id_piece;
    uint16_t type_piece;
}piece_object;


int main()
{

    OPCUA_Manager myManager("opc.tcp://127.0.0.1:4840","|var|CODESYS Control Win V3 x64.Application.",4);

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
    }

    return 0;
}