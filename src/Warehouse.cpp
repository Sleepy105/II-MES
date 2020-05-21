
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
    if (type > 0){
        pieceTypeCount_[type-1] += 1;
        return true;
    }
    return false;
}

bool Warehouse::RemovePiece(uint8_t type){
    // if type is not type 0 and warehouse still has pieces of that type
    if (type > 0 && pieceTypeCount_[type-1] > 0){
        pieceTypeCount_[type-1] -= 1;
        return true;
    }
    return false;
}

uint8_t Warehouse::GetPieceCount(uint8_t type){
    return pieceTypeCount_[type-1];
}

bool Warehouse::RestoreStatus(int *information){
    for(int i = 1; i < 10; i++) {
        pieceTypeCount_[i] = information[i-1];
    }
    
    return false;
}



