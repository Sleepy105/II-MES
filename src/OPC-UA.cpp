/**
 * @brief 
 * 
 */

#include "OPC-UA.hpp"

OPCUA_Manager::OPCUA_Manager(const char* URL, const char* BaseID, int16_t index, OrderQueue *order_queue_reference, Warehouse *warehouse_reference) {
    client_ = ServerConnect(URL);
    nodeIndex_ = index;
    BaseNodeID_ = BaseID;
    strcpy(URL_, URL);
    if (!client_) {
        connected_ = false;
    }
    else {
        connected_ = true;
    }


}



bool OPCUA_Manager::Is_Connected() {
    return (UA_Client_connect(client_, URL_) == UA_STATUSCODE_GOOD);
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



bool OPCUA_Manager::SendPieceOPC_UA(Order::BaseOrder order) {

    uint16_t pathIDcounter = 1;

    uint16_t object_index = 1;
    uint16_t transformation = 1;
    uint16_t type_piece = order.GetInitialPiece();

    std::list<Order::Piece>::iterator piece_iter = order.GetPieces().begin();
	while ( !((*piece_iter).isOnHold()) ){
		if (piece_iter == order.GetPieces().end()){
			return false;
		}
		piece_iter++;
	}

    uint32_t id_piece = (*piece_iter).GetID();
    uint8_t *path = (*piece_iter).GetPath();

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
    strcat(NodeID, "GVL.OBJECT[1].");
    strcpy(NodeID_backup, NodeID); // Copiar backup, vamos voltar a esta string varias vezes

    // escrever transformacao
    UA_WriteRequest wReq;
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcat(NodeID, "transformation");

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &transformation;
    UA_WriteResponse wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {

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

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD){
        return false;
    }
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever valor inicial do path_id_counter
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path_id_counter");

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &pathIDcounter;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {

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

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &id_piece;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {

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

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.arrayLength = 59;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    wReq.nodesToWrite[0].value.value.data = path_UA;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {

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

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {

    }
    else return false;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    return true;
}



bool OPCUA_Manager::CheckPiecesFinished(){
    // Check if there are true booleans in warehouse exit
    // POU.AT2.piece_queue[10]
    // POU.AT2.piece_id_array[10]
    return true;
}



bool OPCUA_Manager::CheckIncomingPieces(){
    UA_WriteRequest wReq;
    UA_WriteResponse wResp;
    Order::BaseOrder *order_to_add;
    uint16_t PieceID;

    ////////////////////////////////////////////////////////////// CARPET C7T1B
    //Check if there's a piece in carpet C7T1b, which is just checking if PLC wrote to MES_ok
    char NodeID[128];
    
    // MES ok variable is not set (we haven't processed this piece yet)
    strcpy (NodeID,BaseNodeID_);
    strcat (NodeID,"POU.C7T1b.MES_ok");

    UA_Variant *val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client_, UA_NODEID_STRING(nodeIndex_, NodeID), val);
    if(retval == UA_STATUSCODE_GOOD) {
        value = *(UA_Boolean*)val->data;
    }
    // MES ok is false: we have not yet processed this piece
    if (value == false){
        PieceID = (uint16_t) order_queue->AddOrder(Order::BaseOrder(0, Order::ORDER_TYPE_LOAD, 1, 1, 1, "0")); //deadline não interessa, pus "0" just in case

        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = UA_WriteValue_new();
        wReq.nodesToWriteSize = 1;
        wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
        wReq.nodesToWrite[0].value.hasValue = true;
        wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
        wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        wReq.nodesToWrite[0].value.value.data = &PieceID;
        wResp = UA_Client_Service_write(client_, wReq);

        UA_WriteRequest_clear(&wReq);
        UA_WriteResponse_clear(&wResp);

        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = UA_WriteValue_new();
        wReq.nodesToWriteSize = 1;
        wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
        wReq.nodesToWrite[0].value.hasValue = true;
        wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
        wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        wReq.nodesToWrite[0].value.value.data = &Mes_is_ok;
        wResp = UA_Client_Service_write(client_, wReq);

        UA_WriteRequest_clear(&wReq);
        UA_WriteResponse_clear(&wResp);
    }

    ////////////////////////////////////////////////////////////// CARPET C7T7B
    //Check if there's a piece in carpet C7T7b
    strcpy (NodeID,BaseNodeID_);
    strcat (NodeID,"POU.C7T7b.MES_ok");

    retval = UA_Client_readValueAttribute(client_, UA_NODEID_STRING(nodeIndex_, NodeID), val);
    if(retval == UA_STATUSCODE_GOOD) {
        value = *(UA_Boolean*)val->data;
    }
    // MES ok is false: we have not yet processed this piece
    if (value == false){
        // Adicionar Order antes de mandar o MES_ok
        PieceID = (uint16_t) order_queue->AddOrder(Order::BaseOrder(0, Order::ORDER_TYPE_LOAD, 1, 2, 2, "0")); //deadline não interessa, pus "0" just in case

        // Atribuir ID da peça
        UA_WriteRequest_init(&wReq);
        strcpy (NodeID,BaseNodeID_);
        strcat (NodeID,"GVL.OBJECT");
        wReq.nodesToWrite = UA_WriteValue_new();
        wReq.nodesToWriteSize = 1;
        wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
        wReq.nodesToWrite[0].value.hasValue = true;
        wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
        wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        wReq.nodesToWrite[0].value.value.data = &PieceID;
        wResp = UA_Client_Service_write(client_, wReq);

        UA_WriteRequest_clear(&wReq);
        UA_WriteResponse_clear(&wResp);

        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = UA_WriteValue_new();
        wReq.nodesToWriteSize = 1;
        wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
        wReq.nodesToWrite[0].value.hasValue = true;
        wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
        wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        wReq.nodesToWrite[0].value.value.data = &Mes_is_ok;
        wResp = UA_Client_Service_write(client_, wReq);

        UA_WriteRequest_clear(&wReq);
        UA_WriteResponse_clear(&wResp);
    }
    UA_Variant_delete(val);


    return true;
}