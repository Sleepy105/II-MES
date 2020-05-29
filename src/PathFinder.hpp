#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <string>
#include <map> 
#include <list>
#include <iterator>

#include "cppGraph/Graph.h"

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

    enum Direction { Stop, Right, Down, Left, Up };
    
};


class PathFinder::BaseModule {
protected:
    BaseModule* modules[5] = {NULL};
    bool downstreams[5] = {false};
    std::list<Transformation*> valid_transformations;
public:
    BaseModule() {}
    ~BaseModule() {}

    enum Type {Machine, Linear, Rotational, Pusher};
    Type type;

    /**
     * @brief Check if module can do a certain transformation of parts
     * 
     * @param t 
     * @return true 
     * @return false 
     */
    bool canDoTransformation(Transformation& t);

    /**
     * @brief Set that it can do the Transformation t
     * 
     * @param t 
     */
    void addCanDoTransformation(Transformation& t);

    /**
     * @brief Set the Module present in Dir direction
     * 
     * @param dir 
     * @param module 
     * @param downstream Is this direction considered to be an downstream direction
     */
    void setDir(Direction dir, BaseModule* module, bool downstream);

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
     * @brief Returns if the direction is an downstream
     * 
     * @param dir 
     * @return true 
     * @return false 
     */
    bool isDownstream(Direction dir);

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
};

class PathFinder::Machine : public BaseModule {
protected:
    Warehouse* warehouse;

    const uint32_t Receive = 1;
    const uint32_t ToolChange = 30;

    uint8_t current_tool = 1;

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
    uint32_t calcTimeToHandleTransformation(Order::BaseOrder& order, Transformation& transformation);
};

class PathFinder::Pusher : public BaseModule {
private:
    /* data */
public:
    Pusher() { type = Type::Pusher; }
    ~Pusher() {}
};



class PathFinder::PathFinder{
private:
    Warehouse* warehouse;
    BaseModule* machines[9] = {NULL};
    Transformation* transformations[13] = {NULL};

    ModulePath* searchMachines(std::list<Transformation> list);
public:
    PathFinder(Warehouse* warehouse);
    ~PathFinder() {}

    enum Block {A1, A2, A3, B1, B2, B3, C1, C2, C3};

    /**
     * @brief Search for optimum path for a part of this order
     * 
     * @param order Order to consider
     * @return Path* Optimum Path or NULL if no path can be found
     */
    Path* FindPath (Order::BaseOrder& order);
};

#endif
