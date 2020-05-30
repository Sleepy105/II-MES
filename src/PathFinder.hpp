#ifndef _PATHFINDER_HPP_
#define _PATHFINDER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <iterator>

#include "Order.hpp"
#include "Warehouse.hpp"

namespace PathFinder {
    class PathFinder;

    class Machine;
    class Pusher;

    typedef struct {
        std::list<Machine*> path;
        uint32_t time;
    } ModulePath;

    typedef struct {
        uint8_t from;
        uint8_t to;
        uint8_t tool;
        uint8_t time;
    } Transformation;

    enum Direction { Stop, Right, Down, Left, Up };
    enum Cell { C1=1, C2, C3, C4 };
    enum Row { R1=1, R2, R3 };
    
    typedef std::list<Direction> MovesPath;
    typedef std::list<Transformation*> TransformationsPath;

    /**
     * @brief Copy the contents of a path to another.
     * 
     * @param path 
     * @return TransformationsPath* Pointer to new object
     */
    TransformationsPath* copyTransformationsPath(TransformationsPath& path);
};


class PathFinder::Machine {
protected:
    Machine* modules[5] = {NULL};
    bool downstreams[5] = {false};
    MovesPath dir_moves[5];
    TransformationsPath valid_transformations;

    Cell cell;
    Row row;
    Warehouse* warehouse;

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
    Machine(Cell cell, Row row, Warehouse* warehouse) : cell(cell), row(row), warehouse(warehouse) {
        MovesPath dummy;
        setDir(Direction::Stop, this, true, dummy);
    }
    ~Machine() {}

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
    void setDir(Direction dir, Machine* module, bool downstream, MovesPath& moves);

    /**
     * @brief Get the Module for that direction
     * 
     * @param dir 
     * @return Machine* 
     */
    Machine* getDir(Direction dir);

    /**
     * @brief Get the direction for that Module
     * 
     * @param module 
     * @return Direction 
     */
    Direction searchDir(Machine* module);

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
     * @brief Calculate time that this Machine will take to handle a part of this type.
     * 
     * @param order
     * @param part_type 
     * @return uint32_t 
     */
    uint32_t calcTimeToHandleTransformation(Order::BaseOrder& order, Transformation& transformation);

    ModulePath* search(Order::BaseOrder& order, TransformationsPath::iterator t, TransformationsPath::iterator last);

    Cell getCell();
    
    Row getRow();

    MovesPath& getDirMoves(Direction dir);
};

class PathFinder::Pusher {
private:
    void* opc;
    Cell cell;
    Row row;
public:
    Pusher(void* opc, Cell cell, Row row) : opc(opc), cell(cell), row(row) {}
    ~Pusher() {}

    /**
     * @brief Check if there is room on this pusher
     * 
     * @return true 
     * @return false 
     */
    bool isSpaceAvailable();
};



class PathFinder::PathFinder{
private:
    Warehouse* warehouse;
    void* opc;
    Machine* machines[9] = {NULL};
    Transformation* transformations[13] = {NULL};

    Pusher* pushers[3] = {NULL};

    ModulePath* searchMachines(Order::BaseOrder& order, TransformationsPath& list);
public:
    PathFinder(Warehouse* warehouse, void* opc);
    ~PathFinder() {}

    enum Block {A1, A2, A3, B1, B2, B3, C1, C2, C3, P1=0, P2, P3};

    /**
     * @brief Search for optimum path for a part of this order
     * 
     * @param order Order to consider
     * @return Path* Optimum Path or NULL if no path can be found
     */
    Path* FindPath (Order::BaseOrder& order);
};

#endif
