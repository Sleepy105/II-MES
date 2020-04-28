
#include "Warehouse.hpp"

Warehouse::Warehouse(uint8_t initial_values[]){
    uint8_t i;
    if (initial_values) {
        for (i = 0; i < 9; i++){
            pieceTypeCount_[i] = initial_values[i];
        }
    }else{
        for (i = 0; i < 9; i++){
            pieceTypeCount_[i] = DEFAULT_INITIAL_PIECE_COUNT;
        }
    }

}

bool Warehouse::AddPiece(uint8_t type){

}

bool Warehouse::RemovePiece(uint8_t type){

}

uint8_t Warehouse::GetPieceCount(uint8_t type){

}

uint8_t Warehouse::RestorePiece(uint8_t type){

}



