#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <list>
#include <iterator>

#include "Order.hpp"

namespace PathFinder {
    class PathFinder;

    class BaseModule;
    typedef struct {
        std::list<BaseModule*> path;
        uint32_t time;
    } ModulePath;
    typedef struct {
        uint8_t from;
        uint8_t to;
        uint8_t tool;
        uint8_t time;
    } Transformation;

    enum Direction { Right=1, Down, Left, Up};
    
};


class PathFinder::BaseModule {
private:
    std::list<BaseModule> upstream_modules;
    std::list<Transformation> valid_transformations;

    uint32_t time_so_far;
    ModulePath* _best_path_so_far;
    
    /**
     * @brief Search all valid upstream modules for best path
     * 
     * @param time_so_far 
     * @return ModulePath* 
     */
    ModulePath* searchUpstream(uint32_t time_so_far, uint32_t best_so_far);

public:
    BaseModule();
    ~BaseModule();

    /**
     * @brief Evaluate self (and upstream)
     * 
     * @param time_so_far 
     * @return ModulePath* 
     */
    ModulePath* search(uint32_t time_so_far, uint32_t best_so_far);
    
    /**
     * @brief Check if module can do a certain transformation of parts
     * 
     * @param t 
     * @return true 
     * @return false 
     */
    bool canDoTransformation(Transformation& t);
};


class PathFinder::PathFinder{
public:
    PathFinder();
    ~PathFinder();
    /**
     * @brief Search for optimum path for a part of this order
     * 
     * @param order Order to consider
     * @return Path* Optimum Path or NULL if no path can be found
     */
    Path* FindPath (Order::BaseOrder& order);
};

#endif
