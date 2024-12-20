#include "bitboard.hpp"
#include "bithelpers.hpp"

array_coords bitboard_t::u64_to_coords(U64 u){
    std::vector<coords> ar = {};
    U64 u_copy = u;
    while (u_copy != 0){
        int index = get_LSB(u_copy);
        int i = index/8;
        int j = index%8;
        ar.push_back(makep(i,j));
        u_copy = u_copy&(u_copy-1);
    }
    return ar;
}

std::vector<int> bitboard_t::u64_to_index(U64 u){
    std::vector<int> ar = {};
    U64 u_copy = u;
    while (u_copy != 0){
        int index = get_LSB(u_copy);
        ar.push_back(index);
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

void bitboard_t::printBBattacked(bool color) {
    for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
            if (is_square_attacked(i*8+j, color)){
                std::cout << "\033[44m";
            }
            if (!is_square_attacked(i*8+j,color)) {
                if((i + j) % 2 == 0)
                    std::cout << "\033[43m";
                else
                    std::cout << "\033[100m";
            }
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


void bitboard_t::allMovesPawns(bool color, array_moves* moves){
    coords before, after;
    U64 same = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i]; 
    }

    U64 pawns_copy = color ? piecesBB[0] : piecesBB[6];
    while(pawns_copy) {
        index = get_LSB(pawns_copy);
        pawns_copy &= (pawns_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksPawns[color][index] & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }
        
        //en passant captures
        if(enpassant_square != -1) {
            if(attacksPawns[color][index] & (1 << enpassant_square)) {
                after.i = enpassant_square/8;
                after.j = enpassant_square%8;
                moves->push_back(new move_t(before,after,-2));
            }
        }

        int promotion_row = 6, direction = 1, first_row = 1;
        if(color == 0) {
            promotion_row = 1;
            first_row = 6;
            direction = -1;
        }
        // promotion
        if(before.j == promotion_row) {
            if(!get_bit(both, before.j+(before.i+direction)*8)) {
                after.i = before.i+direction;
                after.j = before.j;
                moves->push_back(new move_t(before, after, 4));
                moves->push_back(new move_t(before, after, 5));
                moves->push_back(new move_t(before, after, 6));
                moves->push_back(new move_t(before, after, 7));
                continue;
            }
        }
        if(!get_bit(both, before.j+(before.i+direction)*8)) {
            after.i = before.i+direction;
            after.j = before.j;
            moves->push_back(new move_t(before, after, 0)); // normal move
            if(before.j == first_row && !get_bit(both, before.j+(before.i+2*direction)*8)) {
                moves->push_back(new move_t(before, after, 3)); // double pawn push
            }
        }
    }
};

void bitboard_t::allMovesKing(bool color, array_moves* moves) {
    coords before, after;
    U64 same = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i]; 
    }


    U64 king_copy = color ? piecesBB[1] : piecesBB[7];
    while(king_copy) {
        index = get_LSB(king_copy);
        king_copy &= (king_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksKing[index] & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            else {
                m.type_move = 0;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }

        if(color == 1) {
            if(w_castle_kside) {
                if(!get_bit(both, before.j + 1) && !get_bit(both, before.j + 2)) {
                    if(!is_square_attacked(before.j + before.i*8,color) && !is_square_attacked(before.j+1 + before.i*8,color)) {
                        after.i = before.i;
                        after.j = before.j + 2;
                        moves->push_back(new move_t (after, before, 1));
                    }
                }
            }
            if(w_castle_qside) {
                if(!get_bit(both, before.j - 1) && !get_bit(both, before.j - 2) && !get_bit(both, before.j - 3)) {
                    if(!is_square_attacked(before.j + before.i*8,color) && !is_square_attacked(before.j-1 + before.i*8,color)) {
                        after.i = before.i;
                        after.j = before.j - 2;
                        moves->push_back(new move_t (after, before, 1));
                    }
                }
            }
        }
        else {
            if(b_castle_kside) {
                if(!get_bit(both, before.j + 1) && !get_bit(both, before.j + 2)) {
                    if(!is_square_attacked(before.j + before.i*8,color) && !is_square_attacked(before.j+1 + before.i*8,color)) {
                        after.i = before.i;
                        after.j = before.j + 2;
                        moves->push_back(new move_t (after, before, 1));
                    }
                }
            }
            if(b_castle_qside) {
                if(!get_bit(both, before.j - 1) && !get_bit(both, before.j - 2) && !get_bit(both, before.j - 3)) {
                    if(!is_square_attacked(before.j + before.i*8,color) && !is_square_attacked(before.j-1 + before.i*8,color)) {
                        after.i = before.i;
                        after.j = before.j - 2;
                        moves->push_back(new move_t (after, before, 1));
                    }
                }
            }
        }
    }
}


void bitboard_t::allMovesKnights(bool color, array_moves* moves){
    coords before, after;
    U64 same = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }

    U64 knights_copy = color ? piecesBB[1] : piecesBB[7];
    while(knights_copy) {
        index = get_LSB(knights_copy);
        knights_copy &= (knights_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksKnights[index] & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            else {
                m.type_move = 0;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }
    }
};

