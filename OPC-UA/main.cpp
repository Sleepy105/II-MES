#define DEBUG_UA
#define ASCII_NUMERAL_OFFSET 48
#define DEFAULT_INDEX 4

#define RIGHT   1
#define DOWN    2
#define LEFT    3
#define UP      4
#define PLACEHOLDER 1


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

#include <stdlib.h>
#include <string.h>


typedef struct {
    int16_t path[59];
    int16_t transformation;
    int16_t id_piece;
    int16_t type_piece;
}piece_object;


/*
    Converte um inteiro para uma string com o inteiro em ASCII (inclui o NULL character)
    args:
        char* string:   string que irá conter o resultado (vai ser alterada, não contém verificação de tamanho)
        int16_t value:  valor inteiro a converter para string
*/
void ConvIntToString(char* string, int16_t value) {
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



/*
    Escrever um object_information com tipo de transformação, caminho a percorrer, peça final, e peça inicial (atual)
    args:
        UA_Client*  client:     objeto do cliente para o qual se vai enviar o pedido de escrita
        char* BaseNodeID:       string identificadora do nó base
        int16_t[] path:         vetor de inteiros com as direções a tomar (devera ter 59 elementos)
        int16_t transformation: número identificador da transformacao pretendida
        int16_t id_piece:       número identificador da peca em si
        int16_t type_piece:     tipo de peca (1: P1; 2: P2; etc.)
        int16_t object_index:   índice do objeto a escrever, em princípio será sempre no índice 0 (ordem de transformacao) ou nos índices dos tapetes de saída (descarga)
    NOTA: se algum destes argumentos não fizer sentido (argumento "transformacao" no caso de ordem de descarga de uma peca p. ex.) colocar esse argumento a 0
*/
void WriteUA_Object(UA_Client* client, char* BaseNodeID, int16_t path[], int16_t transformation, int16_t id_piece, int16_t type_piece, int16_t object_index) {
#ifdef DEBUG_UA
    printf("\nWriting OBJECT type to master node (4, \"%s\")...\n", BaseNodeID);
#endif

    int16_t pathIDcounter = 1;

    // Converter dados em string identificadora do no
    char NodeID[128];
    char NodeID_backup[128];
    char aux[20];
    strcpy(NodeID, BaseNodeID);
    strcat(NodeID, "GVL.OBJECT[");
    ConvIntToString(aux, object_index);
    strcat(NodeID, aux);
    strcat(NodeID, "].");
    strcpy(NodeID_backup, NodeID); // Copiar backup, vamos voltar a esta string varias vezes
    int16_t NodeIDindex = DEFAULT_INDEX;

    // escrever transformacao
    UA_WriteRequest wReq;
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcat(NodeID, "transformation");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &transformation;
    UA_WriteResponse wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Values Written successfully to OBJECT node!\n");
#endif
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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &id_piece;
    wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Values Written successfully to OBJECT node!\n");
#endif
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
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &pathIDcounter;
    wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Values Written successfully to OBJECT node!\n");
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    // escrever ID da peca
    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "id_piece");
#ifdef DEBUG_UA
    printf("writting to \"%s\"\n", NodeID);
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
    wReq.nodesToWrite[0].value.value.data = &type_piece;
    wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Values Written successfully to OBJECT node!\n");
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);

    UA_Int16* path_UA = (UA_Int16*) UA_Array_new(59, &UA_TYPES[UA_TYPES_INT16]);
    for (int16_t i = 0; i < 59; i++) {
        path_UA[i] = path[i];
    }

    UA_WriteRequest_init(&wReq);
    wReq.nodesToWrite = UA_WriteValue_new();
    wReq.nodesToWriteSize = 1;
    strcpy(NodeID, NodeID_backup);
    strcat(NodeID, "path");
#ifdef DEBUG_UA
    printf("writting to \"%s\" the following array:\n", NodeID);
    for (int16_t i = 0; i < 59; i++) {
        printf("path[%d]=%d\t", i, path[i]);
    }
    printf("\n");
#endif
    wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(NodeIDindex, NodeID);
    wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    wReq.nodesToWrite[0].value.hasValue = true;
    wReq.nodesToWrite[0].value.value.arrayLength = 59;
    wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT16];
    wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    wReq.nodesToWrite[0].value.value.data = path_UA;
    wResp = UA_Client_Service_write(client, wReq);
#ifdef DEBUG_UA
    if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
        printf("Values Written successfully to OBJECT node!\n");
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);
}



