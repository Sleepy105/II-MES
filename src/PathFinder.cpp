#include "PathFinder.hpp"

#include <stdio.h>

#include "OPC-UA.hpp"

PathFinder::Transformation T1 = {
    .from   = 1,
    .to     = 2,
    .tool   = 1,
    .time   = 15,
};

PathFinder::Transformation T2 = {
    .from   = 2,
    .to     = 3,
    .tool   = 1,
    .time   = 15,
};

PathFinder::Transformation T3 = {
    .from   = 2,
    .to     = 6,
    .tool   = 2,
    .time   = 15,
};

PathFinder::Transformation T4 = {
    .from   = 6,
    .to     = 9,
    .tool   = 3,
    .time   = 15,
};

PathFinder::Transformation T5 = {
    .from   = 1,
    .to     = 3,
    .tool   = 1,
    .time   = 20,
};

PathFinder::Transformation T6 = {
    .from   = 3,
    .to     = 4,
    .tool   = 1,
    .time   = 15,
};

PathFinder::Transformation T7 = {
    .from   = 3,
    .to     = 7,
    .tool   = 2,
    .time   = 20,
};

PathFinder::Transformation T8 = {
    .from   = 7,
    .to     = 9,
    .tool   = 3,
    .time   = 20,
};

PathFinder::Transformation T9 = {
    .from   = 1,
    .to     = 4,
    .tool   = 1,
    .time   = 10,
};

PathFinder::Transformation T10 = {
    .from   = 4,
    .to     = 5,
    .tool   = 1,
    .time   = 30,
};

PathFinder::Transformation T11 = {
    .from   = 4,
    .to     = 8,
    .tool   = 2,
    .time   = 10,
};

PathFinder::Transformation T12 = {
    .from   = 8,
    .to     = 9,
    .tool   = 3,
    .time   = 10,
};

bool PathFinder::Machine::canDoTransformation(Transformation& t) {
    for (TransformationsPath::iterator iter = valid_transformations.begin();
            iter != valid_transformations.end();
            iter++)
    {
        if ((*iter) == &t) {
            return true;
        }
    }
    return false;
}

void PathFinder::Machine::addCanDoTransformation(Transformation& t) {
    if (canDoTransformation(t)) {
        return;
    }
    valid_transformations.push_back(&t);
}

void PathFinder::Machine::setDir(Direction dir, Machine* module, bool downstream, MovesPath& moves) {
    modules[dir] = module;
    downstreams[dir] = downstream;
    dir_moves[dir].clear();
    for (auto moves_iter = moves.begin(); moves_iter != moves.end(); moves_iter++) {
        dir_moves[dir].push_back(*moves_iter);
    }
}

PathFinder::Machine* PathFinder::Machine::getDir(Direction dir) {
    return modules[dir];
}

PathFinder::Direction PathFinder::Machine::searchDir(Machine* module) {
    for ( int dirInt = Direction::Stop; dirInt <= Direction::Up; dirInt++ ) {
        Direction dir = static_cast<Direction>(dirInt);
        if (getDir(dir) == module) {
            return dir;
        }
    }
    meslog(ERROR) << "Direction not found!" << std::endl;
    throw "Someone screwed up in this joint!";
}

bool PathFinder::Machine::isDownstream(Direction dir) {
    return downstreams[dir];
}

PathFinder::Transformation* PathFinder::Machine::getTransformationThatMakesPart(uint8_t part_type) {
    for (TransformationsPath::iterator iter = valid_transformations.begin();
            iter != valid_transformations.end();
            iter++)
    {
        if ((*iter)->to == part_type) {
            return (*iter);
        }
    }
    return NULL;
}

bool PathFinder::Machine::canHandlePart(uint8_t part_type) {
    for (TransformationsPath::iterator iter = valid_transformations.begin();
            iter != valid_transformations.end();
            iter++)
    {
        if ((*iter)->from == part_type) {
            return true;
        }
    }
    return false;
}

