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

U64 bitboard_t::attacksKing_mask(int square) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    return attack_bitboard;
}

U64 bitboard_t::attacksPawns_mask(int square, bool color) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    //attack_bitboard |= (temp_bitboard >> 7)

    return attack_bitboard;
}

U64 bitboard_t::attacksKnight_mask(int square) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    return attack_bitboard;
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

    return attack_bitboard;
}

U64 bitboard_t::attacksQueen_mask(int square, U64 occupied) {
    //TODO
    U64 temp_bitboard = 0ULL;
    U64 attack_bitboard = 0ULL;
    set_bit(temp_bitboard, square);

    return attack_bitboard;
}