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
#include "UDPManager.hpp"
using namespace tinyxml2;

XMLParser::XMLParser(OrderQueue* obj, void* udp, void* warehouse) : queue(obj), udp(udp), warehouse(warehouse) {
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
    }

    order = doc.FirstChildElement( "ORDERS" )->FirstChildElement( "Request_Stores" );
    for (; order != NULL; order = order->NextSiblingElement()) {
        if (!parse_RequestStores()) {
            handleParsingError();
        }
    }
}

bool XMLParser::parse_Transformation(uint8_t order_id, XMLElement* transform) {
    uint8_t from = (uint8_t)xml_to_int(transform->Attribute("From"));
    uint8_t to = (uint8_t)xml_to_int(transform->Attribute("To"));
    uint32_t quantity = (uint32_t)xml_to_int(transform->Attribute("Quantity"));
    uint32_t max_delay = (uint32_t)xml_to_int(transform->Attribute("MaxDelay"));

    return queue->AddOrder(Order::BaseOrder(order_id, Order::ORDER_TYPE_TRANSFORMATION, quantity, from, to, max_delay));
}

bool XMLParser::parse_Unload(uint8_t order_id, XMLElement* unload) {
    uint8_t type = (uint8_t)xml_to_int(unload->Attribute("Type"));
    uint32_t destination = (uint32_t)xml_to_int(unload->Attribute("Destination"));
    uint32_t quantity = (uint32_t)xml_to_int(unload->Attribute("Quantity"));

    return queue->AddOrder(Order::BaseOrder(order_id, Order::ORDER_TYPE_UNLOAD, quantity, type, destination));
}

bool XMLParser::parse_RequestStores() {
    /*
     * <Current_Stores>
     * <WorkPiece type=”Px” quantity=”XX”/>
     * <WorkPiece type=”Px” quantity=”XX”/>
     * ...
     * </Current_Stores>
     */
    XMLDocument doc;
    XMLElement* request = doc.NewElement("Current_Stores");
    doc.InsertFirstChild(request);
    for (uint8_t i = 1; i <= 9; i++) {
        XMLElement* element = request->InsertNewChildElement("WorkPiece");
        element->SetAttribute("type", ("P" + std::to_string(i)).c_str());
        element->SetAttribute("quantity", ((Warehouse*)warehouse)->GetPieceCount(i));
    }

    XMLPrinter printer;
    doc.Print( &printer );

    ((UDPManager*)udp)->sendData(printer.CStr());
    return true;
}

void XMLParser::handleParsingError() {
    /**
     *  Handle parsing errors. Should send a message back through UDP
     *
     */
    const char* data = "Error in MES!!!\n";
    ((UDPManager*)udp)->sendData(data);
    return;
}

int XMLParser::xml_to_int(const char* xml) {
    const char* in = xml;
    if (xml[0] == 'P' || xml[0] == 'D') {
        in = xml+1;
    }
    std::stringstream strValue;
    int intValue;
    strValue << in;
    strValue >> intValue;
    return intValue;
}