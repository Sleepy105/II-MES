/**
 * @file XMLParser.cpp
 * @brief 
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>
#include <sstream>

#include "../lib/tinyxml2/tinyxml2.h"
using namespace tinyxml2;

#include "Order.cpp"

class XMLParser {
private:
    static int xml_to_int(const char* xml);
    static uint32_t xml_to_uint32(const char* xml);
public:
    XMLParser(/* args */);
    ~XMLParser();

    static void parseString(std::string str);
};

XMLParser::XMLParser(/* args */) {
}

XMLParser::~XMLParser() {
}

void XMLParser::parseString(std::string str) {
    XMLDocument doc;
    doc.Parse(str.c_str(), str.length());
    XMLElement* order = doc.FirstChildElement( "ORDERS" )->FirstChildElement( "Order" );
    for (; order != NULL; order = order->NextSiblingElement()) {
        uint8_t order_id = (uint8_t)xml_to_int(order->Attribute("Number"));
        
        XMLElement* transform = order->FirstChildElement("Transform");
        if (transform) {
            uint32_t max_delay = (uint32_t)xml_to_int(transform->Attribute("MaxDelay"));
            auto order = new Order::BaseOrder(order_id, ORDER_TYPE_TRANSFORMATON);
            continue;
        }

        XMLElement* unload = order->FirstChildElement("Unload");
        if (unload) {
            uint32_t max_delay = (uint32_t)xml_to_int(unload->Attribute("MaxDelay"));
            auto order = new Order::BaseOrder(order_id, ORDER_TYPE_UNLOAD);
            continue;
        }

        XMLElement* request_stores = order->FirstChildElement("Request_Stores");
        if (request_stores) {
            auto order = new Order::BaseOrder(order_id, ORDER_TYPE_REQUESTSTORES);
            continue;
        }
    }
}

int XMLParser::xml_to_int(const char* xml) {
    std::stringstream strValue;
    int intValue;
    strValue << xml;
    strValue >> intValue;
    return intValue;
}