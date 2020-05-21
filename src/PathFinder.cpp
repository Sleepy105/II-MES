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

    /****** ROW #1 *******/

    cells[1] = new Linear;
    
    cells[2] = new Linear;
    cells[2]->setDir(Direction::Left, cells[1], true);
    cells[1]->setDir(Direction::Right, cells[2], false);

    cells[3] = new Rotational;
    cells[3]->setDir(Direction::Left, cells[2], true);
    cells[2]->setDir(Direction::Right, cells[3], false);

    cells[4] = new Linear;
    cells[4]->setDir(Direction::Left, cells[3], true);
    cells[3]->setDir(Direction::Right, cells[4], false);

    cells[5] = new Rotational;
    cells[5]->setDir(Direction::Left, cells[4], true);
    cells[4]->setDir(Direction::Right, cells[5], false);

    cells[6] = new Linear;
    cells[6]->setDir(Direction::Left, cells[5], true);
    cells[5]->setDir(Direction::Right, cells[6], false);

    cells[7] = new Rotational;
    cells[7]->setDir(Direction::Left, cells[6], true);
    cells[6]->setDir(Direction::Right, cells[7], false);

    cells[8] = new Rotational;
    cells[8]->setDir(Direction::Left, cells[7], true);
    cells[7]->setDir(Direction::Right, cells[8], false);

    cells[9] = new Linear;
    cells[9]->setDir(Direction::Left, cells[8], true);
    cells[8]->setDir(Direction::Right, cells[9], false);

    /****** ROW #2 *******/

    cells[10] = new Linear;
    cells[10]->setDir(Direction::Up, cells[3], true);
    cells[3]->setDir(Direction::Down, cells[10], false);

    cells[11] = new Linear;
    cells[11]->setDir(Direction::Up, cells[5], true);
    cells[5]->setDir(Direction::Down, cells[11], false);

    cells[12] = new Linear;
    cells[12]->setDir(Direction::Up, cells[7], true);
    cells[7]->setDir(Direction::Down, cells[12], false);

    cells[13] = new Linear;
    cells[13]->setDir(Direction::Up, cells[8], true);
    cells[8]->setDir(Direction::Down, cells[13], false);

    /****** ROW #3 *******/

    cells[14] = new Machine(warehouse);

    cells[15] = new Rotational;
    cells[15]->setDir(Direction::Up, cells[10], true);
    cells[10]->setDir(Direction::Down, cells[15], false);
    cells[15]->setDir(Direction::Left, cells[14], true);
    cells[14]->setDir(Direction::Right, cells[15], true);

    cells[16] = new Machine(warehouse);
    cells[16]->setDir(Direction::Left, cells[15], true);
    cells[15]->setDir(Direction::Right, cells[16], true);

    cells[17] = new Rotational;
    cells[17]->setDir(Direction::Up, cells[11], true);
    cells[11]->setDir(Direction::Down, cells[17], false);
    cells[17]->setDir(Direction::Left, cells[16], true);
    cells[16]->setDir(Direction::Right, cells[17], true);

    cells[18] = new Machine(warehouse);
    cells[18]->setDir(Direction::Left, cells[17], true);
    cells[17]->setDir(Direction::Right, cells[18], true);

    cells[19] = new Rotational;
    cells[19]->setDir(Direction::Up, cells[12], true);
    cells[12]->setDir(Direction::Down, cells[19], false);
    cells[19]->setDir(Direction::Left, cells[18], true);
    cells[18]->setDir(Direction::Right, cells[19], true);

    cells[20] = new Pusher;
    cells[20]->setDir(Direction::Up, cells[13], true);
    cells[13]->setDir(Direction::Down, cells[20], false);

    /****** ROW #4 *******/

    cells[25] = new Machine(warehouse);

    cells[26] = new Rotational;
    cells[26]->setDir(Direction::Up, cells[15], true);
    cells[15]->setDir(Direction::Down, cells[26], false);
    cells[26]->setDir(Direction::Left, cells[25], true);
    cells[25]->setDir(Direction::Right, cells[26], true);

    cells[27] = new Machine(warehouse);
    cells[27]->setDir(Direction::Left, cells[26], true);
    cells[26]->setDir(Direction::Right, cells[27], true);

    cells[28] = new Rotational;
    cells[28]->setDir(Direction::Up, cells[17], true);
    cells[17]->setDir(Direction::Down, cells[28], false);
    cells[28]->setDir(Direction::Left, cells[27], true);
    cells[27]->setDir(Direction::Right, cells[28], true);

    cells[29] = new Machine(warehouse);
    cells[29]->setDir(Direction::Left, cells[28], true);
    cells[28]->setDir(Direction::Right, cells[29], true);

    cells[30] = new Rotational;
    cells[30]->setDir(Direction::Up, cells[19], true);
    cells[19]->setDir(Direction::Down, cells[30], false);
    cells[30]->setDir(Direction::Left, cells[29], true);
    cells[29]->setDir(Direction::Right, cells[30], true);

    cells[31] = new Pusher;
    cells[31]->setDir(Direction::Up, cells[20], true);
    cells[20]->setDir(Direction::Down, cells[31], false);

    /****** ROW #5 *******/

    cells[36] = new Machine(warehouse);

    cells[37] = new Rotational;
    cells[37]->setDir(Direction::Up, cells[26], true);
    cells[26]->setDir(Direction::Down, cells[37], false);
    cells[37]->setDir(Direction::Left, cells[36], true);
    cells[36]->setDir(Direction::Right, cells[37], true);

    cells[38] = new Machine(warehouse);
    cells[38]->setDir(Direction::Left, cells[37], true);
    cells[37]->setDir(Direction::Right, cells[38], true);

    cells[39] = new Rotational;
    cells[39]->setDir(Direction::Up, cells[28], true);
    cells[28]->setDir(Direction::Down, cells[39], false);
    cells[39]->setDir(Direction::Left, cells[38], true);
    cells[38]->setDir(Direction::Right, cells[39], true);

    cells[40] = new Machine(warehouse);
    cells[40]->setDir(Direction::Left, cells[39], true);
    cells[39]->setDir(Direction::Right, cells[40], true);

    cells[41] = new Rotational;
    cells[41]->setDir(Direction::Up, cells[30], true);
    cells[30]->setDir(Direction::Down, cells[41], false);
    cells[41]->setDir(Direction::Left, cells[40], true);
    cells[40]->setDir(Direction::Right, cells[41], true);

    cells[42] = new Pusher;
    cells[42]->setDir(Direction::Up, cells[31], true);
    cells[31]->setDir(Direction::Down, cells[42], false);

    /****** ROW #6 *******/

    cells[47] = new Linear;
    cells[47]->setDir(Direction::Up, cells[37], true);
    cells[37]->setDir(Direction::Down, cells[47], false);

    cells[48] = new Linear;
    cells[48]->setDir(Direction::Up, cells[39], true);
    cells[39]->setDir(Direction::Down, cells[48], false);

    cells[49] = new Linear;
    cells[49]->setDir(Direction::Up, cells[41], true);
    cells[41]->setDir(Direction::Down, cells[49], false);

    cells[50] = new Linear;
    cells[50]->setDir(Direction::Up, cells[42], true);
    cells[42]->setDir(Direction::Down, cells[50], false);

    /****** ROW #7 *******/

    cells[51] = new Linear;

    cells[52] = new Linear;
    cells[52]->setDir(Direction::Left, cells[51], false);
    cells[51]->setDir(Direction::Right, cells[52], true);

    cells[53] = new Rotational;
    cells[53]->setDir(Direction::Up, cells[47], true);
    cells[47]->setDir(Direction::Down, cells[53], false);
    cells[53]->setDir(Direction::Left, cells[52], false);
    cells[52]->setDir(Direction::Right, cells[53], true);

    cells[54] = new Linear;
    cells[54]->setDir(Direction::Left, cells[53], false);
    cells[53]->setDir(Direction::Right, cells[54], true);

    cells[55] = new Rotational;
    cells[55]->setDir(Direction::Up, cells[48], true);
    cells[48]->setDir(Direction::Down, cells[55], false);
    cells[55]->setDir(Direction::Left, cells[54], false);
    cells[54]->setDir(Direction::Right, cells[55], true);

    cells[56] = new Linear;
    cells[56]->setDir(Direction::Left, cells[55], false);
    cells[55]->setDir(Direction::Right, cells[56], true);

    cells[57] = new Rotational;
    cells[57]->setDir(Direction::Up, cells[49], true);
    cells[49]->setDir(Direction::Down, cells[57], false);
    cells[57]->setDir(Direction::Left, cells[56], false);
    cells[56]->setDir(Direction::Right, cells[57], true);

    cells[58] = new Rotational;
    cells[58]->setDir(Direction::Up, cells[50], true);
    cells[50]->setDir(Direction::Down, cells[58], false);
    cells[58]->setDir(Direction::Left, cells[57], false);
    cells[57]->setDir(Direction::Right, cells[58], true);

    cells[59] = new Linear;
    cells[59]->setDir(Direction::Left, cells[58], false);
    cells[58]->setDir(Direction::Right, cells[59], true);

}



Path* PathFinder::PathFinder::FindPath(Order::BaseOrder &order) {
    ModulePath* module_path = NULL;

    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order.GetType() == Order::ORDER_TYPE_TRANSFORMATION) {
        module_path = cells[51]->search(order, order.GetFinalPiece(), 0, NULL);
        if (!module_path) {
            meslog(ERROR) << "No path found for transformation order #" << order.GetID() << std::endl;
            return NULL;
        }
    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order.GetType() == Order::ORDER_TYPE_UNLOAD) {
        module_path = cells[9+order.GetFinalPiece()*11]->search(order, order.GetFinalPiece(), 0, NULL);
        if (!module_path) {
            meslog(ERROR) << "No path found for unload order #" << order.GetID() << std::endl;
            return NULL;
        }
    }

    Path* path = new Path;
    uint32_t count = 0;
    for (std::list<BaseModule*>::iterator iter = module_path->path.begin();
        iter != module_path->path.end();
        iter++)
    {
        BaseModule* module = (*iter);
        path->moves[count++] = module->searchDir(*(++iter--));
    }

    return path;
}