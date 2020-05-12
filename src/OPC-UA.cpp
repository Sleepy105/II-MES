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


// Completo: reconecta para testar conecao
bool OPCUA_Manager::Is_Connected() {
    return (UA_Client_connect(client_, URL_) == UA_STATUSCODE_GOOD);
}


// Completo: conecta ao servidor e retorna objeto (estrutura) client a ser usado por outras funcoes
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


// Completo: pega em value e guarda-o numa string old fashioned C
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

// Completo: retorna true se nao houver pecas/transicao de pecas na carpete de saida de armazem
bool OPCUA_Manager::warehouseOutCarpetIsFree() {
    char NodeID[128];
    strcpy(NodeID, BaseNodeID_);
    strcat(NodeID, "GVL.AT1_tp"); // this variable will got to 0 after a piece has exited the warehouse carpet

    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    UA_ReadValueId nodes_to_read[1];
    nodes_to_read[0].attributeId = UA_ATTRIBUTEID_VALUE;
    nodes_to_read[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    request.nodesToRead = nodes_to_read;
    request.nodesToReadSize = 1;

    UA_ReadResponse response = UA_Client_Service_read(client_, request);
    if (*(UA_UInt16*) response.results[0].value.data != 0){ // there's already a piece, can't send new one
        return false;
    } // else, carpet is free, can send new piece
    return true;
}

// Quase completo: falta substituir dummy value de transformation.
// Envia ultima peca que esteja na lista de pecas da order. 
// Nesta altura a peca ja deve estar na base de dados e ter um id atribuido
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

    // Get last piece from order list
    uint32_t id_piece = order.GetPieces().back().GetID();
    uint8_t *path = order.GetPieces().back().GetPath();

    // Criar vetor em formato compatível com OPC-UA
    UA_Int16* path_UA = (UA_Int16*)UA_Array_new(59, &UA_TYPES[UA_TYPES_UINT16]);
    uint16_t i;
    for (i = 0; i < 59; i++) {
        path_UA[i] = (uint16_t) path[i];
    }

    // TESTING PENDING!!! Found out how to write in multiple places in a single write request, 
    // but this was found out by me (didn't see anyone else doing this), and might have unforeseen problems
    UA_WriteResponse wResp;
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


// Incompleto e nao testado: id das pecas e lido, mas nao se faz nada quanto a isso.
// Falta remover as pecas da lista de pecas e atualizar orders conforme ids lidos.
// Verifica buffer de pecas que deram entrada no armazem. Le as pecas todas imediatamente
// i.e. nao e preciso executar esta funcao 5x para ler 5 pecas, sempre que se chama a
// funcao ela lê as pecas todas e limpa o buffer. Consoante o buffer, leem-se os ids
// das pecas para um vetor e mete-se as flags respetivas do buffer como "lidas" para
// o PLC saber que pode escrever nesse sitio do buffer. Falta remover as pecas recebidas
// da lista de pecas da OrderQueue
// Retorna true se conseguiu ler pecas, ou false se nao conseguiu (porque nao havia)
bool OPCUA_Manager::CheckPiecesFinished(){
    char NodeID[128];
    char NodeID_backup[128];
    char aux[3];
    uint16_t number_of_ids_to_read;
    uint16_t piece_ids[10] = {0};
    bool all_up_to_date = true;

    strcpy (NodeID_backup, BaseNodeID_);
    strcat (NodeID_backup, "POU.AT2.piece_queue[");
    

    // Check if there are true booleans in warehouse entry carpet (pieces that the warehouse 
    // received but haven't yet been processed by MES)
    bool piece_queue[10] = { false };
    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    UA_ReadValueId nodes_to_read[10];

    uint16_t i;

    for (i = 0; i < 10; i++) {
        strcpy(NodeID, NodeID_backup);
        ConvIntToString(aux, i + 1); //PLC array indexes start at 1
        strcat(NodeID, aux);
        strcat(NodeID, "]");

        UA_ReadValueId_init(&nodes_to_read[i]);
        nodes_to_read[i].attributeId = UA_ATTRIBUTEID_VALUE;
        nodes_to_read[i].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
    }

    request.nodesToRead = nodes_to_read;
    request.nodesToReadSize = 10;

    UA_ReadResponse response = UA_Client_Service_read(client_, request);
    if (response.results[0].value.type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
        for (i = 0; i < 10; i++) {
            piece_queue[i] = *(UA_Boolean*)response.results[i].value.data;
        }
    } 
    // bool results are now in piece_queue[10];

    
    strcpy (NodeID_backup, BaseNodeID_);
    strcat (NodeID_backup, "POU.AT2.piece_id_array[");

    // check if the queue has any unprocessed pieces and count how many nodes we need to read
    // start initializing nodes_to_read as we go (save us from another loop)
    number_of_ids_to_read = 0;
    for (i = 0; i < 10; i++){
        if (piece_queue[i]){
            all_up_to_date = false; // at least one element is true -> not all is up to date

            strcpy(NodeID, NodeID_backup);
            ConvIntToString(aux, i + 1); //PLC array indexes start at 1
            strcat(NodeID, aux);
            strcat(NodeID, "]");

            UA_ReadValueId_init(&nodes_to_read[number_of_ids_to_read]);
            nodes_to_read[number_of_ids_to_read].attributeId = UA_ATTRIBUTEID_VALUE;
            nodes_to_read[number_of_ids_to_read].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);

            number_of_ids_to_read++;
        }
    }

    if (all_up_to_date){
        return false; // nothing to do, return "false" meaning no pieces were finished
    }

    // Not all_up_to_date. Start reading id's
    request.nodesToRead = nodes_to_read;
    request.nodesToReadSize = number_of_ids_to_read;

    response = UA_Client_Service_read(client_, request);
    if (response.results[0].value.type == &UA_TYPES[UA_TYPES_UINT16]) {
        for (i = 0; i < number_of_ids_to_read; i++) {
            piece_ids[i] = *(UA_UInt16*)response.results[i].value.data;
            order_queue->RemovePiece((uint32_t)piece_ids[i]);
        }
    }

    // Now that we have the ids, we can tag the read piece_queue indexes as "false".
    // We don't need to overwrite the old ids, they will just be overwritten later on by the PLC upon
    // checking that the piece_queue index that corresponds to it is "false".

    uint16_t node_index = 0;
    UA_WriteRequest wReq;
    UA_WriteResponse wResp;
    UA_WriteValue nodes_to_write[10];
    bool write_false = false;

    strcpy (NodeID_backup, BaseNodeID_);
    strcat (NodeID_backup, "POU.AT2.piece_queue[");

    // fill in which nodes to write to
    // we could simply write to all nodes in the queue, but we risk that the PLC writes to a node while this function
    // executes and then we'd miss that read, and a piece would disappear from the MES (as if it would be stuck forever 
    // in the factory floor)
    for (i = 0; i < 10; i++){
        if (piece_queue[i]){

            strcpy(NodeID, NodeID_backup);
            ConvIntToString(aux, i + 1); // PLC array indexes start at 1
            strcat(NodeID, aux);
            strcat(NodeID, "]");

            nodes_to_write[node_index].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
            nodes_to_write[node_index].attributeId = UA_ATTRIBUTEID_VALUE;
            nodes_to_write[node_index].value.hasValue = true;
            nodes_to_write[node_index].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
            nodes_to_write[node_index].value.value.storageType = UA_VARIANT_DATA_NODELETE;
            nodes_to_write[node_index].value.value.data = &write_false;
            node_index++;
        }
    }

    // start write request
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = nodes_to_write;
    wReq.nodesToWriteSize = node_index; // same as total node amount which is the same as "number_of_ids_to_read" obtained previously
    wResp = UA_Client_Service_write(client_, wReq);
    wReq.nodesToWrite = NULL;
    wReq.nodesToWriteSize = 0;
    UA_WriteResponse_clear(&wResp);
    UA_WriteRequest_clear(&wReq);

    // We now have an array, in piece_ids with all ids of pieces that have finished. But we haven't processed them yet
    // TO BE IMPLEMENTED: UPDATE ORDERS BASED ON RETRIEVED IDS

    return true;
}


