/**
 * @file XMLParser.cpp
 * @brief 
 * @version 0.1
 * @date 2020-04-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "XMLParser.hpp"
using namespace tinyxml2;

XMLParser::XMLParser(OrderQueue* obj) : queue(obj) {
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
            if (!parse_Transformation(order_id, transform)) {
                handleParsingError();
            }
            continue;
        }

        XMLElement* unload = order->FirstChildElement("Unload");
        if (unload) {
            if (!parse_Unload(order_id, unload)) {
                handleParsingError();
            }
            continue;
        }

        XMLElement* request_stores = order->FirstChildElement("Request_Stores");
        if (request_stores) {
            if (!parse_RequestStores(order_id, request_stores)) {
                handleParsingError();
            }
            continue;
        }
    }
}

bool XMLParser::parse_Transformation(uint8_t order_id, XMLElement* transform) {
    uint32_t from = (uint32_t)xml_to_int(transform->Attribute("From"));
    uint32_t to = (uint32_t)xml_to_int(transform->Attribute("To"));
    uint32_t quantity = (uint32_t)xml_to_int(transform->Attribute("quantity"));
    uint32_t max_delay = (uint32_t)xml_to_int(transform->Attribute("MaxDelay"));
    return queue->AddOrder(Order::BaseOrder(order_id, Order::ORDER_TYPE_TRANSFORMATON, quantity, from, to));
}

bool XMLParser::parse_Unload(uint8_t order_id, XMLElement* unload) {
    uint32_t quantity = (uint32_t)xml_to_int(unload->Attribute("quantity"));
    uint32_t max_delay = (uint32_t)xml_to_int(unload->Attribute("MaxDelay"));
    return queue->AddOrder(Order::BaseOrder(order_id, Order::ORDER_TYPE_UNLOAD, quantity));
}

bool XMLParser::parse_RequestStores(uint8_t order_id, XMLElement* request_stores) {
    return queue->AddOrder(Order::BaseOrder(order_id, Order::ORDER_TYPE_REQUESTSTORES));
}

void XMLParser::handleParsingError() {
    /**
     *  TODO: Handle parsing errors. Should send a message back through UDP
     *
     */

    return;
}

int XMLParser::xml_to_int(const char* xml) {
    std::stringstream strValue;
    int intValue;
    strValue << xml;
    strValue >> intValue;
    return intValue;
}