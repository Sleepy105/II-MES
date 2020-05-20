#include "PathFinder.hpp"

/*
1 - direita
2 - baixo 
3 - esquerda
4 - cima
*/
// STUB: escreve sempre o mesmo caminho no vetor path[]
Path *PathFinder::FindPath(Order::BaseOrder &order) {
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