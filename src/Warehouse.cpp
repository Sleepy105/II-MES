
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

Warehouse::~Warehouse(){
    
}

bool Warehouse::AddPiece(uint8_t type){
    pieceTypeCount_[type-1]++;
    return true;
}

bool Warehouse::RemovePiece(uint8_t type){
    if (pieceTypeCount_[type-1] > 0){
        pieceTypeCount_[type-1]--;
        return true;
    }
    return false;
}

uint8_t Warehouse::GetPieceCount(uint8_t type){
    return pieceTypeCount_[type-1];
}

//POR IMPLEMENTAR
bool Warehouse::RestoreStatus(){
    
    return false;
}



