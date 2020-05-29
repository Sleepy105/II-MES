#include "PathFinder.hpp"
#include <stdio.h>

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
    for (std::list<Transformation*>::iterator iter = valid_transformations.begin();
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

void PathFinder::Machine::setDir(Direction dir, Machine* module, bool downstream) {
    modules[dir] = module;
    downstreams[dir] = downstream;
}

PathFinder::Machine* PathFinder::Machine::getDir(Direction dir) {
    return modules[dir];
}

PathFinder::Direction PathFinder::Machine::searchDir(Machine* module) {
    for ( const auto dir : { Direction::Right, Direction::Up } ) {
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

uint32_t PathFinder::Machine::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return 0;
}

PathFinder::Transformation* PathFinder::Machine::getTransformationThatMakesPart(uint8_t part_type) {
    for (std::list<Transformation*>::iterator iter = valid_transformations.begin();
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
    for (std::list<Transformation*>::iterator iter = valid_transformations.begin();
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
    uint32_t handle_time = Receive;

    /*for (std::list<Operation*>::iterator iter = operation_queue.begin();
            iter != operation_queue.end();
            iter++)
    {
        Operation* operation = (*iter);
        if (operation->type == ChangeTools) {
            handle_time += ToolChange;
        }
        else if (operation->type == PartTransformation) {
            handle_time += operation->transformation->time;
        }
    }*/ // TODO Review this whole thing

    bool requiresToolChange = (current_tool != transformation.tool);
    
    if (requiresToolChange) {
        // Divide tool change time by number of parts in the order
        // Take into account number of available parts in the warehouse
        uint32_t order_part_count = order.GetCount();
        uint32_t available_part_count = warehouse->GetPieceCount(order.GetInitialPiece());
        handle_time += ToolChange/(available_part_count < order_part_count ? available_part_count : order_part_count );

        // TODO Add tool change to operation queue
        current_tool = transformation.tool;
    }

    handle_time += transformation.time;

    return handle_time;
}

PathFinder::ModulePath* PathFinder::Machine::search(Order::BaseOrder& order, std::list<Transformation>::iterator t_iter, std::list<Transformation>::iterator last) {
    uint32_t self_time = calcTimeToHandleTransformation(order, *t_iter);

    if (t_iter == last) {
        ModulePath* path = new ModulePath;
        path->path.push_back(this);
        path->time = self_time;
        return path;
    };

    ModulePath* best_path = NULL;
    for ( const auto dir : { Direction::Right, Direction::Up } ) {
        if (!isDownstream(dir)) continue;
        if (!canDoTransformation(*t_iter)) continue;

        t_iter++;

        ModulePath* path = getDir(dir)->search(order, t_iter, last);
        if (!best_path) {
            best_path = path;
        }
        else if (path && path->time < best_path->time) {
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

PathFinder::PathFinder::PathFinder(Warehouse* warehouse) : warehouse(warehouse) {

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

    machines[A1] = new Machine(warehouse);
    machines[A1]->canDoTransformation(T1);
    machines[A1]->canDoTransformation(T2);
    machines[A1]->canDoTransformation(T3);
    machines[A1]->canDoTransformation(T4);

    machines[A2] = new Machine(warehouse);
    machines[A2]->canDoTransformation(T1);
    machines[A2]->canDoTransformation(T2);
    machines[A2]->canDoTransformation(T3);
    machines[A2]->canDoTransformation(T4);

    machines[A3] = new Machine(warehouse);
    machines[A3]->canDoTransformation(T1);
    machines[A3]->canDoTransformation(T2);
    machines[A3]->canDoTransformation(T3);
    machines[A3]->canDoTransformation(T4);

    machines[B1] = new Machine(warehouse);
    machines[B1]->canDoTransformation(T5);
    machines[B1]->canDoTransformation(T6);
    machines[B1]->canDoTransformation(T7);
    machines[B1]->canDoTransformation(T8);

    machines[B2] = new Machine(warehouse);
    machines[B2]->canDoTransformation(T5);
    machines[B2]->canDoTransformation(T6);
    machines[B2]->canDoTransformation(T7);
    machines[B2]->canDoTransformation(T8);

    machines[B3] = new Machine(warehouse);
    machines[B3]->canDoTransformation(T5);
    machines[B3]->canDoTransformation(T6);
    machines[B3]->canDoTransformation(T7);
    machines[B3]->canDoTransformation(T8);

    machines[C1] = new Machine(warehouse);
    machines[C1]->canDoTransformation(T9);
    machines[C1]->canDoTransformation(T10);
    machines[C1]->canDoTransformation(T11);
    machines[C1]->canDoTransformation(T12);

    machines[C2] = new Machine(warehouse);
    machines[C2]->canDoTransformation(T9);
    machines[C2]->canDoTransformation(T10);
    machines[C2]->canDoTransformation(T11);
    machines[C2]->canDoTransformation(T12);

    machines[C3] = new Machine(warehouse);
    machines[C3]->canDoTransformation(T9);
    machines[C3]->canDoTransformation(T10);
    machines[C3]->canDoTransformation(T11);
    machines[C3]->canDoTransformation(T12);
    
    machines[A1]->setDir(Direction::Right, machines[A2], true);
    machines[A1]->setDir(Direction::Down, machines[B1], true);
    machines[A2]->setDir(Direction::Right, machines[A3], true);
    machines[A2]->setDir(Direction::Down, machines[B2], true);
    machines[A3]->setDir(Direction::Down, machines[B3], true);

    machines[B1]->setDir(Direction::Right, machines[B2], true);
    machines[B1]->setDir(Direction::Down, machines[C1], true);
    machines[B2]->setDir(Direction::Right, machines[B3], true);
    machines[B2]->setDir(Direction::Down, machines[C2], true);
    machines[B3]->setDir(Direction::Down, machines[C3], true);

    machines[C1]->setDir(Direction::Right, machines[C2], true);
    machines[C2]->setDir(Direction::Right, machines[C3], true);
}

Path* PathFinder::PathFinder::FindPath(Order::BaseOrder &order) {
    Path* path = new Path;
    ModulePath* best_module_path = NULL;

    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order.GetType() == Order::ORDER_TYPE_TRANSFORMATION) {
        std::cout << std::to_string(order.GetInitialPiece()) << " " << std::to_string(order.GetFinalPiece()) << std::endl;

        if (1 == order.GetInitialPiece() && 2 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 3 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T5);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (1 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T5);
            transformation_path.push_back(T6);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T9);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (1 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T10);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T5);
            transformation_path.push_back(T6);
            transformation_path.push_back(T10);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T9);
            transformation_path.push_back(T10);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (1 == order.GetInitialPiece() && 6 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T3);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (1 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T7);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T5);
            transformation_path.push_back(T7);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (1 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T5);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T9);
            transformation_path.push_back(T11);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (1 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T1);
            transformation_path.push_back(T3);
            transformation_path.push_back(T4);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T7);
            transformation_path.push_back(T8);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T5);
            transformation_path.push_back(T7);
            transformation_path.push_back(T8);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T1);
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T5);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T9);
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (2 == order.GetInitialPiece() && 3 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T2);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T10);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 6 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T3);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T2);
            transformation_path.push_back(T7);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (2 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T3);
            transformation_path.push_back(T4);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T2);
            transformation_path.push_back(T7);
            transformation_path.push_back(T8);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }

            transformation_path.push_back(T2);
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            delete(module_path);
            module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (3 == order.GetInitialPiece() && 4 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T6);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T6);
            transformation_path.push_back(T10);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 7 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T7);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (3 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T7);
            transformation_path.push_back(T8);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();

            transformation_path.push_back(T6);
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            ModulePath* module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
            if (module_path && module_path->time < best_module_path->time) {
                delete(best_module_path);
                best_module_path = module_path;
            }
        }
        else if (4 == order.GetInitialPiece() && 5 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T10);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (4 == order.GetInitialPiece() && 8 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T11);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (4 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T11);
            transformation_path.push_back(T12);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (6 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T4);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (7 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T8);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }
        else if (8 == order.GetInitialPiece() && 9 == order.GetFinalPiece()) {
            std::list<Transformation> transformation_path;
            transformation_path.push_back(T12);
            best_module_path = searchMachines(order, transformation_path);
            transformation_path.clear();
        }



        // Check if there is a valid path
        if (!best_module_path) {
            delete(path);
            return path;
        }

        // Find the first transformation to be done to the part
        //std::cout << "HI: " << shortestPath.front() << std::endl;
        int curr_part = 1;//std::stoi(shortestPath.front());

        // TODO Update of free/blocked machines

    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order.GetType() == Order::ORDER_TYPE_UNLOAD) {
        // TODO
    }

    return path;
}

PathFinder::ModulePath* PathFinder::PathFinder::searchMachines(Order::BaseOrder& order, std::list<Transformation> list) {
    // Calculate best time for each Transformation
    ModulePath* path = NULL;
    for ( const auto machine : { Block::A1, Block::C3 } ) {
        if (!machines[machine]->canDoTransformation(list.front())) {
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