uint32_t PathFinder::Machine::calcTimeToHandleTransformation(Order::BaseOrder& order, Transformation& transformation) {
    uint32_t handle_time = 0;

    for (auto iter = operation_queue.begin();
            iter != operation_queue.end();
            iter++)
    {
        Operation* operation = *iter;
        /*if (operation->type == ChangeTools) {
            handle_time += ToolChange;
        }
        else*/ 
        if (operation->type == OperationType::PartTransformation) {
            handle_time += operation->transformation->time;
        }
    }
    
    if (requiresToolChange(transformation)) {
        // Divide tool change time by number of parts in the order
        // Take into account number of available parts in the warehouse
        uint32_t order_part_count = order.GetCount();
        uint32_t available_part_count = warehouse->GetPieceCount(order.GetInitialPiece());
        handle_time += ToolChange/(available_part_count < order_part_count ? available_part_count : order_part_count );
    }

    handle_time += transformation.time;

    return handle_time;
}

PathFinder::ModulePath* PathFinder::Machine::search(Order::BaseOrder& order, TransformationsPath::iterator t_iter, TransformationsPath::iterator last) {
    uint32_t self_time = calcTimeToHandleTransformation(order, **t_iter);

    if (++t_iter == last) {
        ModulePath* path = new ModulePath;
        path->path.push_back(this);
        path->time = self_time;
        return path;
    };

    ModulePath* best_path = NULL;
    for ( int dirInt = Direction::Stop; dirInt <= Direction::Up; dirInt++ ) {
        Direction dir = static_cast<Direction>(dirInt);
        if (!isDownstream(dir)) continue;
        if (!getDir(dir)->canDoTransformation(**t_iter)) continue;

        ModulePath* path = getDir(dir)->search(order, t_iter, last);
        if (!best_path) {
            best_path = path;
            continue;
        }

        for (auto m_iter = path->path.begin(); m_iter != path->path.end(); m_iter++) {
            if (((*m_iter) == this) && requiresToolChange(**t_iter)) {
                uint32_t order_part_count = order.GetCount();
                uint32_t available_part_count = warehouse->GetPieceCount(order.GetInitialPiece());
                path->time -= ToolChange/(available_part_count < order_part_count ? available_part_count : order_part_count );
                path->time += ToolChange;
            }
        }

        if (path && path->time <= best_path->time) {
            delete(best_path);
            best_path = path;
        }
    }

    if (best_path) {
        best_path->path.push_front(this);
        best_path->time += self_time;
    }

    return best_path;
}

PathFinder::Cell PathFinder::Machine::getCell() {
    return cell;
}

PathFinder::Row PathFinder::Machine::getRow() {
    return row;
}

PathFinder::MovesPath& PathFinder::Machine::getDirMoves(Direction dir) {
    return dir_moves[dir];
}

void PathFinder::Machine::addOperation(Operation* op) {
    operation_queue.push_back(op);
}

bool PathFinder::Machine::requiresToolChange(Transformation& t) {
    return (current_tool != t.tool);
}

void PathFinder::Machine::setTool(uint8_t tool) {
    current_tool = tool;
}

void PathFinder::Machine::removeOperation() {
    operation_queue.pop_front();
}

uint PathFinder::Machine::queuedOperations() {
    return operation_queue.size();
}

void PathFinder::Pusher::setOPCpointer(void* ptr) {
    opc = ptr;
}

bool PathFinder::Pusher::isSpaceAvailable() {
    meslog(INFO) << "Pusher #" << row << " has " << ((OPCUA_Manager*)opc)->GetPieceAllocInPusher(row) << " pieces in it." << std::endl;
    return ((*(OPCUA_Manager*)opc).GetPieceAllocInPusher(row) < 4);
}

