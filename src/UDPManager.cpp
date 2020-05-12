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

    if (!isSocketValid()) {
        meslog(ERROR) << "Socket creation failed." << std::endl;
        return;
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
    free(buffer);
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

bool UDPManager::isSocketValid() {
    if (socket_fd < 0) {
        meslog(ERROR) << "Invalid UDP Socket." << std::endl;
        return false;
    }
    return true;
}

bool UDPManager::sendData(char* data, size_t data_size) {
    if (!isSocketValid()) {
        meslog(ERROR) << "Error sending data." << std::endl;
        return false;
    }

    if (!data_size) {
        data_size = strlen(data);
        if (!data_size) {
            meslog(ERROR) << "Message is empty." << std::endl;
            return false;
        }
    }

    ssize_t rc = sendto(socket_fd, data, data_size, MSG_CONFIRM, (sockaddr*)&client, clientlen);
    if (rc < 0) {
        meslog(ERROR) << "Error sending data." << std::endl;
        return false;
    }

    meslog(INFO) << "Data sent successfully." << std::endl;
    return true;
}