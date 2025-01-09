#include "bitboard.hpp"

U64 fileA = 0x0101010101010101;
U64 fileH = 0x8080808080808080;
U64 fileAB = 0x0303030303030303;
U64 fileGH = 0xc0c0c0c0c0c0c0c0;


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
    if (square%8 >= 6){
        temp_bitboard = 0x0303030303030303;
    }
    if (square%8 <= 1){
        temp_bitboard = 0xC0C0C0C0C0C0C0C0;
    }
    if (square < 8){
        temp_bitboard |= 0xFFFF000000000000;
    }
    if (square > 47){
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

U64 bitboard_t::attacksBishopsMagic(int square, U64 occupied) {
    U64 rayoccupied = occupied & attacksBishopsNoBlockers[square];
    U64 key = (rayoccupied*magicBishops[square]) >> (64 - bishopShifts[square]);

    return attacksBishops[square][key];
}

U64 bitboard_t::attacksRooksMagic(int square, U64 occupied) {
    U64 rayoccupied = occupied & attacksRooksNoBlockers[square];
    U64 key = (rayoccupied*magicRooks[square]) >> (64 - rookShifts[square]);
    
    return attacksRooks[square][key];
}

U64 bitboard_t::attacksQueensMagic(int square, U64 occupied) {
    return attacksBishopsMagic(square, occupied) | attacksRooksMagic(square, occupied);
}



/*
NO NEED TO LOOK BELOW.
ALL FUNCTIONS BELOW ARE ONLY FOR INITIALIZING THE BITBOARDS
*/




U64 bitboard_t::attacksBishop_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);
    for (int i = 1; (square - i*9)>=0; i++){  //down left
        if ((square - i*9)%8 >= square % 8){
            break;
        }
        set_bit(attack_bitboard, (square - i*9));
        if (get_bit(occupied, (square - i*9))==1){
            break;
        }
    }
    for (int i=1; (square + i*9)<64; i++){ //up right
        if ((square + i*9)%8 <= square % 8){
            break;
        }
        set_bit(attack_bitboard, (square + i*9));
        if (get_bit(occupied, (square + i*9))==1){
            break;
        }
    }
    for (int i=1; (square - i*7)>=0; i++){ //down right
        if (((square - i*7))%8 <= square % 8){
            break;
        }
        set_bit(attack_bitboard, (square - i*7));
        if (get_bit(occupied, (square - i*7))==1){
            break;
        }
    }
    for (int i = 1; (square + i*7)<64; i++){ //up left
        if (((square + i*7))%8 >= square % 8){
            break;
        }
        set_bit(attack_bitboard, (square + i*7));
        if (get_bit(occupied, (square + i*7))==1){
            break;
        }
    }
    return attack_bitboard;
}

U64 bitboard_t::attacksBishop_mask_short(int square) {
    return attacksBishop_mask(square, 0) & 35604928818740736; //35604928818740736 - 1 everywhere, except at the boarders
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
    for (int i=1; (square - i*8)>=0; i++){ //>= 0 because 0 is still legal move
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

U64 bitboard_t::attacksRook_mask_short(int square) {
    U64 attack_bitboard = 0ULL;

    int i = square / 8;
    int j = square % 8;

    for (int y = j+1; y <= 6; y++){
        set_bit(attack_bitboard, (i*8+y));
    }
    for (int y = j-1; y >= 1; y--){
        set_bit(attack_bitboard, (i*8+y));
    }
    for (int x = i+1; x <= 6; x++){
        set_bit(attack_bitboard, (x*8+j));
    }
    for (int x = i-1; x >= 1; x--){
        set_bit(attack_bitboard, (x*8+j));
    }

    return attack_bitboard;
}

U64 bitboard_t::attacksQueen_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    //from rook
    for (int i=1; (square + i*8)<64; i++){
        set_bit(attack_bitboard, (square + i*8));
        if (get_bit(occupied, (square + i*8))==1){
            break;
        }
    }
    for (int i=1; (square - i*8)>=0; i++){ //>= 0 because 0 is still legal move
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
    
    //from bishop
    for (int i = 1; (square - i*9)>=0; i++){  //down left
        if ((square - i*9)%8 > square % 8){
            break;
        }
        set_bit(attack_bitboard, (square - i*9));
        if (get_bit(occupied, (square - i*9))==1){
            break;
        }
    }
    for (int i=1; (square + i*9)<64; i++){ //up right
        if ((square + i*9)%8 < square % 8){
            break;
        }
        set_bit(attack_bitboard, (square + i*9));
        if (get_bit(occupied, (square + i*9))==1){
            break;
        }
    }
    for (int i=1; (square - i*7)>=0; i++){ //down right
        if (((square - i*7))%8 < square % 8){
            break;
        }
        set_bit(attack_bitboard, (square - i*7));
        if (get_bit(occupied, (square - i*7))==1){
            break;
        }
    }
    for (int i = 1; (square + i*7)<64; i++){ //up left
        if (((square + i*7))%8 > square % 8){
            break;
        }
        set_bit(attack_bitboard, (square + i*7));
        if (get_bit(occupied, (square + i*7))==1){
            break;
        }
    }

    return attack_bitboard;
}
