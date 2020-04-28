/**
 * @brief 
 * 
 */

#include "OPC-UA.hpp"

OPCUA_Manager::OPCUA_Manager(const char* URL, const char* BaseID, int16_t index) {
    client_ = ServerConnect(URL);
    nodeIndex_ = index;
    BaseNodeID_ = BaseID;
    if (!client_) {
        connected_ = false;
    }
    else {
        connected_ = true;
    }
}

bool OPCUA_Manager::Is_Connected() const {
    return connected_;
}

UA_Client* OPCUA_Manager::ServerConnect(const char* endpointURL) const {
    UA_Client* client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    UA_StatusCode retval = UA_Client_connect(client, endpointURL);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return NULL;
    }
    return client;
}

void OPCUA_Manager::ConvIntToString(char* string, uint16_t value) {
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


bool OPCUA_Manager::SendPieceOPC_UA(Order::BaseOrder order/*uint16_t path[], uint16_t transformation, uint16_t id_piece, uint16_t type_piece, uint16_t object_index*/) {
#ifdef DEBUG_UA
    printf("\nWriting OBJECT type to master node (4, \"%s\")...\n", BaseNodeID_);
#endif

    uint16_t pathIDcounter = 1;

    uint16_t object_index = 1;
    uint16_t transformation = (uint16_t)order.GetType();
    uint16_t type_piece = 6;
    uint16_t id_piece = 1;
    uint16_t path[59];
    pathfinder.FindPath(path);

    // Criar vetor em formato compatível com OPC-UA
    UA_Int16* path_UA = (UA_Int16*)UA_Array_new(59, &UA_TYPES[UA_TYPES_UINT16]);
    for (uint16_t i = 0; i < 59; i++) {
        path_UA[i] = path[i];
    }


    // Converter dados em string identificadora do no
    char NodeID[128];
    char NodeID_backup[128];
    char aux[20];
    strcpy(NodeID, BaseNodeID_);
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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &transformation;
    UA_WriteResponse wResp = UA_Client_Service_write(client_, wReq);

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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client_, wReq);

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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &pathIDcounter;
    wResp = UA_Client_Service_write(client_, wReq);

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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &id_piece;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
#ifdef DEBUG_UA
        printf("Values Written successfully to OBJECT node!\n");
#endif
    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever caminho que a pe�a vai percorrer
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.arrayLength = 59;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    wReq.nodesToWrite[0].value.value.data = path_UA;
    wResp = UA_Client_Service_write(client_, wReq);

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
    strcpy(NodeID, BaseNodeID_);
    strcat(NodeID, "GVL.AT1_tp");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);//|var|CODESYS Control Win V3 x64.Application.GVL.OBJECT[1].transformation
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client_, wReq);

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