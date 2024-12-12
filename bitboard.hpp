#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <iostream>
#include <map>
#include "macro.hpp"
#include "board.hpp"


/*
If first time read this:
For bit operations use & |. and NOT && ||
You can also use the macros get_bit(), set_bit(), get_LSB()
To test your implementation of a bitboard you can use printBBany(bitboard)
*/

/*

USEFUL BIT FUNCTIONS

*/

struct bitboard_t {
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
    U64 piecesBB[12] {};
    bool turn;
    int pieceTable[SIZESQ];
    int kingWhere[2];

    bitboard_t();

    void printBBpiece(int index);

    void printBBany(U64 u64);

    void printBB();

    /* Turns a location (i, j) to a string (ex: (0, 0) maps to "a1") */
	std::string move_to_string(move_t* move);
	/* Turns a string to a location (i, j) (ex: "a1" maps to (0, 0)) */
	move_t* string_to_move(std::string move);

    void update(move_t* move);
    void undo(move_t* move);

    /* Attack masks */ // implementation in bitmoves.cpp
    U64 attacksKing_mask(int square);
    U64 attacksPawns_mask(int square, bool color);
    U64 attacksKnight_mask(int square);

    U64 attacksBishop_mask(int square, U64 occupied);
    U64 attacksRook_mask(int square, U64 occupied);
    U64 attacksQueen_mask(int square, U64 occupied);

    /* Precomputed data of all moves */
    U64 attacksKing[SIZESQ];
    U64 attacksPawns[2][SIZESQ];
    U64 attacksKnights[SIZESQ];
    
    /* Can implement for now, but we will use magic boards for this*/
    U64 attacksBishops[SIZESQ];
    U64 attacksRooks[SIZESQ];
    U64 attacksQueens[SIZESQ];

    U64 attacksBishopsMagic(U64 occupied, int square);
    U64 attacksRooksMagic(U64 occupied, int square);
    U64 attacksQueensMagic(U64 occupied, int square);

    /* Use precomputed data */
    bool is_square_attacked(int square, bool color);


    /* Need to think */
    /* Later */
    array_moves allMovesPawns(bool color);
    array_moves allMovesKnights(bool color);
    array_moves allMovesKing(bool color);
    
    array_moves allMovesBishop(bool color);
    array_moves allMovesRooks(bool color);
    array_moves allMovesQueens(bool color);

};

#endif