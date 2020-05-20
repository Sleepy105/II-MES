#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <string>
#include <map> 

#include "cppGraph/Graph.h"

#include "Order.hpp"

using namespace std;

typedef struct {
  uint8_t path[59] = {0};
  uint8_t transformation[12] = {0};
  uint8_t machine_transformations[9] = {0};
} info_path;

typedef map<std::string,int> mapT;

class PathFinder{
private:
    info_path Info[59];
    mapT LUT;    
    
public:
    PathFinder();

    /**
     * @brief Search for optimum path for a part of this order
     * 
     * @param order Order to consider
     * @return Path* Optimum Path or NULL if no path can be found
     */
    Path* FindPath (Order::BaseOrder& order);
};

#endif
