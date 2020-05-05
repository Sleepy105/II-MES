/**
 * @file XMLParser.hpp
 * @brief 
 * @version 0.1
 * @date 2020-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _XMLPARSER_HPP_
#define _XMLPARSER_HPP_

#include <iostream>
#include <sstream>
#include "tinyxml2/tinyxml2.h"

#include "Order.hpp"
#include "OrderQueue.hpp"


class XMLParser {
private:
    OrderQueue* queue = NULL;

    static int xml_to_int(const char* xml);
    static uint32_t xml_to_uint32(const char* xml);

    bool parse_Transformation(uint8_t order_id, tinyxml2::XMLElement* transform);
    bool parse_Unload(uint8_t order_id, tinyxml2::XMLElement* unload);
    bool parse_RequestStores(uint8_t order_id, tinyxml2::XMLElement* request_stores);
    void handleParsingError();
public:
    XMLParser(OrderQueue* obj);
    ~XMLParser();

    void parseString(std::string str);
};

#endif
