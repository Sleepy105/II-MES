/**
 * @file UDPManager.cpp
 * @brief 
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "UDPManager.hpp"

UDPManager::UDPManager(uint16_t port, uint32_t buffer_size) : port(port), buffer_size(buffer_size) {
    socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_fd < 0) {
        meslog(ERROR) << "Socket creation failed." << std::endl;
    }
    
    serverlen = sizeof(server);
    clientlen = sizeof(client);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (sockaddr*)&server,serverlen)) {
        meslog(ERROR) << "Bind Failed." << std::endl;
    }

    buffer = (char*)calloc(buffer_size, sizeof(char));
    if (!buffer) {
        meslog(ERROR) << "Buffer allocation Failed." << std::endl;
    }

    meslog(INFO) << "Online" << std::endl;
}

UDPManager::UDPManager(uint16_t port) : UDPManager(port, UDPMANAGER_DEFAULT_BUFFER_SIZE) {}

UDPManager::~UDPManager() {
}

std::thread UDPManager::spawn_worker(XMLParser* obj) {
    std::thread worker (&UDPManager::_worker, this, obj);
    return worker;
}

void UDPManager::_worker(XMLParser* obj) {
    int rc = 0;
    while (true) {
        rc = recvfrom(socket_fd, buffer, buffer_size, 0, (sockaddr *)&client, &clientlen);
        if (rc && buffer) {
            std::string str = buffer;
            obj->parseString(str);
        } else {
            break;
        }
    }

    meslog(ERROR) << "Error reading from Socket." << std::endl;
}
