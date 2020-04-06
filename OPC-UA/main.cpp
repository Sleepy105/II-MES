// OPC-UA-Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define DEBUG_UA

#include "lib/open62541/plugins/include/open62541/client_config_default.h"
#include "lib/open62541/include/open62541/client_highlevel.h"
#include "lib/open62541/include/open62541/client_subscriptions.h"
#include "lib/open62541/plugins/include/open62541/plugin/log_stdout.h"
#include <stdlib.h>

/*
#include "open62541/client_config_default.h"
#include "open62541/client_highlevel.h"
#include "open62541/plugin/log_stdout.h"
#include <stdlib.h>*/

UA_Client* ServerConnect(const char* endpointURL) {
    UA_Client* client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    UA_StatusCode retval = UA_Client_connect(client, endpointURL);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return NULL;
    }
    return client;
}

void WriteUA_Bool(UA_Client* client, UA_Int16 NodeIDindex, char* NodeID, UA_Boolean value) {
#ifdef DEBUG_UA
    printf("\nWriting \"%i\" to node (4, \"%s\"):\n", value, NodeID);
#endif
    UA_WriteRequest wReq;
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &value;
    UA_WriteResponse wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Value Written!\nthe new value is: %i\n\n", value);
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);
}

void WriteUA_Int16(UA_Client* client, UA_Int16 NodeIDindex, char* NodeID, UA_Int16 value) {
#ifdef DEBUG_UA
    printf("\nWriting \"%i\" to node (4, \"%s\"):\n", value, NodeID);
#endif
    UA_WriteRequest wReq;
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &value;
    UA_WriteResponse wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Value Written!\nthe new value is: %i\n\n", value);
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);
}

UA_Boolean ReadUA_Bool(UA_Client* client, UA_Int16 NodeIDindex, char *NodeID) {
    UA_Boolean value;
    UA_Variant* val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(NodeIDindex, NodeID), val);
    if (retval == UA_STATUSCODE_GOOD) {
        value = *(UA_Boolean*)val->data;
#ifdef DEBUG_UA
        printf("Read value (Bool) is: %i\n", value);
#endif
    }
    else {
        value = 0;
#ifdef DEBUG_UA
        printf("Unable to Read value\n");
#endif
    }
    return value;
}

UA_Int16 ReadUA_Int16(UA_Client* client, UA_Int16 NodeIDindex, char* NodeID) {
    UA_Int16 value;
    UA_Variant* val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(NodeIDindex, NodeID), val);
    if (retval == UA_STATUSCODE_GOOD) {
        value = *(UA_Int16*)val->data;
#ifdef DEBUG_UA
        printf("Read value (Int16) is: %i\n", value);
#endif
    }
    else {
        value = 0;
#ifdef DEBUG_UA
        printf("Unable to Read value\n");
#endif
    }
    return value;
}



int main()
{
    const char URL[128] = "opc.tcp://127.0.0.1:4840";

    UA_Client* client = ServerConnect(URL);

    UA_Int16 NodeIndex1 = 4;
    char NodeID1[67] = "|var|CODESYS Control Win V3 x64.Application.MESvariables.connected";

    UA_Int16 NodeIndex2 = 4;
    char NodeID2[57] = "|var|CODESYS Control Win V3 x64.Application.PLC_PRG.test";

    UA_Boolean  value_bool;
    UA_Int16    value_int16;

    /* Read attribute */
    value_bool  = ReadUA_Bool (client, NodeIndex1, NodeID1);
    value_int16 = ReadUA_Int16(client, NodeIndex2, NodeID2);

    /* Write attribute */
    WriteUA_Bool(client, NodeIndex1, NodeID1, TRUE);
    WriteUA_Int16(client, NodeIndex2, NodeID2, 10);

    /* Re-read attribute to check that it actually changed */
    value_bool  = ReadUA_Bool (client, NodeIndex1, NodeID1);
    value_int16 = ReadUA_Int16(client, NodeIndex2, NodeID2);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
