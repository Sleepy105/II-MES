#include "PathFinder.hpp"

// STUB: escreve sempre o mesmo caminho no vetor path[]
void PathFinder::FindPath(uint16_t path[]){
    uint8_t i;
    for (i = 0; i  < 59; i++){
        path[i] = 0;
    }
    path[0] = 1;
    path[1] = 1;
    path[2] = 2;
    path[3] = 2;
    path[4] = 1;
    path[5] = 3;
    path[6] = 2;
    path[7] = 2;
    path[8] = 2;
    path[9] = 2;
    path[10] = 3;
    path[11] = 3;
}