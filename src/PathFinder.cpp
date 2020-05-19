#include "PathFinder.hpp"

/*
1 - direita
2 - baixo 
3 - esquerda
4 - cima
*/
// STUB: escreve sempre o mesmo caminho no vetor path[]
void PathFinder::FindPath(Order::BaseOrder *order_path){
    uint8_t path[59];
    uint8_t transformation[12] = {0};
    uint8_t machine_transformations[9] = {0};

    Order::Piece *piece = order_path->GetLastPiece();

    uint8_t initial_piece = order_path->GetInitialPiece();
    uint8_t final_piece = order_path->GetFinalPiece();
    uint8_t destination = final_piece; // isto é redundante. É só para ficar mais claro
    uint8_t order_type = order_path->GetType();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION){
        if (initial_piece == 2 && final_piece == 6){
            transformation[2] = 1;
            machine_transformations[0] = 1;
            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 3;
            path[5] = 1;
            path[6] = 2;
            path[7] = 2;
            path[8] = 2;
            path[9] = 2;
            path[10] = 3;
            path[11] = 3;
            path[12] = 0;
        }else if (initial_piece == 3 && final_piece == 5){
            transformation[5] = 1;
            transformation[9] = 1;
            machine_transformations[3] = 1;
            machine_transformations[6] = 1;
            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 2;
            path[5] = 3;
            path[6] = 1;
            path[7] = 2;
            path[8] = 3;
            path[9] = 1;
            path[10] = 2;
            path[11] = 2;
            path[12] = 3;
            path[13] = 3;
            path[14] = 0;
        }else if (initial_piece == 7 && final_piece == 9){
            transformation[7] = 1;
            machine_transformations[3] = 1;
            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 2;
            path[5] = 3;
            path[6] = 1;
            path[7] = 2;
            path[8] = 2;
            path[9] = 2;
            path[10] = 3;
            path[11] = 3;
            path[12] = 0;
        }else if (initial_piece == 1 && final_piece == 4){
            path[0] = 1;
            path[1] = 1;
            path[2] = 1;
            path[3] = 1;
            path[4] = 1;
            path[5] = 1;
            path[6] = 2;
            path[7] = 2;
            path[8] = 2;
            path[9] = 2;
            path[10] = 3;
            path[11] = 1;
            path[12] = 2;
            path[13] = 2;
            path[14] = 3;
            path[15] = 3;
            path[16] = 3;
            path[17] = 3;
            path[18] = 3;
            path[19] = 3;
            path[20] = 0;
            transformation[8] = 1;
            machine_transformations[8] = 1;
        }else if (initial_piece == 4 && final_piece == 8){
          transformation[10] = 1;
            machine_transformations[6] = 1;
            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 2;
            path[5] = 2;
            path[6] = 3;
            path[7] = 1;
            path[8] = 2;
            path[9] = 2;
            path[10] = 3;
            path[11] = 3;
            path[12] = 0;
        }else if (initial_piece == 1 && final_piece == 9){
            transformation[8] = 1;
            machine_transformations[6] = 1;

            transformation[10] = 1;
            machine_transformations[7] = 1;

            transformation[11] = 1;
            machine_transformations[8] = 1;


            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 2;
            path[5] = 2;
            path[6] = 3;
            path[7] = 1;
            path[8] = 1;
            path[9] = 1;
            path[10] = 1;
            path[11] = 1;
            path[12] = 2;
            path[13] = 2;
            path[14] = 3;
            path[15] = 3;
            path[16] = 3;
            path[17] = 3;
            path[18] = 3;
            path[19] = 3;
            path[20] = 0;
        }else if (initial_piece == 4 && final_piece == 5){
           transformation[9] = 1;
            machine_transformations[6] = 1;
            path[0] = 1;
            path[1] = 1;
            path[2] = 2;
            path[3] = 2;
            path[4] = 2;
            path[5] = 2;
            path[6] = 3;
            path[7] = 1;
            path[8] = 2;
            path[9] = 2;
            path[10] = 3;
            path[11] = 3;
            path[12] = 0;
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
          path[0] = 1;
          path[1] = 1;
          path[2] = 1;
          path[3] = 1;
          path[4] = 1;
          path[5] = 1;
          path[6] = 1;
          path[7] = 2;
          path[8] = 2;
          path[9] = 1;
          path[10] = 0;
          break;
          
        case 2:
          //unload Pusher2
          path[0] = 1;
          path[1] = 1;
          path[2] = 1;
          path[3] = 1;
          path[4] = 1;
          path[5] = 1;
          path[6] = 1;
          path[7] = 2;
          path[8] = 2;
          path[9] = 2;
          path[10] = 1;
          path[11] = 0;
          break;
          
        case 3:
          //unload Pusher3
          path[0] = 1;
          path[1] = 1;
          path[2] = 1;
          path[3] = 1;
          path[4] = 1;
          path[5] = 1;
          path[6] = 1;
          path[7] = 2;
          path[8] = 2;
          path[9] = 2;
          path[10] = 2;
          path[11] = 1;
          path[12] = 0;
          break;
        
        default:
          meslog(ERROR) << "Invalid destination for Unload-type order." << std::endl;
          break;
        }
    }

    piece->SetPath(path);
    piece->SetMachines(machine_transformations);
    piece->SetTransformations(transformation);
}