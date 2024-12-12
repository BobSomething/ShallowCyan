#include "bitboard.hpp"

void bitboard_t::update(move_t* move) {
    
}

void bitboard_t::undo(move_t* move) {
    
}

/*
DUC read:

This will be probably useful:
To not have loop over attack moves you can & it wih not fileA: you do ~fileA
*/

U64 fileA = 0x0101010101010101;
U64 fileH = 0x8080808080808080;

/*
Im assuming the bitboard look like this
57 58 59 ... 64
... .... ... ...
.... 
1 2 3 ...     8
with the integer stands for the index of the bit
*/

U64 bitboard_t::attacksKing_mask(int square) {
    //TODO
    //To avoid loops
    U64 temp_bitboard = 0x0000000000000000;
    if (square%8 == 7){
        temp_bitboard = 0x0101010101010101;
    }
    if (square%8 == 0){
        temp_bitboard = 0x8080808080808080;
    }
    if (square < 8){
        temp_bitboard |= 0xFF00000000000000;
    }
    if (square > 55){
        temp_bitboard |= 0x00000000000000FF;
    }

    U64 attack_bitboard = 0ULL;
    // set_bit(temp_bitboard, square);

    set_bit(attack_bitboard, (square + 1));
    set_bit(attack_bitboard, (square - 1));
    set_bit(attack_bitboard, (square + 8));
    set_bit(attack_bitboard, (square - 8));
    set_bit(attack_bitboard, (square + 7));
    set_bit(attack_bitboard, (square + 9));
    set_bit(attack_bitboard, (square - 7));
    set_bit(attack_bitboard, (square - 9));

    return attack_bitboard & (~temp_bitboard);
    // return temp_bitboard;
}

U64 bitboard_t::attacksPawns_mask(int square, bool color) {
    //TODO
    //To avoid loops
    U64 temp_bitboard = 0x0000000000000000;
    if (square%8 == 7){
        temp_bitboard = 0x0101010101010101;
    }
    if (square%8 == 0){
        temp_bitboard = 0x8080808080808080;
    }
    if (square < 8){
        temp_bitboard |= 0xFF00000000000000;
    }
    if (square > 55){
        temp_bitboard |= 0x00000000000000FF;
    }

    U64 attack_bitboard = 0ULL;
    if (color == 1){
        set_bit(attack_bitboard, (square + 7));
        set_bit(attack_bitboard, (square + 9));
    }
    if (color == 0){
        set_bit(attack_bitboard, (square - 7));
        set_bit(attack_bitboard, (square - 9));
    }

    //attack_bitboard |= (temp_bitboard >> 7)

    return attack_bitboard & (~temp_bitboard);
}

U64 bitboard_t::attacksKnight_mask(int square) {
    //TODO
    //To avoid loop
    U64 temp_bitboard = 0x0000000000000000;
    if (square%8 == 7){
        temp_bitboard = 0x0303030303030303;
    }
    if (square%8 == 0){
        temp_bitboard = 0xC0C0C0C0C0C0C0C0;
    }
    if (square < 8){
        temp_bitboard |= 0xFFFF000000000000;
    }
    if (square > 55){
        temp_bitboard |= 0x000000000000FFFF;
    }

    U64 attack_bitboard = 0ULL;
    set_bit(attack_bitboard, (square+10));
    set_bit(attack_bitboard, (square+6));
    set_bit(attack_bitboard, (square+17));
    set_bit(attack_bitboard, (square+15));
    set_bit(attack_bitboard, (square-10));
    set_bit(attack_bitboard, (square-6));
    set_bit(attack_bitboard, (square-17));
    set_bit(attack_bitboard, (square-15));

    return attack_bitboard & (~temp_bitboard);
}

U64 bitboard_t::attacksBishop_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    return attack_bitboard;
}

U64 bitboard_t::attacksRook_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    for (int i=1; (square + i*8)<64; i++){
        set_bit(attack_bitboard, (square + i*8));
        if (get_bit(occupied, (square + i*8))==1){
            break;
        }
    }
    for (int i=1; (square - i*8)>0; i++){
        set_bit(attack_bitboard, (square - i*8));
        if (get_bit(occupied, (square - i*8))==1){
            break;
        }
    }
    for (int i=-1; i>-(square%8)-1; i--){
        set_bit(attack_bitboard, (square + i));
        if (get_bit(occupied, (square+i))==1){
            break;
        }
    }
    for (int i=1; i<8-(square%8); i++){
        set_bit(attack_bitboard, (square + i));
        if (get_bit(occupied, (square+i))==1){
            break;
        }
    }

    return attack_bitboard;
    // return occupied;
}

U64 bitboard_t::attacksQueen_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    return attack_bitboard;
}