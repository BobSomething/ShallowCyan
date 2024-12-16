#include "bitboard.hpp"
#include "bithelpers.hpp"

array_coords bitboard_t::u64_to_coords(U64 u){
    std::vector<coords> ar = {};
    U64 u_copy = u;
    while (u_copy != 0){
        int i = get_LSB(u_copy)/8;
        int j = get_LSB(u_copy)%8;
        ar.push_back(makep(i,j));
        u_copy = u_copy&(u_copy-1);
    }
    return ar;
}

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

std::map<std::string, int> letter_to_coords_2 = { // Dictionary to translate input to move
	{"a" , 0},
	{"b" , 1},
	{"c" , 2},
	{"d" , 3},
	{"e" , 4},
	{"f" , 5},
	{"g" , 6},
	{"h" , 7}
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




/*

IMPLEMENTATION OF BITBOARDS

*/


bitboard_t::bitboard_t() {
    
    pieceTable[0] = 3;
    pieceTable[1] = 1;
    pieceTable[2] = 2;
    pieceTable[3] = 4;
    pieceTable[4] = 5; kingWhere[0] = 4;
    pieceTable[5] = 2;
    pieceTable[6] = 1;
    pieceTable[7] = 3;
    for(int i=8; i<=15; i++)
        pieceTable[i] = 0;

    for(int i=16; i<=47; i++)
        pieceTable[i] = -1;
    
    for(int i=48; i<=55; i++)
        pieceTable[i] = 6;
    pieceTable[56] = 9;
    pieceTable[57] = 7;
    pieceTable[58] = 8;
    pieceTable[59] = 10;
    pieceTable[60] = 11; kingWhere[1] = 60;
    pieceTable[61] = 8;
    pieceTable[62] = 7;
    pieceTable[63] = 9;

    piecesBB[0] = 0xFF00ULL; //white pawns
    piecesBB[1] = 0x42ULL; //white knights
    piecesBB[2] = 0x24ULL; //white bishops 
    piecesBB[3] = 0x81ULL; //white rooks
    piecesBB[4] = 0x8ULL; //white queens
    piecesBB[5] = 0x10ULL; //white king

    piecesBB[6] = 0xFF000000000000ULL; //black pawns
    piecesBB[7] = 0x4200000000000000ULL; //black knights
    piecesBB[8] = 0x2400000000000000ULL; //black bishops
    piecesBB[9] = 0x8100000000000000ULL; //black rooks
    piecesBB[10] = 0x0800000000000000ULL; //black queens
    piecesBB[11] = 0x1000000000000000ULL; //black king

    turn = 1;

    for(int square=0; square<SIZESQ; square++) {
        attacksPawns[0][square] = attacksPawns_mask(square, 0);
        attacksPawns[1][square] = attacksPawns_mask(square, 1);

        attacksKing[square] = attacksKing_mask(square);
        attacksKnights[square] = attacksKnight_mask(square);

        //bishop
        attacksBishopsNoBlockers[square] = attacksBishop_mask_short(square);

        U64 attack_mask = attacksBishopsNoBlockers[square];
        int n = bishopShifts[square];
        for(int index = 0; index < (1 << n); index++) {
            U64 occupied = get_set_with_index(index, attack_mask);
            int key = (occupied*magicBishops[square]) >> (64 - bishopShifts[square]);
            attacksBishops[square][key] = attacksBishop_mask(square, occupied);
        }

        //rook
        attacksRooksNoBlockers[square] = attacksRook_mask_short(square);

        attack_mask = attacksRook_mask_short(square);
        n = rookShifts[square];
        for(int index = 0; index < (1 << n); index++) {
            U64 occupied = get_set_with_index(index, attack_mask);
            int key = (occupied*magicRooks[square]) >> (64 - rookShifts[square]);
            attacksRooks[square][key] = attacksRook_mask(square, occupied);
        }
    }

}


/* Prints the specific piece bitboard */
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

/* Prints any bitboard with the input as a uint64_t */
void bitboard_t::printBBany(U64 u64) {
    for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				std::cout << "\033[43m";
			else
				std::cout << "\033[100m";

            std::cout << " " << ((u64 & (1ULL << (8*i+j))) ? 1 : 0) << " ";
		}
		std::cout << "\033[40m" << std::endl;
	}
	std::cout << "  ";
	for (int j = 0; j < SIZE; j++) {
		std::cout << " " + coords_to_letter_2[j] + " ";
	}
	std::cout << " " << std::endl;
    std::cout << "Bitboard: " << u64 << std::endl;
}

/* Print the whole board */
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

std::string bitboard_t::move_to_string(move_t* move) {
	std::string base = coords_to_letter_2[move->before.j] + std::to_string(move->before.i+1) + coords_to_letter_2[move->after.j] + std::to_string(move->after.i+1);
	//move->type_move == if it is promotion
	return base;
}



move_t* bitboard_t::string_to_move(std::string move) {
	int type = 0;
	if(move.length() == 5) {
		std::map<std::string, int> promotion = {
			{"q" , 0},
			{"r" , 1},
			{"b" , 2},
			{"n" , 3},
		};
		type = 30 + promotion[move.substr(4,1)];
	}
		
	move_t* new_move = new move_t{stoi(move.substr(1,1)) - 1, letter_to_coords_2[move.substr(0,1)], stoi(move.substr(3,1)) - 1, letter_to_coords_2[move.substr(2,1)],type};
	/*
	new_move->before.j = letter_to_coords[move->substr(0,1)];
	new_move->after.j = letter_to_coords[move->substr(2,1)];
	new_move->before.i = 8 - stoi(move->substr(1,1)); //stoi = string to integer
	new_move->after.i = 8 - stoi(move->substr(3,1));
	*/
	return new_move;
}

array_moves bitboard_t::allMovesKnights(bool color){
    coords before;
    coords after;
    array_moves ar;
    U64 black = 0x0000000000000000;
    U64 white = 0x0000000000000000;
    for (int i=6; i<11; i++){
        black |= piecesBB[i];
    }
    for (int i=0; i<5; i++){
        white |= piecesBB[i];
    }
    for (int j=0; j<63; j++){
        if (color == 0){
            if (pieceTable[j] == 7){
                before.i = j/8;
                before.j = j%8;
                U64 u = attacksKnight_mask(j) & (~black);
                array_coords array = u64_to_coords(u);
                for (int k=0; k<array.size(); k++){
                    after = array[k];
                    move_t m (before, after);
                    ar.push_back(&m);
                }
            }
        }
        if (color == 1){
            if (pieceTable[j] == 1){
                before.i = j/8;
                before.j = j%8;
                U64 u = attacksKnight_mask(j) & (~white);
                array_coords array = u64_to_coords(u);
                for (int k=0; k<array.size(); k++){
                    after = array[k];
                    move_t m (before, after);
                    ar.push_back(&m);
                }
            }
        }
    }
};


