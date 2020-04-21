#include <stdlib.h>
#include <stdio.h>

#include "UDPManager.cpp"
#include "XMLParser.cpp"

int main (int argc, char const *argv[]) {

    XMLParser XMLParser;

    UDPManager UDPManager(54321);
    std::thread udp_worker = UDPManager.spawn_worker(XMLParser.parseString);

    udp_worker.join();
    
    return 0;
}
