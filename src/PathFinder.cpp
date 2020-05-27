#include "PathFinder.hpp"
#include <stdio.h>

PathFinder::PathFinder() {

    uint32_t aux_LUT_index = 0;
    // P1 -> P2
    LUT["12"] = 0;
    aux_LUT_index = 0;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P3
    LUT["123"] = 1;
    aux_LUT_index = 1;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P4
    LUT["1234"] = 2;
    aux_LUT_index = 2;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P5
    LUT["12348"] = 3;
    aux_LUT_index = 3;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P8 //DIST
    LUT["12348"] = 4;
    aux_LUT_index = 4;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P9     //DIST
    LUT["123489"] = 5;
    aux_LUT_index = 5;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P7
    LUT["1237"] = 6;
    aux_LUT_index = 6;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P9 //DIST
    LUT["12379"] = 7;
    aux_LUT_index = 7;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P6 //DIST
    LUT["126"] = 8;
    aux_LUT_index = 8;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P9 //DIST
    LUT["1269"] = 9;
    aux_LUT_index = 9;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].transformation[3] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 3;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    




    // P2 -> P6
    LUT["26"] = 0;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 0;
    Info[aux_LUT_index].path[9] = 0;
    Info[aux_LUT_index].path[10] = 0;

    // P3 -> P5
    LUT["345"] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;
    Info[aux_LUT_index].path[10] = 0;
    Info[aux_LUT_index].path[11] = 0;
    Info[aux_LUT_index].path[12] = 0;

    // P7 -> P9
    LUT["79"] = 2;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 0;
    Info[aux_LUT_index].path[9] = 0;
    Info[aux_LUT_index].path[10] = 0;

    // P1 -> P4
    LUT["14"] = 3;
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 0;
    Info[aux_LUT_index].path[9] = 0;
    Info[aux_LUT_index].path[10] = 0;

    // P4 -> P8
    LUT["48"] = 4;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 0;
    Info[aux_LUT_index].path[9] = 0;
    Info[aux_LUT_index].path[10] = 0;

    // P1 -> P9
    LUT["1489"] = 5;
    //Distribuido por 3 celulas
    /*Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[5] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[8] = 1;
    
    Info[aux_LUT_index].path[0] = 1;
    Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 1;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 1;
    Info[aux_LUT_index].path[11] = 1;
    Info[aux_LUT_index].path[12] = 2;
    Info[aux_LUT_index].path[13] = 2;
    Info[aux_LUT_index].path[14] = 3;
    Info[aux_LUT_index].path[15] = 3;
    Info[aux_LUT_index].path[16] = 3;
    Info[aux_LUT_index].path[17] = 3;
    Info[aux_LUT_index].path[18] = 3;
    Info[aux_LUT_index].path[19] = 3;
    Info[aux_LUT_index].path[20] = 0;*/
    
    //Tudo na mesma celula
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 3;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;

    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 1;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;
    Info[aux_LUT_index].path[12] = 0;
    Info[aux_LUT_index].path[13] = 0;
    Info[aux_LUT_index].path[14] = 0;
    Info[aux_LUT_index].path[15] = 0;
    Info[aux_LUT_index].path[16] = 0;
    Info[aux_LUT_index].path[17] = 0;
    Info[aux_LUT_index].path[18] = 0;

    // P4 -> P5
    LUT["45"] = 6;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    //Info[aux_LUT_index].path[0] = 1;
    //Info[aux_LUT_index].path[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 0;
    Info[aux_LUT_index].path[9] = 0;
    Info[aux_LUT_index].path[10] = 0;
    Info[aux_LUT_index].path[11] = 0;
    Info[aux_LUT_index].path[12] = 0;
    Info[aux_LUT_index].path[13] = 0;
    Info[aux_LUT_index].path[14] = 0;
    Info[aux_LUT_index].path[15] = 0;
    Info[aux_LUT_index].path[16] = 0;
    Info[aux_LUT_index].path[17] = 0;
    Info[aux_LUT_index].path[18] = 0;

    
}


Path *PathFinder::FindPath(Order::BaseOrder &order) {
    Path *path = new Path;

    Order::Piece *piece = order.GetLastPiece();

    uint8_t initial_piece = order.GetInitialPiece();
    uint8_t final_piece = order.GetFinalPiece();
    uint8_t order_type = order.GetType();
    uint8_t destination = final_piece; // isto é redundante. É só para ficar mais claro

    Graph T(string("TransformationsGraph.txt"));
    T.getInfo();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION) {
        vector<string> shortestPath = T.Dijktras(to_string(initial_piece), to_string(final_piece));
        string _shortestPath;
        for (auto const &s : shortestPath) {
            _shortestPath += s;
        }
        //cout << "Shortest Path: " << _shortestPath << endl;

        //if (&shortestPath == NULL) {meslog(ERROR) << "No path found for transformation order provided." << std::endl;}

        
        mapT::iterator it = LUT.find(_shortestPath);
        uint8_t index_info = 0;
        if (it != LUT.end())
            index_info = it->second;

        bool C1free,C2free,C3free;  //read from OPCUA
        C1free = 1;
        uint8_t path_index = 0;
        uint8_t machine_step = 0;
        // Saida armazem
        if(C1free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 0;
            path->moves[3] = 0;
            path->moves[4] = 0;
            path->moves[5] = 0;
            path_index = 2;
            machine_step = 0;
        }

        else if(C2free && !C1free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 0;
            path->moves[5] = 0;
            path_index = 4;
            machine_step = 3;
        }

        else if(C3free && !C1free && !C2free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path_index = 6;
            machine_step = 6;
        }

        //Info transformations and machines
        
        for (int i = machine_step; i < 9; i++) {
            path->machine_transformations[i] = Info[index_info].machine_transformations[i];
        }

        for (int i = 0; i < 12; i++) {
            path->transformations[i] = Info[index_info].transformation[i];
        }


        //mudar SetPath para usar pointers
        for (int i = 0; i < 59; i++,path_index++) {
            path->moves[path_index] = Info[index_info].path[i];
        }

        // Entrada armazem
        if(C1free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 0;
            path->moves[path_index+3] = 0;
            path->moves[path_index+4] = 0;
            path->moves[path_index+5] = 0;
        }

        else if(C2free && !C1free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 3;
            path->moves[path_index+3] = 3;
            path->moves[path_index+4] = 0;
            path->moves[path_index+5] = 0;
        }

        else if(C3free && !C1free && !C2free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 3;
            path->moves[path_index+3] = 3;
            path->moves[path_index+4] = 3;
            path->moves[path_index+5] = 3;
        }

       
       // Update of free/blocked cells

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
