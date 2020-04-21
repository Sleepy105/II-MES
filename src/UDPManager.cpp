/**
 * @file UDPManager.cpp
 * @brief 
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#include "helpers.h"

#define UDPMANAGER_DEFAULT_BUFFER_SIZE 2000

class UDPManager {
private:
    uint16_t port;
    uint32_t buffer_size = UDPMANAGER_DEFAULT_BUFFER_SIZE; // Has to be larger than the max length of an UDP/IP Packet
    int socket_fd = -1;
    sockaddr_in server, client;
    socklen_t serverlen, clientlen;
    char* buffer = NULL;

    void _worker(void (*callback)(std::string));
public:
    /**
     * @brief Construct a new UDPManager object
     * 
     * @param port UPD Port to listen for cummunications
     */
    UDPManager(uint16_t port);

    /**
     * @brief Construct a new UDPManager object
     * 
     * @param port UPD Port to listen for cummunications
     * @param buffer_size Size of the data buffer
     */
    UDPManager(uint16_t port, uint32_t buffer_size);

    /**
     * @brief Destroy the UDPManager object
     * 
     */
    ~UDPManager();

    /**
     * @brief Spawn a new thread, which listens to the selected port
     * 
     */
    std::thread spawn_worker(void (*callback)(std::string));
};

UDPManager::UDPManager(uint16_t port, uint32_t buffer_size) : port(port), buffer_size(buffer_size) {
    socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_fd < 0) {
        log(ERROR) << "Socket creation failed." << std::endl;
    }
    
    serverlen = sizeof(server);
    clientlen = sizeof(client);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (sockaddr*)&server,serverlen)) {
        log(ERROR) << "Bind Failed." << std::endl;
    }

    buffer = (char*)calloc(buffer_size, sizeof(char));
    if (!buffer) {
        log(ERROR) << "Buffer allocation Failed." << std::endl;
    }

    log(INFO) << "Online" << std::endl;
}

UDPManager::UDPManager(uint16_t port) : UDPManager(port, UDPMANAGER_DEFAULT_BUFFER_SIZE) {}

UDPManager::~UDPManager() {
}

std::thread UDPManager::spawn_worker(void (*callback)(std::string)) {
    std::thread worker (&UDPManager::_worker, this, callback);
    return worker;
}

void UDPManager::_worker(void (*callback)(std::string)) {
    int rc = 0;
    while (true) {
        rc = recvfrom(socket_fd, buffer, buffer_size, 0, (sockaddr *)&client, &clientlen);
        if (rc && buffer) {
            std::string str = buffer;
            callback(str);
        } else {
            break;
        }
    }

    log(ERROR) << "Error reading from Socket." << std::endl;
}
