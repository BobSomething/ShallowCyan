#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <iostream>
#include <vector>
#include "piece.hpp"
#include "macro.hpp"

struct piece_t;

struct board_t {
	piece_t* state[SIZE][SIZE];	// board state
	bool turn;					// 1 = white turn, 0 = black turn
	int fifty_moves;			// amount of moves into the fifty move rule
	
	/* Makes a starting chess board */
	board_t();

	/* Prints the board sexily */
	void print();

	/* Evaluates the current position. Positive score means white is winning, negative score means black */
	int eval();

	/* Turns a location (i, j) to a string (ex: (0, 0) maps to "a1") */
	std::string pair_to_string(coords location);
	/* Turns a string to a location (i, j) (ex: "a1" maps to (0, 0)) */
	coords string_to_pair(std::string location);
};

#endif