//Bishop, basically copy paste from knight
void bitboard_t::allMovesBishop(bool color, array_moves* moves){
    coords before, after;
    U64 same = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i]; 
    }
    

    U64 bishop_copy = color ? piecesBB[2] : piecesBB[8];
    while(bishop_copy) {
        index = get_LSB(bishop_copy);
        bishop_copy &= (bishop_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksBishopsMagic(index, both) & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            else {
                m.type_move = 0;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }
    }
    /*coords before;
    coords after;
    array_moves ar;
    U64 same = 0;
    int shift = color ? 0 : 6;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }

    U64 bishops_copy = color ? piecesBB[2] : piecesBB[8];
    while(bishops_copy) {
        int index = get_LSB(bishops_copy);

        bishops_copy &= (bishops_copy - 1);
        before.i = index/8;
        before.j = index%8;
        U64 u = attacksBishopsMagic(index, (~same)) & (~same);
        array_coords array = u64_to_coords(u);
        for (int k=0; k<array.size(); k++){
            after = array[k];
            move_t m (before, after);
            ar.push_back(&m);
        }
    }*/
};

void bitboard_t::allMovesRooks(bool color, array_moves* moves){
    coords before, after;
    U64 same = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i]; 
    }
    

    U64 rook_copy = color ? piecesBB[3] : piecesBB[9];
    while(rook_copy) {
        index = get_LSB(rook_copy);
        rook_copy &= (rook_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksRooksMagic(index, both) & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            else {
                m.type_move = 0;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }
    }
    /* coords before;
    coords after;
    array_moves ar;
    U64 same = 0;
    int shift = color ? 0 : 6;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }

    U64 rooks_copy = color ? piecesBB[3] : piecesBB[9];
    while(rooks_copy) {
        int index = get_LSB(rooks_copy);

        rooks_copy &= (rooks_copy - 1);
        before.i = index/8;
        before.j = index%8;
        U64 u = attacksRooksMagic(index, (~same)) & (~same);
        array_coords array = u64_to_coords(u);
        for (int k=0; k<array.size(); k++){
            after = array[k];
            move_t m (before, after);
            ar.push_back(&m);
        }
    } */
};

void bitboard_t::allMovesQueens(bool color, array_moves* moves){
    coords before, after;
    U64 same = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i]; 
    }
    

    U64 queen_copy = color ? piecesBB[4] : piecesBB[10];
    while(queen_copy) {
        index = get_LSB(queen_copy);
        queen_copy &= (queen_copy - 1);
        
        before.i = index/8;
        before.j = index%8;

        attacks = attacksQueensMagic(index, both) & (~same);
        while(attacks) {
            index = get_LSB(attacks);
            after.i = index/8;
            after.j = index%8;
            move_t m (before,after);
            if(get_bit(~same,index)) { // A capture move
                m.type_move = -1;
                moves->push_back(&m);
            }
            else {
                m.type_move = 0;
                moves->push_back(&m);
            }
            attacks &= (attacks - 1);
        }
    }

    /* coords before;
    coords after;
    array_moves ar;
    U64 same = 0;
    int shift = color ? 0 : 6;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }

    U64 queens_copy = color ? piecesBB[4] : piecesBB[10];
    while(queens_copy) {
        int index = get_LSB(queens_copy);

        queens_copy &= (queens_copy - 1);
        before.i = index/8;
        before.j = index%8;
        U64 u = attacksQueensMagic(index, (~same)) & (~same);
        array_coords array = u64_to_coords(u);
        for (int k=0; k<array.size(); k++){
            after = array[k];
            move_t m (before, after);
            ar.push_back(&m);
        }
    } */
};

bool bitboard_t::is_square_attacked(int square, bool color){
    int shift = color ? 0 : 6;
    /* U64 same = 0;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    } */
    U64 both = 0;
    for(int i = shift; i<12; i++) {
        both |= piecesBB[i];
    }
    U64 attacking = 0;
    if(attacksPawns[color^1][square] & piecesBB[shift])
        return true;
    if(attacksKnights[square] & piecesBB[shift+1]) {
        return true;
    }
    if(attacksBishopsMagic(square,both) & (piecesBB[shift+2] | piecesBB[shift+4]))
        return true;
    if(attacksRooksMagic(square,both) & (piecesBB[shift+3] | piecesBB[shift+4]))
        return true;
    if(attacksKing[square] & piecesBB[shift+5])
        return true;
    return false;

    /* for(int j = shift; j<6+shift; j++) {
        std::vector<int> ar = u64_to_index(piecesBB[j]);
        if (j%6 == 0){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksPawns_mask(ar[i], (color));
            }
        }
        if (j%6 == 1){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksKnight_mask(ar[i]);
            }
        }
        if (j%6 == 2){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksBishopsMagic(ar[i], (~same));
            }
        }
        if (j%6 == 3){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksRook_mask(ar[i], (~same));
            }
        }
        if (j%6 == 4){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksQueen_mask(ar[i], (~same));
            }
        }
        if (j%6 == 5){
            for (int i=0; i<ar.size(); i++){
                attacking |= attacksKing_mask(ar[i]);
            }
        }
    }
    return get_bit(attacking, square); */

};


