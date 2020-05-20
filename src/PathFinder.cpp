#include "PathFinder.hpp"

PathFinder::PathFinder() {

    // P2 -> P6
    LUT["26"] = 0;
    Info[0].transformation[2] = 1;
    Info[0].machine_transformations[0] = 1;
    Info[0].path[0] = 1;
    Info[0].path[1] = 1;
    Info[0].path[2] = 2;
    Info[0].path[3] = 2;
    Info[0].path[4] = 3;
    Info[0].path[5] = 1;
    Info[0].path[6] = 2;
    Info[0].path[7] = 2;
    Info[0].path[8] = 2;
    Info[0].path[9] = 2;
    Info[0].path[10] = 3;
    Info[0].path[11] = 3;
    Info[0].path[12] = 0;

    // P3 -> P5
    LUT["345"] = 1;
    Info[1].transformation[5] = 1;
    Info[1].transformation[9] = 1;
    Info[1].machine_transformations[1] = 1;
    Info[1].machine_transformations[2] = 1;
    Info[1].path[0] = 1;
    Info[1].path[1] = 1;
    Info[1].path[2] = 2;
    Info[1].path[3] = 2;
    Info[1].path[4] = 2;
    Info[1].path[5] = 3;
    Info[1].path[6] = 1;
    Info[1].path[7] = 2;
    Info[1].path[8] = 3;
    Info[1].path[9] = 1;
    Info[1].path[10] = 2;
    Info[1].path[11] = 2;
    Info[1].path[12] = 3;
    Info[1].path[13] = 3;
    Info[1].path[14] = 0;

    // P7 -> P9
    LUT["79"] = 2;
    Info[2].transformation[7] = 1;
    Info[2].machine_transformations[1] = 1;
    Info[2].path[0] = 1;
    Info[2].path[1] = 1;
    Info[2].path[2] = 2;
    Info[2].path[3] = 2;
    Info[2].path[4] = 2;
    Info[2].path[5] = 3;
    Info[2].path[6] = 1;
    Info[2].path[7] = 2;
    Info[2].path[8] = 2;
    Info[2].path[9] = 2;
    Info[2].path[10] = 3;
    Info[2].path[11] = 3;
    Info[2].path[12] = 0;

    // P1 -> P4
    LUT["14"] = 3;
    Info[3].transformation[8] = 1;
    Info[3].machine_transformations[2] = 1;
    Info[3].path[0] = 1;
    Info[3].path[1] = 1;
    Info[3].path[2] = 2;
    Info[3].path[3] = 2;
    Info[3].path[4] = 2;
    Info[3].path[5] = 2;
    Info[3].path[6] = 3;
    Info[3].path[7] = 1;
    Info[3].path[8] = 2;
    Info[3].path[9] = 2;
    Info[3].path[10] = 3;
    Info[3].path[11] = 3;
    Info[3].path[12] = 0;

    // P4 -> P8
    LUT["48"] = 4;
    Info[4].transformation[10] = 1;
    Info[4].machine_transformations[2] = 1;
    Info[4].path[0] = 1;
    Info[4].path[1] = 1;
    Info[4].path[2] = 2;
    Info[4].path[3] = 2;
    Info[4].path[4] = 2;
    Info[4].path[5] = 2;
    Info[4].path[6] = 3;
    Info[4].path[7] = 1;
    Info[4].path[8] = 2;
    Info[4].path[9] = 2;
    Info[4].path[10] = 3;
    Info[4].path[11] = 3;
    Info[4].path[12] = 0;

    // P1 -> P9
    LUT["1489"] = 5;
    Info[5].transformation[8] = 1;
    Info[5].machine_transformations[2] = 1;
    Info[5].transformation[10] = 1;
    Info[5].machine_transformations[5] = 1;
    Info[5].transformation[11] = 1;
    Info[5].machine_transformations[8] = 1;
    Info[5].path[0] = 1;
    Info[5].path[1] = 1;
    Info[5].path[2] = 2;
    Info[5].path[3] = 2;
    Info[5].path[4] = 2;
    Info[5].path[5] = 2;
    Info[5].path[6] = 3;
    Info[5].path[7] = 1;
    Info[5].path[8] = 1;
    Info[5].path[9] = 1;
    Info[5].path[10] = 1;
    Info[5].path[11] = 1;
    Info[5].path[12] = 2;
    Info[5].path[13] = 2;
    Info[5].path[14] = 3;
    Info[5].path[15] = 3;
    Info[5].path[16] = 3;
    Info[5].path[17] = 3;
    Info[5].path[18] = 3;
    Info[5].path[19] = 3;
    Info[5].path[20] = 0;

    // P4 -> P5
    LUT["45"] = 6;
    Info[6].transformation[9] = 1;
    Info[6].machine_transformations[2] = 1;
    Info[6].path[0] = 1;
    Info[6].path[1] = 1;
    Info[6].path[2] = 2;
    Info[6].path[3] = 2;
    Info[6].path[4] = 2;
    Info[6].path[5] = 2;
    Info[6].path[6] = 3;
    Info[6].path[7] = 1;
    Info[6].path[8] = 2;
    Info[6].path[9] = 2;
    Info[6].path[10] = 3;
    Info[6].path[11] = 3;
    Info[6].path[12] = 0;
    Info[6].path[13] = 0;
    Info[6].path[14] = 0;
    Info[6].path[15] = 0;
    Info[6].path[16] = 0;
    Info[6].path[17] = 0;
    Info[6].path[18] = 0;
    Info[6].path[19] = 0;
    Info[6].path[20] = 0;

    //T.getInfo();
}


Path *PathFinder::FindPath(Order::BaseOrder &order) {
    Path *path = new Path;

    Order::Piece *piece = order.GetLastPiece();

    uint8_t initial_piece = order.GetInitialPiece();
    uint8_t final_piece = order.GetFinalPiece();
    uint8_t order_type = order.GetType();
    uint8_t destination = final_piece; // isto é redundante. É só para ficar mais claro

    Graph T(string("TransformationsGraph.txt"));
    //T.getInfo();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION) {
        vector<string> shortestPath = T.Dijktras(to_string(initial_piece),to_string(final_piece));
        string _shortestPath;
        for (auto const& s : shortestPath) { _shortestPath += s; }
        //cout << "Shortest Path: " << _shortestPath << endl;

        //if (shortestPath == NULL) meslog(ERROR) << "No path found for transformation order provided." << std::endl;

        mapT::iterator  it= LUT.find(_shortestPath);
        uint8_t index_info = 0;
        if( it != LUT.end() ) index_info = it->second;

        //mudar SetPath para usar pointers
        for(int i=0; i<59; i++){
            path->moves[i] = Info[index_info].path[i];
        }

        if (&shortestPath == NULL)
            meslog(ERROR) << "No path found for transformation order provided." << std::endl;

        mapT::iterator it = LUT.find(_shortestPath);
        uint8_t index_info = 0;
        if (it != LUT.end())
            index_info = it->second;


        for (int i = 0; i < 9; i++) {
            path->machine_transformations[i] = Info[index_info].machine_transformations[i];
        }

        for (int i = 0; i < 9; i++) {
            path->transformations[i] = Info[index_info].transformation[i];
        }
    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order_type == Order::ORDER_TYPE_UNLOAD)
    {
        switch (destination)
        {
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