PathFinder::PathFinder::PathFinder(Warehouse* warehouse, void* opc) : warehouse(warehouse), opc(opc) {

    transformations[1] = &T1;
    transformations[2] = &T2;
    transformations[3] = &T3;
    transformations[4] = &T4;
    transformations[5] = &T5;
    transformations[6] = &T6;
    transformations[7] = &T7;
    transformations[8] = &T8;
    transformations[9] = &T9;
    transformations[10] = &T10;
    transformations[11] = &T11;
    transformations[12] = &T12;

    machines[A1] = new Machine(Cell::C1, Row::R1, warehouse);
    machines[A1]->addCanDoTransformation(T1);
    machines[A1]->addCanDoTransformation(T2);
    machines[A1]->addCanDoTransformation(T3);
    machines[A1]->addCanDoTransformation(T4);

    machines[A2] = new Machine(Cell::C2, Row::R1, warehouse);
    machines[A2]->addCanDoTransformation(T1);
    machines[A2]->addCanDoTransformation(T2);
    machines[A2]->addCanDoTransformation(T3);
    machines[A2]->addCanDoTransformation(T4);

    machines[A3] = new Machine(Cell::C3, Row::R1, warehouse);
    machines[A3]->addCanDoTransformation(T1);
    machines[A3]->addCanDoTransformation(T2);
    machines[A3]->addCanDoTransformation(T3);
    machines[A3]->addCanDoTransformation(T4);

    machines[B1] = new Machine(Cell::C1, Row::R2, warehouse);
    machines[B1]->addCanDoTransformation(T5);
    machines[B1]->addCanDoTransformation(T6);
    machines[B1]->addCanDoTransformation(T7);
    machines[B1]->addCanDoTransformation(T8);

    machines[B2] = new Machine(Cell::C2, Row::R2, warehouse);
    machines[B2]->addCanDoTransformation(T5);
    machines[B2]->addCanDoTransformation(T6);
    machines[B2]->addCanDoTransformation(T7);
    machines[B2]->addCanDoTransformation(T8);

    machines[B3] = new Machine(Cell::C3, Row::R2, warehouse);
    machines[B3]->addCanDoTransformation(T5);
    machines[B3]->addCanDoTransformation(T6);
    machines[B3]->addCanDoTransformation(T7);
    machines[B3]->addCanDoTransformation(T8);

    machines[C1] = new Machine(Cell::C1, Row::R3, warehouse);
    machines[C1]->addCanDoTransformation(T9);
    machines[C1]->addCanDoTransformation(T10);
    machines[C1]->addCanDoTransformation(T11);
    machines[C1]->addCanDoTransformation(T12);

    machines[C2] = new Machine(Cell::C2, Row::R3, warehouse);
    machines[C2]->addCanDoTransformation(T9);
    machines[C2]->addCanDoTransformation(T10);
    machines[C2]->addCanDoTransformation(T11);
    machines[C2]->addCanDoTransformation(T12);

    machines[C3] = new Machine(Cell::C3, Row::R3, warehouse);
    machines[C3]->addCanDoTransformation(T9);
    machines[C3]->addCanDoTransformation(T10);
    machines[C3]->addCanDoTransformation(T11);
    machines[C3]->addCanDoTransformation(T12);

    MovesPath move_across;
    repeat(2) move_across.push_back(Direction::Right);

    MovesPath move_down1;
    move_down1.push_back(Direction::Right);
    move_down1.push_back(Direction::Down);
    move_down1.push_back(Direction::Left);
    
    machines[A1]->setDir(Direction::Right, machines[A2], true, move_across);
    machines[A1]->setDir(Direction::Down, machines[B1], true, move_down1);
    machines[A2]->setDir(Direction::Right, machines[A3], true, move_across);
    machines[A2]->setDir(Direction::Down, machines[B2], true, move_down1);
    machines[A3]->setDir(Direction::Down, machines[B3], true, move_down1);

    machines[B1]->setDir(Direction::Right, machines[B2], true, move_across);
    machines[B1]->setDir(Direction::Down, machines[C1], true, move_down1);
    machines[B2]->setDir(Direction::Right, machines[B3], true, move_across);
    machines[B2]->setDir(Direction::Down, machines[C2], true, move_down1);
    machines[B3]->setDir(Direction::Down, machines[C3], true, move_down1);

    machines[C1]->setDir(Direction::Right, machines[C2], true, move_across);
    machines[C2]->setDir(Direction::Right, machines[C3], true, move_across);

    pushers[P1] = new Pusher(opc, Cell::C4, Row::R1);
    pushers[P2] = new Pusher(opc, Cell::C4, Row::R2);
    pushers[P3] = new Pusher(opc, Cell::C4, Row::R3);
}

