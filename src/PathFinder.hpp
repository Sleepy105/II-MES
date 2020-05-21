#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <list>
#include <iterator>

#include "Order.hpp"
#include "Warehouse.hpp"

namespace PathFinder {
    class PathFinder;

    class BaseModule;
    class Machine;
    class Linear;
    class Rotational;
    class Pusher;

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
protected:
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
    BaseModule() {}
    ~BaseModule() {}

    enum Type {Machine, Linear, Rotational, Pusher};
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
     * @brief Get the direction for that Module
     * 
     * @param module 
     * @return Direction 
     */
    Direction searchDir(BaseModule* module);

    /**
     * @brief Returns if the direction is an upstream
     * 
     * @param dir 
     * @return true 
     * @return false 
     */
    bool isUpstream(Direction dir);

    /**
     * @brief Check if this Module can handle a part of this type
     * 
     * @param part_type 
     * @return true 
     * @return false 
     */
    bool canHandlePart(uint8_t part_type);

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

class PathFinder::Linear : public BaseModule {
protected:
    const uint32_t Receive = 1;
public:
    Linear() { type = Type::Linear; }
    ~Linear() {}

    /**
     * @brief Calculate time that this Linear Conveyor will take to handle a part of this type.
     * 
     * @param order
     * @param part_type 
     * @return uint32_t 
     */
    uint32_t calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type);
};

class PathFinder::Machine : public Linear {
protected:
    Warehouse* warehouse;

    const uint32_t ToolChange = 30;

    enum OperationType {ChangeTools=1, PartTransformation};
    
    typedef struct {
        OperationType type;
        Transformation* transformation = NULL;
    } Operation;

    std::list<Operation*> operation_queue;

    /**
     * @brief Get the Transformation that makes parts of this type
     * 
     * @param part_type 
     * @return Transformation* 
     */
    Transformation* getTransformationThatMakesPart(uint8_t part_type);
public:
    Machine(Warehouse* warehouse) : warehouse(warehouse) { type = Type::Machine; }
    ~Machine() {}

    /**
     * @brief Check if this Module can handle a part of this type
     * 
     * @param part_type 
     * @return true 
     * @return false 
     */
    bool canHandlePart(uint8_t part_type);

    /**
     * @brief Calculate time that this Machine will take to handle a part of this type.
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

class PathFinder::Rotational : public Linear {
protected:
    const uint32_t Rotate = 1;
public:
    Rotational() { type = Type::Rotational; }
    ~Rotational() {}

    /**
     * @brief Calculate time that this Rotational Conveyor will take to handle a part of this type.
     * 
     * @param order
     * @param part_type 
     * @return uint32_t 
     */
    uint32_t calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type);
};

class PathFinder::Pusher : public Linear {
private:
    /* data */
public:
    Pusher() { type = Type::Pusher; }
    ~Pusher() {}
};



class PathFinder::PathFinder{
private:
    Warehouse* warehouse;
    BaseModule* cells[60] = {NULL};
public:
    PathFinder(Warehouse* warehouse);
    ~PathFinder() {}
    /**
     * @brief Search for optimum path for a part of this order
     * 
     * @param order Order to consider
     * @return Path* Optimum Path or NULL if no path can be found
     */
    Path* FindPath (Order::BaseOrder& order);
};

#endif
