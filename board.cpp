#include "board.hpp"
#include "piece.hpp"

board_t::board_t() {
	// DONE (probably, has not been tested. also, uses "new")
	/* Makes a starting chess board */
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			state[i][j] = nullptr;
			bool color=0;
			if (i == 0 || i == 1) {color = 1;}
			if (i == 0 || i == 7) {
				piece_t* piece;
				if (j == 0 || j == 7){
						piece = new rook_t(coords(i, j), color, this);
				}
				if (j == 1 || j == 6){
						piece = new knight_t(coords(i, j), color, this);
				}
				if (j == 2 || j == 5){
						piece = new bishop_t(coords(i, j), color, this);
				}
				if (j == 3){
						piece = new queen_t(coords(i, j), color, this);
				}
				if (j == 4){
						piece = new king_t(coords(i, j), color, this);
				}
				state[i][j] = piece;
			}
			if (i == 1 || i == 6) {
				state[i][j] = new pawn_t(coords(i, j), color, this);
			}
		}
	}
	turn = 1;
	fifty_moves = 0;
}

void board_t::clear_board() {
	for(int i=0; i<SIZE; i++){ 
		for(int j=0; j<SIZE; j++){
			state[i][j] = nullptr;
		}
	}
}

void board_t::add_piece(int x, int y, piece_t* piece) {
	state[x][y] = piece;
}


void board_t::print() {
	// TODO
	/* Prints the board sexily */
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << "|";
			if (state[i][j] == nullptr){
				std::cout << " ";
			}
			if (state[i][j] != nullptr){
				std::cout << state[i][j]->display;
			}
		}
		std::cout << "|" << std::endl;
	}
}

void board_t::update(std::string move) {
	//TODO
	/* Updates the board with this move, assumes that the move is legal */
	std::map<std::string, int> letter_to_coords = { // Dictionary to translate input to move
		{"a" , 0},
		{"b" , 1},
		{"c" , 2},
		{"d" , 3},
		{"e" , 4},
		{"f" , 5},
		{"g" , 6},
		{"h" , 7}
	};
	//Define new position and old position
	int og_pos_j = letter_to_coords[move.substr(0,1)];
	int new_pos_j = letter_to_coords[move.substr(2,1)];
	int og_pos_i = 8 - stoi(move.substr(1,1));
	int new_pos_i = 8 - stoi(move.substr(3,1));
	//Update the board
	state[new_pos_i][new_pos_j] = state[og_pos_i][og_pos_j];
	state[og_pos_i][og_pos_j] = nullptr;
}

int board_t::eval() {
	// DONE (for now, we will change how boards are evaluated later)
	/* Evaluates the current position. Positive score means white is winning, negative score means black */
	int ret;
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