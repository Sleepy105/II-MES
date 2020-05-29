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

PathFinder::ModulePath* PathFinder::BaseModule::search(Order::BaseOrder& order, uint8_t part_type, uint32_t time_so_far, ModulePath* best_so_far) {
    uint32_t self_time = calcTimeToHandlePart(order, part_type);

    part_type = changeType(part_type);

    if (!best_so_far && endpoint) {
        best_so_far = new ModulePath();
        best_so_far->path.push_front(this);
        best_so_far->time += self_time;
        return best_so_far;
    }

    ModulePath* best_path = searchDownstream(order, part_type, time_so_far+self_time, best_so_far);
    best_path->path.push_front(this);
    best_path->time += self_time;
    return best_path;
}

PathFinder::ModulePath* PathFinder::BaseModule::searchDownstream(Order::BaseOrder& order, uint8_t part_type, uint32_t time_so_far, ModulePath* best_so_far) {
    for ( const auto dir : { Direction::Right, Direction::Up } ) {
        if (!isDownstream(dir)) {
            continue;
        }
        
        BaseModule* module = getDir(dir);
        if (!module) {
            continue;
        }

        if (!module->canHandlePart(part_type)) {
            continue;
        }
        
        ModulePath* path = module->search(order, part_type, time_so_far, best_so_far);

        if (!best_so_far) {
            best_so_far = path;
            continue;
        }

        // TODO Even if slower, prioritize paths that use a larger number of machines of the same type, if the order is for more than 1 part

        if (path->time < best_so_far->time) {
            delete(best_so_far);
            best_so_far = path;
            continue;
        }

        delete(path);
    }
    return best_so_far;
}

bool PathFinder::BaseModule::canDoTransformation(Transformation* t) {
    for (std::list<Transformation*>::iterator iter = valid_transformations.begin();
            iter != valid_transformations.end();
            iter++)
    {
        if ((*iter) == t) {
            return true;
        }
    }
    return false;
}

void PathFinder::BaseModule::addCanDoTransformation(Transformation* t) {
    if (canDoTransformation(t)) {
        return;
    }
    valid_transformations.push_back(t);
}

void PathFinder::BaseModule::setDir(Direction dir, BaseModule* module, bool downstream) {
    modules[dir] = module;
    downstreams[dir] = downstream;
}

PathFinder::BaseModule* PathFinder::BaseModule::getDir(Direction dir) {
    return modules[dir];
}

PathFinder::Direction PathFinder::BaseModule::searchDir(BaseModule* module) {
    for ( const auto dir : { Direction::Right, Direction::Up } ) {
        if (getDir(dir) == module) {
            return dir;
        }
    }
    meslog(ERROR) << "Direction not found!" << std::endl;
    throw "Someone screwed up in this joint!";
}

bool PathFinder::BaseModule::isDownstream(Direction dir) {
    return downstreams[dir];
}

bool PathFinder::BaseModule::canHandlePart(uint8_t part_type) {
    return true;
}

uint32_t PathFinder::BaseModule::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return 0;
}

uint8_t PathFinder::BaseModule::changeType(uint8_t part_type) {
    return part_type;
}

uint32_t PathFinder::Linear::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return Receive;
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

uint32_t PathFinder::Machine::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
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

    // TODO Check for tool changes
    bool requiresToolChange = false;
    
    if (requiresToolChange) {
        // Divide tool change time by number of parts in the order
        // Take into account number of available parts in the warehouse
        uint32_t order_part_count = order.GetCount();
        uint32_t available_part_count = warehouse->GetPieceCount(order.GetInitialPiece());
        handle_time += ToolChange/(available_part_count < order_part_count ? available_part_count : order_part_count );
    }

    Transformation* t = getTransformationThatMakesPart(part_type);
    handle_time += t->time;

    return handle_time;
}

