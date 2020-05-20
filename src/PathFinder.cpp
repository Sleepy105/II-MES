#include "PathFinder.hpp"

/*
1 - direita
2 - baixo 
3 - esquerda
4 - cima
*/
// STUB: escreve sempre o mesmo caminho no vetor path[]
Path* PathFinder::FindPath(Order::BaseOrder& order){
    uint8_t moves[59];
    uint8_t transformation[12] = {0};
    uint8_t machine_transformations[9] = {0};

    Order::Piece *piece = order.GetLastPiece();

    uint8_t initial_piece = order.GetInitialPiece();
    uint8_t final_piece = order.GetFinalPiece();
    uint8_t destination = final_piece; // isto é redundante. É só para ficar mais claro
    uint8_t order_type = order.GetType();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION){
        if (initial_piece == 2 && final_piece == 6){
            transformation[2] = 1;
            machine_transformations[0] = 1;
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 3;
            moves[5] = 1;
            moves[6] = 2;
            moves[7] = 2;
            moves[8] = 2;
            moves[9] = 2;
            moves[10] = 3;
            moves[11] = 3;
            moves[12] = 0;
        }else if (initial_piece == 3 && final_piece == 5){
            transformation[5] = 1;
            transformation[9] = 1;
            machine_transformations[3] = 1;
            machine_transformations[6] = 1;
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 2;
            moves[5] = 3;
            moves[6] = 1;
            moves[7] = 2;
            moves[8] = 3;
            moves[9] = 1;
            moves[10] = 2;
            moves[11] = 2;
            moves[12] = 3;
            moves[13] = 3;
            moves[14] = 0;
        }else if (initial_piece == 7 && final_piece == 9){
            transformation[7] = 1;
            machine_transformations[3] = 1;
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 2;
            moves[5] = 3;
            moves[6] = 1;
            moves[7] = 2;
            moves[8] = 2;
            moves[9] = 2;
            moves[10] = 3;
            moves[11] = 3;
            moves[12] = 0;
        }else if (initial_piece == 1 && final_piece == 4){
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 1;
            moves[3] = 1;
            moves[4] = 1;
            moves[5] = 1;
            moves[6] = 2;
            moves[7] = 2;
            moves[8] = 2;
            moves[9] = 2;
            moves[10] = 3;
            moves[11] = 1;
            moves[12] = 2;
            moves[13] = 2;
            moves[14] = 3;
            moves[15] = 3;
            moves[16] = 3;
            moves[17] = 3;
            moves[18] = 3;
            moves[19] = 3;
            moves[20] = 0;
            transformation[8] = 1;
            machine_transformations[8] = 1;
        }else if (initial_piece == 4 && final_piece == 8){
          transformation[10] = 1;
            machine_transformations[6] = 1;
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 2;
            moves[5] = 2;
            moves[6] = 3;
            moves[7] = 1;
            moves[8] = 2;
            moves[9] = 2;
            moves[10] = 3;
            moves[11] = 3;
            moves[12] = 0;
        }else if (initial_piece == 1 && final_piece == 9){
            transformation[8] = 1;
            machine_transformations[6] = 1;

            transformation[10] = 1;
            machine_transformations[7] = 1;

            transformation[11] = 1;
            machine_transformations[8] = 1;


            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 2;
            moves[5] = 2;
            moves[6] = 3;
            moves[7] = 1;
            moves[8] = 1;
            moves[9] = 1;
            moves[10] = 1;
            moves[11] = 1;
            moves[12] = 2;
            moves[13] = 2;
            moves[14] = 3;
            moves[15] = 3;
            moves[16] = 3;
            moves[17] = 3;
            moves[18] = 3;
            moves[19] = 3;
            moves[20] = 0;
        }else if (initial_piece == 4 && final_piece == 5){
           transformation[9] = 1;
            machine_transformations[6] = 1;
            moves[0] = 1;
            moves[1] = 1;
            moves[2] = 2;
            moves[3] = 2;
            moves[4] = 2;
            moves[5] = 2;
            moves[6] = 3;
            moves[7] = 1;
            moves[8] = 2;
            moves[9] = 2;
            moves[10] = 3;
            moves[11] = 3;
            moves[12] = 0;
        }else{
          meslog(ERROR) << "No path found for transformation order provided." << std::endl;
        }
    }////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else 
    if (order_type == Order::ORDER_TYPE_UNLOAD){
        switch (destination)
        {
        case 1:
          //unload Pusher1
          moves[0] = 1;
          moves[1] = 1;
          moves[2] = 1;
          moves[3] = 1;
          moves[4] = 1;
          moves[5] = 1;
          moves[6] = 1;
          moves[7] = 2;
          moves[8] = 2;
          moves[9] = 1;
          moves[10] = 0;
          break;
          
        case 2:
          //unload Pusher2
          moves[0] = 1;
          moves[1] = 1;
          moves[2] = 1;
          moves[3] = 1;
          moves[4] = 1;
          moves[5] = 1;
          moves[6] = 1;
          moves[7] = 2;
          moves[8] = 2;
          moves[9] = 2;
          moves[10] = 1;
          moves[11] = 0;
          break;
          
        case 3:
          //unload Pusher3
          moves[0] = 1;
          moves[1] = 1;
          moves[2] = 1;
          moves[3] = 1;
          moves[4] = 1;
          moves[5] = 1;
          moves[6] = 1;
          moves[7] = 2;
          moves[8] = 2;
          moves[9] = 2;
          moves[10] = 2;
          moves[11] = 1;
          moves[12] = 0;
          break;
        
        default:
          meslog(ERROR) << "Invalid destination for Unload-type order." << std::endl;
          break;
        }
    }

    //piece->SetPath(moves);
    //piece->SetMachines(machine_transformations);
    //piece->SetTransformations(transformation);
    return NULL;
}