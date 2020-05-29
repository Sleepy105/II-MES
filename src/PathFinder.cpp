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

    if (!best_so_far) {
        best_so_far = new ModulePath();
    }
    ModulePath* best_path = searchUpstream(order, part_type, time_so_far+self_time, best_so_far);
    best_path->path.push_back(this);
    best_path->time += self_time;
    return best_path;
}

PathFinder::ModulePath* PathFinder::BaseModule::searchUpstream(Order::BaseOrder& order, uint8_t part_type, uint32_t time_so_far, ModulePath* best_so_far) {
    for ( const auto dir : { Direction::Right, Direction::Up } ) {
        if (!isUpstream(dir)) {
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

void PathFinder::BaseModule::setDir(Direction dir, BaseModule* module, bool upstream) {
    modules[dir] = module;
    upstreams[dir] = upstream;
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

bool PathFinder::BaseModule::isUpstream(Direction dir) {
    return upstreams[dir];
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
    return (getTransformationThatMakesPart(part_type));
}

uint32_t PathFinder::Machine::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    uint32_t handle_time = Receive;

    for (std::list<Operation*>::iterator iter = operation_queue.begin();
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
    }

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
    return t->from;
}

uint32_t PathFinder::Rotational::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return Rotate + Receive + Rotate;
}

PathFinder::PathFinder::PathFinder(Warehouse* warehouse) : warehouse(warehouse) {

}

Path* PathFinder::PathFinder::FindPath(Order::BaseOrder &order) {
    Path* path = new Path;

    Graph T(std::string("TransformationsGraph.txt"));
    T.getInfo();

    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order.GetType() == Order::ORDER_TYPE_TRANSFORMATION) {
        std::vector<std::string> shortestPath = T.Dijktras(std::to_string(order.GetInitialPiece()), std::to_string(order.GetFinalPiece()));
        std::string _shortestPath;
        for (auto const &s : shortestPath) {
            _shortestPath += s;
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