Path* PathFinder::PathFinder::FindPath(Order::BaseOrder &order) {
    Path* path = new Path;
    ModulePath* best_module_path = NULL;
    TransformationsPath* best_transformations_path = NULL;
    uint16_t move_counter = 0;

    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order.GetType() == Order::ORDER_TYPE_TRANSFORMATION) {
        //std::cout << std::to_string(order.GetInitialPiece()) << " " << std::to_string(order.GetFinalPiece()) << std::endl;

        for ( int blockInt = Block::A1; blockInt <= Block::C3; blockInt++ ) {
            Block block = static_cast<Block>(blockInt);
            std::cout << "Machine C" << machines[block]->getCell() << " R" << machines[block]->getRow() << " has " << machines[block]->queuedOperations() << " operations queued." << std::endl;
        }

        if (1 == order.GetInitialPiece() && 2 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 3 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T5);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T6);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T9);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T10);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T10);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T9);
            transformation_path.push_back(&T10);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 6 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T3);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T7);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T7);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T9);
            transformation_path.push_back(&T11);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T1);
            transformation_path.push_back(&T3);
            transformation_path.push_back(&T4);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T7);
            transformation_path.push_back(&T8);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T7);
            transformation_path.push_back(&T8);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T1);
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T5);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T9);
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 3 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T2);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T10);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 6 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T3);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T7);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T3);
            transformation_path.push_back(&T4);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T2);
            transformation_path.push_back(&T7);
            transformation_path.push_back(&T8);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();

            transformation_path.push_back(&T2);
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            // delete(module_path);
            module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T6);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T10);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T7);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T7);
            transformation_path.push_back(&T8);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();

            transformation_path.push_back(&T6);
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            ModulePath* module_path = searchMachines(order, transformation_path);
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
                delete(best_transformations_path);
                best_transformations_path = copyTransformationsPath(transformation_path);
            }
            transformation_path.clear();
        }
        else if (4 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T10);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (4 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T11);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (4 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T11);
            transformation_path.push_back(&T12);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (6 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T4);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (7 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T8);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }
        else if (8 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            TransformationsPath transformation_path;
            transformation_path.push_back(&T12);
            best_module_path = searchMachines(order, transformation_path);
            best_transformations_path = copyTransformationsPath(transformation_path);
            transformation_path.clear();
        }



        // Check if there is a valid path
        if (!best_module_path) {
            delete(best_transformations_path);
            delete(path);
            return NULL;
        }

        Cell start_cell = best_module_path->path.front()->getCell();
        Cell end_cell = best_module_path->path.back()->getCell();

        repeat(start_cell) {
            repeat(2) path->moves[move_counter++] = Direction::Right;
        }
        repeat(best_module_path->path.front()->getRow()+1) path->moves[move_counter++] = Direction::Down;
        path->moves[move_counter++] = Direction::Left;

        auto iter = best_module_path->path.begin();
        Machine* curr_machine = *iter;
        for (iter++; iter != best_module_path->path.end(); curr_machine = *iter, iter++) {
            MovesPath& machine_moves = curr_machine->getDirMoves(curr_machine->searchDir(*iter));

            for (auto moves_iter = machine_moves.begin(); moves_iter != machine_moves.end(); moves_iter++) {
                path->moves[move_counter++] = *moves_iter;
            }

        }

        auto m_iter = best_module_path->path.begin();
        for (auto t_iter = best_transformations_path->begin(); m_iter != best_module_path->path.end(); t_iter++, m_iter++) {
            for ( int blockInt = Block::A1; blockInt <= Block::C3; blockInt++ ) {
                Block block = static_cast<Block>(blockInt);
                if (machines[block] == *m_iter) {
                    (path->machine_transformations[block])++;

                    for (int i = 1; i <= 12; i++) {
                        if (transformations[i] == *t_iter) {
                            (path->transformations[i-1])++;

                            // Add operation to machine queue
                            Machine::Operation* op = new Machine::Operation;
                            op->type = Machine::OperationType::PartTransformation;
                            op->transformation = transformations[i];
                            machines[block]->addOperation(op);

                            if (machines[block]->requiresToolChange(*(transformations[i]))) {
                                machines[block]->setTool(transformations[i]->tool);
                            }
                        }
                    }
                }
            }
        }

        path->moves[move_counter++] = Direction::Right;
        repeat(5-best_module_path->path.back()->getRow()) path->moves[move_counter++] = Direction::Down;
        repeat(end_cell) {
            repeat(2) path->moves[move_counter++] = Direction::Left;
        }

        path->moves[move_counter++] = Direction::Stop;

        /* DEBUG code */
        /*std::cout << "MACHINES: " << std::endl;
        std::cout << "\tA1: " << machines[A1] << std::endl;
        std::cout << "\tA2: " << machines[A2] << std::endl;
        std::cout << "\tA3: " << machines[A3] << std::endl;
        std::cout << "\tB1: " << machines[B1] << std::endl;
        std::cout << "\tB2: " << machines[B2] << std::endl;
        std::cout << "\tB3: " << machines[B3] << std::endl;
        std::cout << "\tC1: " << machines[C1] << std::endl;
        std::cout << "\tC2: " << machines[C2] << std::endl;
        std::cout << "\tC3: " << machines[C3] << std::endl;
        for (auto iter = best_module_path->path.begin(); iter != best_module_path->path.end(); iter++) {
            std::cout << "Machine: " << *iter << std::endl;
        }
        for (int i = 0; i < move_counter; i++) {
            std::cout << std::to_string(path->moves[i]) << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 9; i++) {
            std::cout << std::to_string(path->machine_transformations[i]) << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < 12; i++) {
            std::cout << std::to_string(path->transformations[i]) << " ";
        }
        std::cout << std::endl;*/
        /*delete(best_module_path);
        delete(best_transformations_path);
        delete(path);
        return NULL;*/
        /**************/
    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order.GetType() == Order::ORDER_TYPE_UNLOAD) {

        if (!pushers[order.GetFinalPiece()-1]->isSpaceAvailable()) {
            delete(path);
            return NULL;
        }

        repeat(7) path->moves[move_counter++] = Direction::Right;
        repeat(order.GetFinalPiece()+1) path->moves[move_counter++] = Direction::Down;
        path->moves[move_counter++] = Direction::Right;
        path->moves[move_counter++] = Direction::Stop;
    }

    return path;
}

