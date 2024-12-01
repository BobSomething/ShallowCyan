#ifndef PIECE_HPP_
#define PIECE_HPP_

#include <iostream>
#include <vector>
#include "board.hpp"
#include "macro.hpp"

struct board_t;


struct piece_t {
    bool color; //1 - white and 0 - black
    int points; //counts how valueable is a piece
    coords location; //location on the board, if there will be no other methods, just put it in the legal_moves

    /* vector of pairs (i,j), indicating the cells */
    virtual array_coords legal_moves(board_t* board); 

    /* constructor of piece_t */
    piece_t(coords location, bool color)
        : location {location}, color {color}, points {0} {}

    /* returns false if the cell is out of bounds and if the cell contains a piece with the same color */
    bool check_cell(int x, int y, board_t* board);
};


struct pawn_t : piece_t {
    array_coords legal_moves(board_t* board);

    pawn_t(coords location, bool color)
        : piece_t(location, color) {points = 100;}
};

struct bishop_t : piece_t {
    array_coords legal_moves(board_t* board);

    bishop_t(coords location, bool color)
        : piece_t(location, color) {points = 300;}
};

struct knight_t : piece_t {
    array_coords legal_moves(board_t* board);

    knight_t(coords location, bool color)
        : piece_t(location, color) {points = 300;}
};

struct rook_t : piece_t {
    array_coords legal_moves(board_t* board);

    rook_t(coords location, bool color)
        : piece_t(location, color) {points = 500;}
};

struct king_t : piece_t {
    array_coords legal_moves(board_t* board);

    king_t(coords location, bool color)
        : piece_t(location, color) {points = 0;}
};

struct queen_t : piece_t {
    array_coords legal_moves(board_t* board);

    queen_t(coords location, bool color)
        : piece_t(location, color) {points = 900;}
};

#endif