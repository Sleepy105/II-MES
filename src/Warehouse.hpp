#ifndef _WAREHOUSE_HPP
#define _WAREHOUSE_HPP

#include <iostream>

class Warehouse{
private:
    uint8_t pieceTypeCount_[9];
    const uint8_t DEFAULT_INITIAL_PIECE_COUNT = 54;

public:
    Warehouse(uint8_t initial_count[] = NULL);
    ~Warehouse();

    bool AddPiece(uint8_t type);
    bool RemovePiece(uint8_t type);
    uint8_t GetPieceCount(uint8_t type);
    bool RestoreStatus();
};

#endif