// Completo mas nao testado.
// Verifica pecas que estejam nos tapetes de carga. Se houver pecas adiciona-as
// imediatamente a uma order que tambem e criada pela funcao. Esta funcao tambem
// serve para o PLC poder prosseguir e colocar a peca acabada de chegar no armazem.
// Retorna true se tiver detetado pecas (em qualquer um dos tapetes) e false se ambos
// os tapetes estava vazios
bool OPCUA_Manager::CheckIncomingPieces(){
    // to write stuff with OPC_UA (might go unused)
    UA_WriteRequest wReq;
    UA_WriteResponse wResp;
    UA_WriteValue nodes_to_write[2];
    uint16_t PieceID;
    bool Mes_is_ok = true;
    bool MES_ok;

    bool return_value = false;

    Order::BaseOrder *order_to_add;

    ////////////////////////////////////////////////////////////// CARPET C7T1B
    char NodeID[128];
    
    // Check if MES ok variable in PLC is not set (we haven't processed this piece yet)
    strcpy (NodeID,BaseNodeID_);
    strcat (NodeID,"POU.C7T1b.MES_ok");

    UA_Variant *val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client_, UA_NODEID_STRING(nodeIndex_, NodeID), val);
    if(retval == UA_STATUSCODE_GOOD) {
        MES_ok = *(UA_Boolean*)val->data;
    }
    UA_Variant_delete(val);
    // MES ok is false: we have not yet processed this piece
    if (!MES_ok){
        PieceID = (uint16_t) order_queue->AddOrder(Order::BaseOrder(0, Order::ORDER_TYPE_LOAD, 1, 1, 1, "0")); //deadline não interessa, pus "0" just in case

        UA_WriteValue nodes_to_write[2];
        UA_WriteValue_init(&nodes_to_write[0]);
        UA_WriteValue_init(&nodes_to_write[1]);
        UA_WriteRequest_init(&wReq);

        strcpy(NodeID, BaseNodeID_);
        strcat(NodeID, "GVL.OBJECT[9].id_piece");
        nodes_to_write[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        nodes_to_write[0].attributeId = UA_ATTRIBUTEID_VALUE;
        nodes_to_write[0].value.hasValue = true;
        nodes_to_write[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
        nodes_to_write[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        nodes_to_write[0].value.value.data = &PieceID;
        
        strcpy(NodeID, BaseNodeID_);
        strcat(NodeID, "POU.C7T1b.MES_ok");
        nodes_to_write[1].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        nodes_to_write[1].attributeId = UA_ATTRIBUTEID_VALUE;
        nodes_to_write[1].value.hasValue = true;
        nodes_to_write[1].value.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
        nodes_to_write[1].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        nodes_to_write[1].value.value.data = &Mes_is_ok;
    
        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = nodes_to_write;
        wReq.nodesToWriteSize = 2;
        wResp = UA_Client_Service_write(client_, wReq);
        wReq.nodesToWrite = NULL;
        wReq.nodesToWriteSize = 0;
        UA_WriteResponse_clear(&wResp);
        UA_WriteRequest_clear(&wReq);

        meslog(INFO) << "Received piece P1 on carpet C7T1b" << std::endl;
        return_value = true;
    }

    ////////////////////////////////////////////////////////////// CARPET C7T7B
    // Check if MES ok variable in PLC is not set (we haven't processed this piece yet)
    strcpy (NodeID,BaseNodeID_);
    strcat (NodeID,"POU.C7T7b.MES_ok");

    retval = UA_Client_readValueAttribute(client_, UA_NODEID_STRING(nodeIndex_, NodeID), val);
    if(retval == UA_STATUSCODE_GOOD) {
        MES_ok = *(UA_Boolean*)val->data;
    }
    UA_Variant_delete(val);
    // MES ok is false: we have not yet processed this piece
    if (!MES_ok){
        // Adicionar Order antes de mandar o MES_ok
        PieceID = (uint16_t) order_queue->AddOrder(Order::BaseOrder(0, Order::ORDER_TYPE_LOAD, 1, 2, 2, "0")); //deadline não interessa, pus "0" just in case

        UA_WriteValue nodes_to_write[2];
        UA_WriteValue_init(&nodes_to_write[0]);
        UA_WriteValue_init(&nodes_to_write[1]);
        UA_WriteRequest_init(&wReq);

        strcpy(NodeID, BaseNodeID_);
        strcat(NodeID, "GVL.OBJECT[59].id_piece");
        nodes_to_write[0].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        nodes_to_write[0].attributeId = UA_ATTRIBUTEID_VALUE;
        nodes_to_write[0].value.hasValue = true;
        nodes_to_write[0].value.value.type = &UA_TYPES[UA_TYPES_UINT16];
        nodes_to_write[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        nodes_to_write[0].value.value.data = &PieceID;
        
        strcpy(NodeID, BaseNodeID_);
        strcat(NodeID, "POU.C7T7b.MES_ok");
        nodes_to_write[1].nodeId = UA_NODEID_STRING_ALLOC(nodeIndex_, NodeID);
        nodes_to_write[1].attributeId = UA_ATTRIBUTEID_VALUE;
        nodes_to_write[1].value.hasValue = true;
        nodes_to_write[1].value.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
        nodes_to_write[1].value.value.storageType = UA_VARIANT_DATA_NODELETE;
        nodes_to_write[1].value.value.data = &Mes_is_ok;
    
        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = nodes_to_write;
        wReq.nodesToWriteSize = 2;
        wResp = UA_Client_Service_write(client_, wReq);
        wReq.nodesToWrite = NULL;
        wReq.nodesToWriteSize = 0;
        UA_WriteResponse_clear(&wResp);
        UA_WriteRequest_clear(&wReq);

        meslog(INFO) << "Received piece P2 on carpet C7T7b" << std::endl;
        return_value = true;
    }


    return return_value;
}