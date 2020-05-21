#include "PathFinder.hpp"

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

PathFinder::BaseModule::BaseModule() {

}

PathFinder::BaseModule::~BaseModule() {
    
}

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

PathFinder::Linear::Linear() {
    type = Type::Linear;
}

PathFinder::Linear::~Linear() {

}

uint32_t PathFinder::Linear::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return Receive;
}

PathFinder::Machine::Machine() {
    type = Type::Machine;
}

PathFinder::Machine::~Machine() {

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
    // TODO Check for tool changes
    // Divide tool change time by number of parts in the order

    Transformation* t = getTransformationThatMakesPart(part_type);
    return t->time + Receive;
}

uint8_t PathFinder::Machine::changeType(uint8_t part_type) {
    Transformation* t = getTransformationThatMakesPart(part_type);
    return t->from;
}

PathFinder::Rotational::Rotational() {
    type = Type::Rotational;
}

PathFinder::Rotational::~Rotational() {

}

uint32_t PathFinder::Rotational::calcTimeToHandlePart(Order::BaseOrder& order, uint8_t part_type) {
    return Rotate + Receive + Rotate;
}

PathFinder::Pusher::Pusher() {
    type = Type::Pusher;
}

PathFinder::Pusher::~Pusher() {

}

PathFinder::PathFinder::PathFinder() {

}

PathFinder::PathFinder::~PathFinder() {

}

Path *PathFinder::PathFinder::FindPath(Order::BaseOrder &order) {
    Path* path = new Path;

    Order::Piece *piece = order.GetLastPiece();

    uint8_t initial_piece = order.GetInitialPiece();
    uint8_t final_piece = order.GetFinalPiece();
    uint8_t order_type = order.GetType();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION) {
        if (initial_piece == 2 && final_piece == 6) {
            path->transformations[2] = 1;
            path->machine_transformations[0] = 1;
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 3;
            path->moves[5] = 1;
            path->moves[6] = 2;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 3;
            path->moves[11] = 3;
            path->moves[12] = 0;
        }
        else if (initial_piece == 3 && final_piece == 5) {
            path->transformations[5] = 1;
            path->transformations[9] = 1;
            path->machine_transformations[3] = 1;
            path->machine_transformations[6] = 1;
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 2;
            path->moves[5] = 3;
            path->moves[6] = 1;
            path->moves[7] = 2;
            path->moves[8] = 3;
            path->moves[9] = 1;
            path->moves[10] = 2;
            path->moves[11] = 2;
            path->moves[12] = 3;
            path->moves[13] = 3;
            path->moves[14] = 0;
        }
        else if (initial_piece == 7 && final_piece == 9) {
            path->transformations[7] = 1;
            path->machine_transformations[3] = 1;
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 2;
            path->moves[5] = 3;
            path->moves[6] = 1;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 3;
            path->moves[11] = 3;
            path->moves[12] = 0;
        }
        else if (initial_piece == 1 && final_piece == 4) {
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path->moves[6] = 2;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 3;
            path->moves[11] = 1;
            path->moves[12] = 2;
            path->moves[13] = 2;
            path->moves[14] = 3;
            path->moves[15] = 3;
            path->moves[16] = 3;
            path->moves[17] = 3;
            path->moves[18] = 3;
            path->moves[19] = 3;
            path->moves[20] = 0;
            path->transformations[8] = 1;
            path->machine_transformations[8] = 1;
        }
        else if (initial_piece == 4 && final_piece == 8) {
            path->transformations[10] = 1;
            path->machine_transformations[6] = 1;
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 2;
            path->moves[5] = 2;
            path->moves[6] = 3;
            path->moves[7] = 1;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 3;
            path->moves[11] = 3;
            path->moves[12] = 0;
        }
        else if (initial_piece == 1 && final_piece == 9) {
            path->transformations[8] = 1;
            path->machine_transformations[6] = 1;

            path->transformations[10] = 1;
            path->machine_transformations[7] = 1;

            path->transformations[11] = 1;
            path->machine_transformations[8] = 1;

            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 2;
            path->moves[5] = 2;
            path->moves[6] = 3;
            path->moves[7] = 1;
            path->moves[8] = 1;
            path->moves[9] = 1;
            path->moves[10] = 1;
            path->moves[11] = 1;
            path->moves[12] = 2;
            path->moves[13] = 2;
            path->moves[14] = 3;
            path->moves[15] = 3;
            path->moves[16] = 3;
            path->moves[17] = 3;
            path->moves[18] = 3;
            path->moves[19] = 3;
            path->moves[20] = 0;
        }
        else if (initial_piece == 4 && final_piece == 5) {
            path->transformations[9] = 1;
            path->machine_transformations[6] = 1;
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 2;
            path->moves[3] = 2;
            path->moves[4] = 2;
            path->moves[5] = 2;
            path->moves[6] = 3;
            path->moves[7] = 1;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 3;
            path->moves[11] = 3;
            path->moves[12] = 0;
        }
        else {
            meslog(ERROR) << "No path found for transformation order provided." << std::endl;
        }
    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order_type == Order::ORDER_TYPE_UNLOAD) {
        switch (final_piece) {
        case 1:
            //unload Pusher1
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path->moves[6] = 1;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 1;
            path->moves[10] = 0;
            break;

        case 2:
            //unload Pusher2
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path->moves[6] = 1;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 1;
            path->moves[11] = 0;
            break;

        case 3:
            //unload Pusher3
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path->moves[6] = 1;
            path->moves[7] = 2;
            path->moves[8] = 2;
            path->moves[9] = 2;
            path->moves[10] = 2;
            path->moves[11] = 1;
            path->moves[12] = 0;
            break;

        default:
            meslog(ERROR) << "Invalid destination for Unload-type order." << std::endl;
            break;
        }
    }

    return path;
}