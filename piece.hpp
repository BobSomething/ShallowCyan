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
    std::string display; //the string to display the board, capital is black, normal is white
    bool moved = false; //if a piece moved, this is only necessary for rooks and kings

    /* vector of pairs (i,j), indicating the cells */
    virtual array_coords legal_moves();
    virtual bool is_checked();

    /* constructor of piece_t */
    piece_t(coords l, bool c, board_t* b)
    // {
    //     location = l;
    //     color = c;
    //     points = 0;
    //     board = b;
    // }
        : location {l}, color {c}, points {0}, board {b} {}

};


struct pawn_t : piece_t {
    array_coords legal_moves();

    pawn_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {
            id = "p";
            points = 100;
            if (color == 0){
                display = "\u2659";
            }
            if (color == 1){
                display = "\u265F";
            }
        }
};

struct bishop_t : piece_t {
    array_coords legal_moves();

    bishop_t(coords location, bool color , board_t* board)
        : piece_t(location, color, board) {
            id = "b";
            points = 300;
            if (color == 0){
                display = "\u2657";
            }
            if (color == 1){
                display = "\u265D";
            }
        }
};

struct knight_t : piece_t {
    array_coords legal_moves();

    knight_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {
            id = "n";
            points = 300;
            if (color == 0){
                display = "\u2658";
            }
            if (color == 1){
                display = "\u265E";
            }
        }
};

struct rook_t : piece_t {
    

    array_coords legal_moves();

    rook_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {
            id = "r";
            points = 500;
            if (color == 0){
                display = "\u2656";
            }
            if (color == 1){
                display = "\u265C";
            }
        }
};

struct king_t : piece_t {
    array_coords legal_moves();

    bool is_checked();

    king_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {
            id = "k";
            points = 0;
            if (color == 0){
                display = "\u2654";
            }
            if (color == 1){
                display = "\u265A";
            }
        }
};

struct queen_t : piece_t {
    array_coords legal_moves();

    queen_t(coords location, bool color, board_t* board)
        : piece_t(location, color, board) {
            id = "q";
            points = 900;
            if (color == 0){
                display = "\u2655";
            }
            if (color == 1){
                display = "\u265B";
            }
        }
};

#endif