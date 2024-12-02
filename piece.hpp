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
    std::string id; //identification of a piece
    board_t* board; //the pointer to the board

    /* vector of pairs (i,j), indicating the cells */
    virtual array_coords legal_moves();
    virtual bool is_checked();

    /* constructor of piece_t */
    piece_t(coords location, bool color, board_t* board)
        : location {location}, color {color}, points {0}, id {""}, board {board} {}

};


struct pawn_t : piece_t {
    array_coords legal_moves();

    pawn_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 100; id = "p";}
};

struct bishop_t : piece_t {
    array_coords legal_moves();

    bishop_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 300; id = "b";}
};

struct knight_t : piece_t {
    array_coords legal_moves();

    knight_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 300; id = "k";}
};

struct rook_t : piece_t {
    array_coords legal_moves();

    rook_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 500; id = "r";}
};

struct king_t : piece_t {
    array_coords legal_moves();

    bool is_checked();

    king_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 0; id = "K";}
};

struct queen_t : piece_t {
    array_coords legal_moves();

    queen_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {points = 900; id = "q";}
};

#endif