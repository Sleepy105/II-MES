#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <list>
#include <iterator>

#include "Order.hpp"

namespace PathFinder {
    class PathFinder;

    class BaseModule;
    class Machine;
    class Linear;
    class Rotational;
    class Slider;

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

    enum Direction { Right=1, Down, Left, Up };
    
};


class PathFinder::BaseModule {
private:
    BaseModule* modules[4] = {NULL};
    bool upstreams[4] = {false};
    std::list<Transformation*> valid_transformations;

    uint32_t time_so_far;
    
    /**
     * @brief Search all valid upstream modules for best path
     * 
     * @param time_so_far 
     * @return ModulePath* 
     */
    ModulePath* searchUpstream(Order::BaseOrder& order, uint8_t part_type, uint32_t time_so_far, ModulePath* best_so_far);

public:
    BaseModule();
    ~BaseModule();

    enum Type {Machine, Linear, Rotational, Slider};
    Type type;

    /**
     * @brief Evaluate self (and upstream)
     * 
     * @param time_so_far 
     * @return ModulePath* 
     */
    ModulePath* search(Order::BaseOrder& order, uint8_t part_type, uint32_t time_so_far, ModulePath* best_so_far);
    
    /**
     * @brief Check if module can do a certain transformation of parts
     * 
     * @param t 
     * @return true 
     * @return false 
     */
    bool canDoTransformation(Transformation* t);

    /**
     * @brief Set that it can do the Transformation t
     * 
     * @param t 
     */
    void addCanDoTransformation(Transformation* t);

    /**
     * @brief Set the Module present in Dir direction
     * 
     * @param dir 
     * @param module 
     * @param upstream Is this direction considered to be an upstream direction
     */
    void setDir(Direction dir, BaseModule* module, bool upstream);

    /**
     * @brief Get the Module for that direction
     * 
     * @param dir 
     * @return BaseModule* 
     */
    BaseModule* getDir(Direction dir);

    /**
     * @brief Returns if the direction is an upstream
     * 
     * @param dir 
     * @return true 
     * @return false 
     */
    bool isUpstream(Direction dir);

    /**
     * @brief Calculate time that this module will take to handle a part of this type.
     * Always returns 0. To be implemented in subclass
     * 
     * @param order
     * @param part_type 
     * @return uint32_t 
     */
    uint32_t calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type);

    /**
     * @brief Does nothing. Subclasses can implement this to change part_type, if needed, mid search().
     * 
     * @param part_type 
     * @return uint8_t Returns part_type
     */
    uint8_t changeType(uint8_t part_type);
};

class PathFinder::Machine : BaseModule {
private:
    /* data */
public:
    Machine();
    ~Machine();
};

class PathFinder::Linear : BaseModule {
private:
    /* data */
public:
    Linear();
    ~Linear();
};

class PathFinder::Rotational : BaseModule {
private:
    /* data */
public:
    Rotational();
    ~Rotational();
};

class PathFinder::Slider : BaseModule {
private:
    /* data */
public:
    Slider();
    ~Slider();
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