/*
    Função simples para conectar ao servidor UA
    args:
        cont char* endpointURL: URL para o qual se deverá conectar
    ret:   
        apontador UA_Client* criado. Deverá ser usado como argumento em todas as funções de leitura/escrita
*/
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



/*
    Escrever um booleano num dado nó
    args:
        UA_Client* client:      objeto do cliente para o qual se vai enviar o pedido de escrita
        char* BaseNodeID:       string identificadora do nó base
        char* NodeExtension:    string com a restante parte do nó
        bool value:             valor a escrever
*/
void WriteUA_Bool(UA_Client* client, const char* BaseNodeID, const char* NodeExtension, bool value) {
#ifdef DEBUG_UA
    printf("\nWriting \"%d\" to node (4, \"%s%s\"):\n", value, BaseNodeID, NodeExtension);
#endif

    char NodeID[128];
    strcpy(NodeID, BaseNodeID);
    strcat(NodeID, NodeExtension);
    int16_t NodeIDindex = DEFAULT_INDEX;

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



/*
    Escrever um booleano num dado nó
    args:
        UA_Client* client:      objeto do cliente para o qual se vai enviar o pedido de escrita
        char* BaseNodeID:       string identificadora do nó base
        char* NodeExtension:    string com a restante parte do nó
        int16_t value:          valor a escrever
*/
void WriteUA_Int16(UA_Client* client, const char* BaseNodeID, const char* NodeExtension, int16_t value) {
#ifdef DEBUG_UA
    printf("\nWriting \"%d\" to node (4, \"%s%s\"):\n", value, BaseNodeID, NodeExtension);
#endif

    char NodeID[128];
    strcpy(NodeID, BaseNodeID);
    strcat(NodeID, NodeExtension);
    int16_t NodeIDindex = DEFAULT_INDEX;

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
        printf("Value Written!\n", value);
#endif
    UA_WriteRequest_clear(&wReq);
    UA_WriteResponse_clear(&wResp);
}



/*
    Ler um booleano num dado nó
    args:
        UA_Client* client:      objeto do cliente para o qual se vai enviar o pedido de escrita
        char* BaseNodeID:       string identificadora do nó base
        char* NodeExtension:    string com a restante parte do nó
    ret:
        booleano lido (TRUE ou FALSE/1 ou 0)
*/
bool ReadUA_Bool(UA_Client* client, const char* BaseNodeID, const char* NodeExtension) {
    bool value;

    char NodeID[128];
    strcpy(NodeID, BaseNodeID);
    strcat(NodeID, NodeExtension);
    int16_t NodeIDindex = DEFAULT_INDEX;

    UA_Variant* val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(NodeIDindex, NodeID), val);
    if (retval == UA_STATUSCODE_GOOD) {
        value = *(bool*)val->data;
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



int16_t ReadUA_Int16(UA_Client* client, const char* BaseNodeID, const char* NodeExtension) {
    int16_t value;

    char NodeID[128];
    strcpy(NodeID, BaseNodeID);
    strcat(NodeID, NodeExtension);
    int16_t NodeIDindex = DEFAULT_INDEX;

    UA_Variant* val = UA_Variant_new();
    UA_StatusCode retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(NodeIDindex, NodeID), val);
    if (retval == UA_STATUSCODE_GOOD) {
        value = *(int16_t*)val->data;
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

    int16_t BaseNodeIndex = 4;
    char BaseNodeID[128] = "|var|CODESYS Control Win V3 x64.Application.";

    piece_object peca;
    peca.id_piece = 1;
    peca.transformation = 3;
    peca.type_piece = 2;
    for (int16_t i = 0; i < 59; i++) {
        peca.path[i] = 0;
    }
    peca.path[1]  = RIGHT;
    peca.path[2]  = RIGHT;
    peca.path[3]  = DOWN;
    peca.path[10] = DOWN;
    peca.path[14] = PLACEHOLDER;
    peca.path[15] = DOWN;
    peca.path[25] = PLACEHOLDER;
    peca.path[26] = DOWN;
    peca.path[36] = PLACEHOLDER;
    peca.path[37] = DOWN;
    peca.path[47] = DOWN;
    peca.path[53] = LEFT;
    peca.path[52] = LEFT;
    peca.path[51] = LEFT;


    WriteUA_Object(client, BaseNodeID, peca.path, peca.transformation, peca.id_piece, peca.type_piece, 3);

    return 0;
}