PathFinder::ModulePath* PathFinder::PathFinder::searchMachines(Order::BaseOrder& order, TransformationsPath& list) {
    // Calculate best time for each Transformation
    ModulePath* path = NULL;
    for ( int machine = Block::A1; machine != Block::C3; machine++) {
        if (!machines[machine]->canDoTransformation(*list.front())) {
            continue;
        }

        bool tagged = false;
        for ( int tester = Block::A1; tester != Block::C3; tester++) {
            if (machines[tester]->getCell() == machines[machine]->getCell()) {
                if (machines[tester]->queuedOperations() > 1) {
                    // Do not use this cell as start cell
                    tagged = true;
                    break;
                }
            }
        }
        if (tagged) {
            continue;
        }

        ModulePath* machine_path = machines[machine]->search(order, list.begin(), list.end());
        if (!path) {
            path = machine_path;
        }
        else if (machine_path && machine_path->time < path->time) {
            delete(path);
            path = machine_path;
        }
    }

    return path;
}

void PathFinder::PathFinder::setOPCpointer(void* ptr) {
    opc = ptr;
    for ( int blockInt = Block::P1; blockInt <= Block::P3; blockInt++ ) {
            Block block = static_cast<Block>(blockInt);
            pushers[blockInt]->setOPCpointer(opc);
    }
}

PathFinder::TransformationsPath* PathFinder::copyTransformationsPath(TransformationsPath& path) {
    TransformationsPath* new_path = new TransformationsPath;

    for (auto iter = path.begin(); iter != path.end(); iter++) {
        new_path->push_back(*iter);
    }

    return new_path;
}

void PathFinder::PathFinder::signalTransformationFinished(Cell cell, Row row) {
    uint c = cell+1; // Dirtiest bugfix...
    uint r = row+1;  // ...evahhh
    for ( int blockInt = Block::A1; blockInt <= Block::C3; blockInt++ ) {
            Block block = static_cast<Block>(blockInt);
        if (machines[blockInt]->getCell() == c && machines[blockInt]->getRow() == r) {
            machines[blockInt]->removeOperation();
        }
    }
}

