#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <iostream>
#include "board.hpp"

struct bitboard_t : board_t {
    /*
    0 - white pawns
    1 - white knights
    2 - white bishops
    3 - white rooks
    4 - white queens
    5 - white king
    6 - black pawns
    7 - black knights
    8 - black bishops
    9 - black rooks
    10 - black queens
    11 - black king 
    */
    uint64_t piecesBB[12] {};

    bitboard_t();

    void printBBpiece(int index);

    void printBB();

};

#endif