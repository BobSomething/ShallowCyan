#include "bitboard.hpp"

std::map<std::pair<std::string,bool>,int> convertPiece = {
    {makep("p",1), 0},
    {makep("n",1), 1},
    {makep("b",1), 2},
    {makep("r",1), 3},
    {makep("q",1), 4},
    {makep("k",1), 5},
    {makep("p",0), 6},
    {makep("n",0), 7},
    {makep("b",0), 8},
    {makep("r",0), 9},
    {makep("q",0), 10},
    {makep("k",0), 11},
};

std::map<int, std::string> coords_to_letter_2 = { // Dictionary to translate input to move
	{0, "a"},
	{1, "b"},
	{2, "c"},
	{3, "d"},
	{4, "e"},
	{5, "f"},
	{6, "g"},
	{7, "h"}
};

std::map<int,std::string> index_to_piece {
    {12, " "},
    {0, "\u265F"},
    {1, "\u265E"},
    {2, "\u265D"},
    {3, "\u265C"},
    {4, "\u265B"},
    {5, "\u265A"},
    {6, "\u2659"},
    {7, "\u2658"},
    {8, "\u2657"},
    {9, "\u2656"},
    {10, "\u2655"},
    {11, "\u2654"},
};


bitboard_t::bitboard_t()
    : board_t() {
        for(int i=0; i<SIZE; i++) {
            for(int j=0; j<SIZE; j++) {
                if(state[i][j] != nullptr) {
                    piecesBB[convertPiece[makep(state[i][j]->id, state[i][j]->color)]] |= 1ULL << (8*i+j);
                }
            }
        }
    }

void bitboard_t::printBBpiece(int index) {
    for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				std::cout << "\033[43m";
			else
				std::cout << "\033[100m";

            std::cout << " " << ((piecesBB[index] & (1ULL << (8*i+j))) ? 1 : 0) << " ";
		}
		std::cout << "\033[40m" << std::endl;
	}
	std::cout << "  ";
	for (int j = 0; j < SIZE; j++) {
		std::cout << " " + coords_to_letter_2[j] + " ";
	}
	std::cout << " " << std::endl;
    std::cout << "Bitboard: " << piecesBB[index] << std::endl;
}

void bitboard_t::printBB() {
    for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				std::cout << "\033[43m";
			else
				std::cout << "\033[100m";

            int index = 0;
            for(index; index < 12; index++) {
                if(piecesBB[index] & (1ULL << (8*i+j)))
                    break;
            }
            std::cout << " " << index_to_piece[index] << " ";
		}
		std::cout << "\033[40m" << std::endl;
	}
	std::cout << "  ";
	for (int j = 0; j < SIZE; j++) {
		std::cout << " " + coords_to_letter_2[j] + " ";
	}
	std::cout << " " << std::endl;
}