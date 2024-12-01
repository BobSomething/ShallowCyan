#include "board.hpp"

board_t::board_t() {
	// DONE (probably, has not been tested. also, uses "new")
	/* Makes a starting chess board */
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			bool color{};
			if (i == 0 || i == 1) color = 1;
			if (i == 0 || i == 7) {
				piece_t* piece;
				switch (j) {
					case 0:
					case 7:
						piece = new rook_t(coords{i, j}, color);
					case 1:
					case 6:
						piece = new knight_t(coords{i, j}, color);
					case 2:
					case 5:
						piece = new bishop_t(coords{i, j}, color);
					case 3:
						piece = new queen_t(coords{i, j}, color);
					case 4:
						piece = new king_t(coords{i, j}, color);
				}
				state[i][j] = piece;
			}
			if (i == 1 || i == 6) {
				state[i][j] = new pawn_t(coords{i, j}, color);
			}
		}
	}
	turn = 1;
	fifty_moves = 0;
}


void board_t::print() {
	// TODO
	/* Prints the board sexily */
	for (int i = 0; i < SIZE; i++) {
		std::cout << "a";
		for (int j = 0; j < SIZE; j++) {
			std::cout << "a";
		}
	}
}

void board_t::update(std::string move) {
	//TODO
	/* Updates the board with this move, assumes that the move is legal */

}

int board_t::eval() {
	// DONE (for now, we will change how boards are evaluated later)
	/* Evaluates the current position. Positive score means white is winning, negative score means black */
	int ret{};
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int indicator = -1;
			if (state[i][j]->color) indicator = 1;
			ret += state[i][j]->points * indicator;
		}
	}
	return ret;
}

std::string board_t::next_move() {
	//TODO
	/* Calculates the next move */
	return "a2a4"; //returning something
}