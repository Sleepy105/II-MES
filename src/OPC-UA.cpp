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


bool OPCUA_Manager::warehouseOutCarpetIsFree() {
    char NodeID[128];
    strcpy(NodeID, BaseNodeID_);
    strcat(NodeID, "GVL.AT1_tp") // this variable will got to 0 after a piece has exited the warehouse carpet

    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    UA_ReadValueId ids[1];
    ids[0].attributeId = UA_ATTRIBUTEID_VALUE;
    ids[0].nodeId = UA_NODEID_STRING_ALLOC(_nodeIndex, NodeID);
    request.nodesToRead = ids;
    request.nodesToReadSize = 1;

    UA_ReadResponse response = UA_Client_Service_read(_client, request);
    if (*(UA_UInt16*) response.results[0].value.data != 0){ // there's already a piece, can't send new one
        return false
    } // else, carpet is free, can send new piece
    return true;
}


bool OPCUA_Manager::SendPieceOPC_UA(Order::BaseOrder order) {

    // Create base string for node access
    char NodeID[128];
    char NodeID_backup[128];
    char aux[20];
    strcpy(NodeID_backup, BaseNodeID_);
    strcat(NodeID_backup, "GVL.OBJECT[1]."); // we'll use this multiple times

    // Check if we can insert the piece (Entry carpet is free)
    if (!warehouseOutCarpetIsFree()){
        return false; // warehouse carpet is not free, can't send piece
    }

    uint16_t pathIDcounter = 1;

    uint16_t object_index = 1;
    uint16_t transformation = 1;
    uint16_t type_piece = order.GetInitialPiece();

    // OLD IMPLEMENTATION: scan pieces for pieces that haven't been processed (not On Hold). Turns out this is the last piece, always
    // std::list<Order::Piece>::iterator piece_iter = order.GetPieces().begin();
	// while ( !((*piece_iter).isOnHold()) ){
	// 	if (piece_iter == order.GetPieces().end()){
	// 		return false;
	// 	}
	// 	piece_iter++;
	// }

    // NEW IMPLEMENTATION: get pieces from Piece list back, aka end
    uint32_t id_piece = order.GetPieces().back().GetID();
    uint8_t *path = order.GetPieces().back().GetPath();

    // Criar vetor em formato compatível com OPC-UA
    UA_Int16* path_UA = (UA_Int16*)UA_Array_new(59, &UA_TYPES[UA_TYPES_UINT16]);
    for (uint16_t i = 0; i < 59; i++) {
        path_UA[i] = (uint16_t) path[i];
    }

    // TESTING PENDING!!! Found out how to write in multiple places in a single write request, 
    // but this was found out by me (didn't see anyone else doing this), and might have unforeseen problems
    UA_WriteRequest wReq;
    UA_WriteValue my_nodes[5];
    UA_WriteValue_init(&my_nodes[0]);
    UA_WriteValue_init(&my_nodes[1]);
    UA_WriteValue_init(&my_nodes[2]);
    UA_WriteValue_init(&my_nodes[3]);
    UA_WriteValue_init(&my_nodes[4]);
    wReq.nodesToWrite = my_nodes;
    wReq.nodesToWriteSize = 5;
    i=0;
    // transformation node write
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "transformation");
    my_nodes[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    my_nodes[i].attributeId = UA_ATTRIBUTEID_VALUE;
    my_nodes[i].value.hasValue = true;
    my_nodes[i].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    my_nodes[i].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    my_nodes[i].value.value.data = &transformation;
    i++;

    // piece type node write
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "type_piece");
    my_nodes[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    my_nodes[i].attributeId = UA_ATTRIBUTEID_VALUE;
    my_nodes[i].value.hasValue = true;
    my_nodes[i].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    my_nodes[i].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    my_nodes[i].value.value.data = &type_piece;
    i++;

    // path_id_counter node write
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path_id_counter");

    my_nodes[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    my_nodes[i].attributeId = UA_ATTRIBUTEID_VALUE;
    my_nodes[i].value.hasValue = true;
    my_nodes[i].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    my_nodes[i].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    my_nodes[i].value.value.data = &pathIDcounter;
    i++;

    // piece id node write
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "id_piece");

    my_nodes[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    my_nodes[i].attributeId = UA_ATTRIBUTEID_VALUE;
    my_nodes[i].value.hasValue = true;
    my_nodes[i].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    my_nodes[i].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    my_nodes[i].value.value.data = &id_piece;
    i++;

    // path node write
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path");

    my_nodes[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    my_nodes[i].attributeId = UA_ATTRIBUTEID_VALUE;
    my_nodes[i].value.hasValue = true;
    my_nodes[i].value.value.arrayLength = 59;
    my_nodes[i].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    my_nodes[i].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    my_nodes[i].value.value.data = path_UA;

    // Send all node writes at once
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        return false;
    }
    // A parte que tenho duvidas surge agora nestas proximas 4 linhas de codigo:
    // O problema que surge esta no "WriteRequest_clear()" tentar apagar os my_nodes[].
    // Ele chama um free interno, mas os my_nodes[] estao declarados na stack (logo o free() falha).
    // tentei po-los na heap (*my_nodes = malloc(sizeof(UA_WriteValue)*5) em vez de my_nodes[5]), mas
    // nao funcionou, nao consigo perceber porque.
    // A solucao que arranjei foi "enganar" o clear em pensar que as variaveis estao vazias (NULL).
    // Efetivamente isto resulta, mas tenho medo que hajam memory leaks que me estejam a escapar...
    // Penso que nao hajam, visto que, estando na stack, depois de sair da funcao estas variaveis vao
    // a vida. Mas nao sei ate que ponto o UA_WriteValue_init() chamado no inicio faz alocacoes sem
    // eu saber...
    wReq.nodesToWrite = NULL;
    wReq.nodesToWriteSize = 0;
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // Escrever tipo de peca na variavel da Warehouse para despoletar a saida da peca.
    //
    //  Este tem mesmo de ser escrito num pedido a parte, porque tem de ser escrito no fim,
    // como eu escrevi os outros nao sei se o OPC-UA escreve tudo numa so operacao atomica
    // ou se vai escrevendo simplesmente por uma ordem aleatoria, sem grande criterio...
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, BaseNodeID_);
    strcat(NodeID, "GVL.AT1_tp");

    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client_, wReq);

    if (wResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        return false;
    }
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