uint8_t PathFinder::Machine::changeType(uint8_t part_type) {
    Transformation* t = getTransformationThatMakesPart(part_type);
    return t->to;
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
    machines[A1]->canDoTransformation(&T1);
    machines[A1]->canDoTransformation(&T2);
    machines[A1]->canDoTransformation(&T3);
    machines[A1]->canDoTransformation(&T4);

    machines[A2] = new Machine(warehouse);
    machines[A2]->canDoTransformation(&T1);
    machines[A2]->canDoTransformation(&T2);
    machines[A2]->canDoTransformation(&T3);
    machines[A2]->canDoTransformation(&T4);

    machines[A3] = new Machine(warehouse);
    machines[A3]->canDoTransformation(&T1);
    machines[A3]->canDoTransformation(&T2);
    machines[A3]->canDoTransformation(&T3);
    machines[A3]->canDoTransformation(&T4);

    machines[B1] = new Machine(warehouse);
    machines[B1]->canDoTransformation(&T5);
    machines[B1]->canDoTransformation(&T6);
    machines[B1]->canDoTransformation(&T7);
    machines[B1]->canDoTransformation(&T8);

    machines[B2] = new Machine(warehouse);
    machines[B2]->canDoTransformation(&T5);
    machines[B2]->canDoTransformation(&T6);
    machines[B2]->canDoTransformation(&T7);
    machines[B2]->canDoTransformation(&T8);

    machines[B3] = new Machine(warehouse);
    machines[B3]->canDoTransformation(&T5);
    machines[B3]->canDoTransformation(&T6);
    machines[B3]->canDoTransformation(&T7);
    machines[B3]->canDoTransformation(&T8);

    machines[C1] = new Machine(warehouse);
    machines[C1]->canDoTransformation(&T9);
    machines[C1]->canDoTransformation(&T10);
    machines[C1]->canDoTransformation(&T11);
    machines[C1]->canDoTransformation(&T12);

    machines[C2] = new Machine(warehouse);
    machines[C2]->canDoTransformation(&T9);
    machines[C2]->canDoTransformation(&T10);
    machines[C2]->canDoTransformation(&T11);
    machines[C2]->canDoTransformation(&T12);

    machines[C3] = new Machine(warehouse);
    machines[C3]->canDoTransformation(&T9);
    machines[C3]->canDoTransformation(&T10);
    machines[C3]->canDoTransformation(&T11);
    machines[C3]->canDoTransformation(&T12);
    
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

    Graph T(std::string("TransformationsGraph.txt"));
    T.getInfo();

    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order.GetType() == Order::ORDER_TYPE_TRANSFORMATION) {
        std::vector<std::string> shortestPath = T.Dijktras(std::to_string(order.GetInitialPiece()), std::to_string(order.GetFinalPiece()));
        std::cout << std::to_string(order.GetInitialPiece()) << " " << std::to_string(order.GetFinalPiece()) << std::endl;
        std::list<Transformation*> transformation_path;

        // Find the first transformation to be done to the part
        //std::cout << "HI: " << shortestPath.front() << std::endl;
        int curr_part = 1;//std::stoi(shortestPath.front());
        for (std::vector<std::string>::iterator iter = shortestPath.begin();
            iter != shortestPath.end();
            ++iter)
        {
            std::cout << "HI: " << *iter << std::endl;
            int next_part = std::stoi(*iter);
            for (int i = 12; i >= 1; i--) {
                if ((transformations[i]->to == next_part) && (transformations[i]->from == curr_part)) {
                    transformation_path.push_back(transformations[i]);
                    curr_part = next_part;
                    break;
                }
                throw std::exception();
            }
        }
        std::exit(0);

        // Search machines able to do these transformations
        ModulePath* best_module_path = NULL;
        for ( const auto machine : { Block::A1, Block::C3 } ) {
            if (!machines[machine]->canDoTransformation(transformation_path.front())) {
                continue;
            }

            ModulePath* ret = machines[machine]->search(order, order.GetInitialPiece(), 0, NULL);
            if (!ret) {
                continue;
            }

            if (!best_module_path || ret->time < best_module_path->time) {
                if (best_module_path) {
                    delete(best_module_path);
                }
                best_module_path = ret;
            }
        }

        // TODO

        // Update of free/blocked machines

    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order.GetType() == Order::ORDER_TYPE_UNLOAD) {
        // TODO
    }

    return path;
}
