#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <iostream>
#include <vector>
#include "piece.hpp"
#include "macro.hpp"

struct move_t {
	coords before;
	coords after;
	//-100 - if it is on the same color
	// -1 - if it is a capture
	// 0 - if it is a normal move
	// 1 - if it is a castle
	// -2 - if it is en passant
	// 21 - if pawn moves 1 up - counts also as normal moves
	// 22 - if pawn moves 2 up - counts also as normal moves
	// 3 - if it is promotion
	// 30 - promotion -> queen
	// 31 - promotion -> rook
	// 32 - promotion -> bishop
	// 33 - promotion -> knight
	int type_move; 
	int score = 0;
	int eval = 0;
	

	move_t(int before_i, int before_j, int after_i, int after_j, int type_move)
		: before {makep(before_i,before_j)}, after {makep(after_i, after_j)}, type_move {type_move} {}
	
	move_t(coords before, coords after)
		: before {before}, after {after}, type_move {0} {}

	move_t(coords before, coords after, int type_move)
		: before {before}, after {after}, type_move {type_move} {}

	move_t()
		: before {makep(-10,-10)}, after {makep(-10,-10)}, type_move {-10} {} //not initialized
};

struct piece_t;

struct board_t {
	piece_t* state[SIZE][SIZE];	// board state
	std::map<piece_t*,coords> current_pieces;
	bool turn;					// 1 = white turn, 0 = black turn
	int fifty_moves;			// amount of moves into the fifty move rule
	move_t* last_move; 			//last move from what square to what square
	int grid_attack[2][SIZE][SIZE]; //An integer for every square, the amount of 0 - white, 1 - black piece targetting it
	//int grid_black[SIZE][SIZE]; //An integer for every square, the amount of white piece targetting it
	piece_t* king[2]; // 0 - black kings, 1 - white king

	//resets/updates the grids above
	void update_grids(move_t* move);
	piece_t* find_first_piece(int x, int y, int dx, int dy); // helper function

	/* Makes a starting chess board */
	board_t();

	/* For debugging: clearing the board */
	void clear_board();
	void add_piece(int x, int y, piece_t* piece);
	void remove_piece(int x, int y);
	void print_grids(bool color); //print the grid with respect to "color", true == white perspective

	/* Prints the board sexily */
	void print();

	std::string print_string();

	/* Import the history by one move */
	void update(std::string move, bool change_turn);

	void update_with_move(move_t* move, bool change_turn);

	void undo(std::string move, piece_t* captured, bool change_turn);

	void undo_with_move(move_t* move, piece_t* captured, bool change_turn);

	/* If in checks */
	bool is_check(bool color, piece_t* captured, move_t* current_move);

	bool check_valid_move_with_check(move_t* current_move);

	long long nb_moves(int depth);

	/* Generates all moves */
	array_moves generate_all_moves();
	

	/* Evaluates the current position. Positive score means white is winning, negative score means black */
	int eval();

	/* Search function */
	move_t search();

	/* Calculates the next move */
	std::string next_move(); //idk if we want to have any other arguments in or not

	/* Turns a location (i, j) to a string (ex: (0, 0) maps to "a1") */
	std::string move_to_string(move_t* move);
	/* Turns a string to a location (i, j) (ex: "a1" maps to (0, 0)) */
	move_t* string_to_move(std::string move);

	board_t copy(); //create a copy of the board
};

#endif