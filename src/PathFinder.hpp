#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include "Order.hpp"

typedef uint8_t* Path;

class PathFinder{
public:
    Path FindPath (Order::BaseOrder& order);
};

#endif
