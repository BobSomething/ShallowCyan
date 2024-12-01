#ifndef PIECE_HPP_
#define PIECE_HPP_

#include <iostream>
#include <vector>
#include <map>
#include "board.hpp"
#include "macro.hpp"

struct board_t;

void error(std::string str) __attribute__((noreturn));

struct piece_t {
    bool color; //1 - white and 0 - black
    int points; //counts how valueable is a piece
    coords location; //location on the board, if there will be no other methods, just put it in the legal_moves
    board_t* board; //the pointer to the board

    /* vector of pairs (i,j), indicating the cells */
    virtual array_coords legal_moves(); 

    /* constructor of piece_t */
    piece_t(coords location, bool color, board_t* board)
        : location {location}, color {color}, points {0}, board {board} {}

    /* 
    returns 0 if the cell is out of bounds or if the cell in bounds and contains a piece with the same color 
    returns 1 if the cell is in bounds and it is empty
    returns 2 if the cell is in bounds and contains a piece with opposite color
    */
    int check_cell(int x, int y);
};


struct pawn_t : piece_t {
    array_coords legal_moves();

    pawn_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 100;}
};

struct bishop_t : piece_t {
    array_coords legal_moves();

    bishop_t(coords location, bool color , board_t* board)
        : piece_t(location, color, board) {points = 300;}
};

struct knight_t : piece_t {
    array_coords legal_moves();

    knight_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 300;}
};

struct rook_t : piece_t {
    array_coords legal_moves();

    rook_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 500;}
};

struct king_t : piece_t {
    array_coords legal_moves();

    king_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 0;}
};

struct queen_t : piece_t {
    array_coords legal_moves();

    queen_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 900;}
};

#endif