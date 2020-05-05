/**
 * @file UDPManager.hpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _UDPMANAGER_HPP_
#define _UDPMANAGER_HPP_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#include "XMLParser.hpp"

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

    void _worker(XMLParser* obj);
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
    std::thread spawn_worker(XMLParser* obj);
};

#endif
