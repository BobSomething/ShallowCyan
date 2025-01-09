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


std::ostream& operator << (std::ostream& os, const move_t& move) {
    std::map<int, std::string> ctl = { {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {4, "e"}, {5, "f"}, {6, "g"}, {7, "h"} };
    std::map<int,std::string> tf = { {0, "P"}, {1, "N"}, {2, "B"}, {3, "R"}, {4, "Q"}, {5, "K"}, {6, "p"}, {7, "n"}, {8, "b"}, {9, "r"}, {10, "q"}, {11, "k"} };

    std::map<int,std::string> tm = {
        {-1, "capture"},
        {0, "normal (quiet)"},
        {1, "castle"},
        {-2, "en passant"},
        {3, "double pawn push"},
        {7, "promotion to a knight"},
        {-7, "promotion to a knight with a capture"},
        {8, "promotion to a bishop"},
        {-8, "promotion to a knight with a capture"},
        {9, "promotion to a knight"},
        {-9, "promotion to a rook with a capture"},
        {10, "promotion to a queen"},
        {-10, "promotion to a queen with a capture"},
    };


    std::string base = ctl[move.before.j] + std::to_string(move.before.i+1) + ctl[move.after.j] + std::to_string(move.after.i+1);
	if (abs(move.type_move) >= 7)
        base += tf[abs(move.type_move)];
    os << "move: " << base << ", type: " << tm[move.type_move] << std::endl;
    return os;
}

/*

IMPLEMENTATION OF BITBOARDS

*/


bitboard_t::bitboard_t() {
    
    pieceTable[0] = 3;
    pieceTable[1] = 1;
    pieceTable[2] = 2;
    pieceTable[3] = 4;
    pieceTable[4] = 5;
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
    pieceTable[60] = 11;
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
    zobrist_init();

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
		std::cout << " " + coords_to_letter[j] + " ";
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
		std::cout << " " + coords_to_letter[j] + " ";
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
		std::cout << " " + coords_to_letter[j] + " ";
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
		std::cout << " " + coords_to_letter[j] + " ";
	}
	std::cout << " " << std::endl;
}

std::string bitboard_t::move_to_string(move_t* move) {
	std::string base = coords_to_letter[move->before.j] + std::to_string(move->before.i+1) + coords_to_letter[move->after.j] + std::to_string(move->after.i+1);
	if (abs(move->type_move) >= 7)
        base += tofen[abs(move->type_move)];
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
		
	move_t* new_move = new move_t{stoi(move.substr(1,1)) - 1, letter_to_coords[move.substr(0,1)], stoi(move.substr(3,1)) - 1, letter_to_coords[move.substr(2,1)],type};
	return new_move;
}


bool bitboard_t::is_square_attacked(int square, bool color){
    int shift = color ? 0 : 6;
    U64 both = 0;
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i];
    }
    if(attacksPawns[color^1][square] & piecesBB[shift]) {
        return true;
    }
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
};

void bitboard_t::undo(move_t* move, int p_before, int p_after, int ep_square, bool w_c_kside, bool w_c_qside, bool b_c_kside, bool b_c_qside) {
    int index_before = move->before.i*8 + move->before.j; 
    int index_after = move->after.i*8 + move->after.j;

    turn ^= 1;

    //undo en passant square and castling rights
    enpassant_square = ep_square;
    w_castle_kside = w_c_kside;
    w_castle_qside = w_c_qside;
    b_castle_kside = b_c_kside;
    b_castle_qside = b_c_qside;

    //undo the move
    clear_bit(piecesBB[p_before], index_after);
    set_bit(piecesBB[p_before], index_before);
    pieceTable[index_before] = p_before; 
    pieceTable[index_after] = -1;

    //if promotion
    if(abs(move->type_move) >= 7) {
        int shift = (turn == 1) ? -6 : 0;
        clear_bit(piecesBB[abs(move->type_move)+shift], index_after);
    }

    //if capture
    if(move->type_move == -1 || move->type_move <= -7) {
        set_bit(piecesBB[p_after], index_after);
        pieceTable[index_after] = p_after;
    }

    //if enpassant
    if(move->type_move == -2) {
        if(turn == 1) {
            set_bit(piecesBB[6],index_after-8)
            pieceTable[index_after-8] = 6; 
        }
        else {
            set_bit(piecesBB[0],index_after+8)
            pieceTable[index_after+8] = 0; 
        }
    }

    //castling - moving the rook
    if(move->type_move == 1) {
        if(turn == 1) {
            if(move->after.j == 6) { //king side
                clear_bit(piecesBB[3],5);
                set_bit(piecesBB[3],7);
                pieceTable[5] = -1;
                pieceTable[7] = 3;
            }
            else if(move->after.j == 2) { //queen side
                clear_bit(piecesBB[3],3);
                set_bit(piecesBB[3],0);
                pieceTable[3] = -1;
                pieceTable[0] = 3;
            }
        }
        else {
            if(move->after.j == 6) { //king side
                clear_bit(piecesBB[9],61);
                set_bit(piecesBB[9],63);
                pieceTable[61] = -1;
                pieceTable[63] = 9;
            }
            else if(move->after.j == 2) { //queen side
                clear_bit(piecesBB[9],59);
                set_bit(piecesBB[9],56);
                pieceTable[59] = -1;
                pieceTable[56] = 9;
            }
        }
    }
    
}

void bitboard_t::update_string(std::string move) {
    coords after, before;
    before.j = move[0] - 'a';
    before.i = move[1] - '1';
    after.j = move[2] - 'a';
    after.i = move[3] - '1';
    
    int index_before = before.i*8 + before.j; 
    int index_after = after.i*8 + after.j;

    //promotion
    if(move.size() == 5) {
        std::map<char,int> promoted_piece = {{'n', 7}, {'b', 8}, {'r', 9}, {'q', 10}};
        int promotion_type = promoted_piece[move[4]];
        if(pieceTable[index_after] != -1) promotion_type = -promotion_type; //if it is a promotion with a capture

        update(new move_t(before,after,promotion_type));
        return;
    }

    //double pawn push
    //if the piece is a pawn and it moved 2 squares => double pawn push
    if(pieceTable[index_before] == ((turn == 1) ? 0 : 6) && abs(after.i - before.i) == 2) {
        update(new move_t(before,after,3));
        return;
    }

    //en passant
    int enpassant_row = (turn == 1) ? 5 : 4;
    // if the piece is a pawn, and it attacks an empty square => en passant
    if(pieceTable[index_before] == ((turn == 1) ? 0 : 6) && before.j != after.j && pieceTable[index_after] == -1) {
        update(new move_t(before,after,-2));
        return;
    }

    //castle
    // if the piece is a king, and it moved 2 squares to the left/right => castle
    if(pieceTable[index_before] == ((turn == 1) ? 5 : 11) && abs(before.j - after.j) == 2) {
        update(new move_t(before,after,1));
        return;
    }
    
    //capture
    if(pieceTable[index_after] != -1) {
        update(new move_t(before,after,-1));
        return;
    }

    update(new move_t(before,after,0));
}

bool bitboard_t::update(move_t* move) {
    int index_before, index_after, piece_before, piece_after;
    index_before = move->before.i*8 + move->before.j; 
    index_after = move->after.i*8 + move->after.j;

    piece_before = pieceTable[index_before];
    piece_after = pieceTable[index_after];

    //storing everything, in case of check after the move
    int p_before = piece_before; 
    int p_after = piece_after;
    int ep_square = enpassant_square;
    bool w_c_kside = w_castle_kside;
    bool w_c_qside = w_castle_qside;
    bool b_c_kside = b_castle_kside;
    bool b_c_qside = b_castle_qside;

    //move the piece
    clear_bit(piecesBB[piece_before], index_before);
    set_bit(piecesBB[piece_before], index_after);
    pieceTable[index_before] = -1;
    pieceTable[index_after] = piece_before;

    
    
    //if capture
    if(move->type_move == -1 || move->type_move <= -7) {
        clear_bit(piecesBB[piece_after], index_after);

        //if capture rook - change the castling rights
        if(piece_after == 3) { //if white rook captured
            if(index_after == 0) w_castle_qside = 0;
            if(index_after == 7) w_castle_kside = 0;
        }   
        if(piece_after == 9) { //if black rook captured
            if(index_after == 56) b_castle_qside = 0;
            if(index_after == 63) b_castle_kside = 0;
        }
    }

    //if promotion
    if(abs(move->type_move) >= 7) {
        int shift = 0;
        if(turn == 1) shift = -6;
        clear_bit(piecesBB[piece_before], index_after);
        set_bit(piecesBB[abs(move->type_move)+shift], index_after);
        pieceTable[index_after] = abs(move->type_move)+shift;
    }

    //if enpassant
    if(move->type_move == -2) {
        if(turn == 1) {
            clear_bit(piecesBB[6],index_after-8);
            pieceTable[index_after-8] = -1;
        }
        else {
            clear_bit(piecesBB[0],index_after+8);
            pieceTable[index_after+8] = -1;
        }
    }

    enpassant_square = -1;

    //updating en passant square if there was a double pawn push
    if(move->type_move == 3) {
        (turn == 1) ? enpassant_square = index_after-8 : enpassant_square = index_after+8;
    }

    //castling - moving the rook
    if(move->type_move == 1) {
        if(turn == 1) {
            if(move->after.j == 6) { //king side
                set_bit(piecesBB[3],5);
                clear_bit(piecesBB[3],7);
                pieceTable[5] = 3;
                pieceTable[7] = -1;
            }
            else if(move->after.j == 2) { //queen side
                set_bit(piecesBB[3],3);
                clear_bit(piecesBB[3],0);
                pieceTable[3] = 3;
                pieceTable[0] = -1;
            }
        }
        else {
            if(move->after.j == 6) { //king side
                set_bit(piecesBB[9],61);
                clear_bit(piecesBB[9],63);

                pieceTable[61] = 9;
                pieceTable[63] = -1;
            }
            else if(move->after.j == 2) { //queen side
                set_bit(piecesBB[9],59);
                clear_bit(piecesBB[9],56);

                pieceTable[59] = 9;
                pieceTable[56] = -1;
            }
        }
    }
    

    //updating castling rights
    if(piece_before == 5) { //if white king moved
        w_castle_kside = 0;
        w_castle_qside = 0;
    }
    if(piece_before == 11) { //if black king moved
        b_castle_kside = 0;
        b_castle_qside = 0;
    }
    if(piece_before == 3) { //if white rook moved
        if(index_before == 0) w_castle_qside = 0;
        if(index_before == 7) w_castle_kside = 0;
    }   
    if(piece_before == 9) { //if black rook moved
        if(index_before == 56) b_castle_qside = 0;
        if(index_before == 63) b_castle_kside = 0;
    }

    turn ^= 1;
    //make sure it king is not exposed with a check at the end
    int king = (turn == 1) ? 11 : 5;
    if(is_square_attacked(get_LSB(piecesBB[king]),turn)) {
        undo(move, p_before, p_after, ep_square, w_c_kside, w_c_qside, b_c_kside, b_c_qside);
        return false;
    }
    
    return true;
}

bool bitboard_t::is_legal(move_t* move) {
    int p_before = pieceTable[move->before.i*8 + move->before.j];
    int p_after = pieceTable[move->after.i*8 + move->after.j]; 
    int ep_square = enpassant_square;
    bool w_c_kside = w_castle_kside;
    bool w_c_qside = w_castle_qside; 
    bool b_c_kside = b_castle_kside;
    bool b_c_qside = b_castle_qside;

    if(!update(move))
        return false;
    undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
    return true;
}

bool bitboard_t::no_moves() {
    array_moves moves;
    generate_all_moves(&moves);
    if(moves.size() == 0)
        return true;
    return false;
}

#include <ctime>
//For now we generate a random move from all legal moves
//After a while try search function
std::string bitboard_t::next_move() {
    if(nb_turns < 10) {
        array_moves moves;
        std::srand(std::time(0));
        generate_all_moves(&moves);
        int rand_pos = std::rand() % moves.size();
        return move_to_string(moves[rand_pos]);
    }
    else {
        return move_to_string(search(5));
    }
}

//create a copy of the current bitboard
bitboard_t bitboard_t::copy() {
    bitboard_t copy_board = bitboard_t();
    for (int i = 0; i<12; ++i) {
        copy_board.piecesBB[i] = this->piecesBB[i];
    }
    copy_board.turn = this->turn;
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.pieceTable[i] = this->pieceTable[i];
    }
    copy_board.enpassant_square = this->enpassant_square;
    copy_board.w_castle_kside = this->w_castle_kside;
    copy_board.w_castle_qside = this->w_castle_qside;
    copy_board.b_castle_kside = this->b_castle_kside;
    copy_board.b_castle_qside = this->b_castle_qside;
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksKing[i] = this->attacksKing[i];
    }
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksKnights[i] = this->attacksKnights[i];
    }
    for (int i=0; i< SIZESQ; ++i) {
        for(int j=0; j<2; ++j){
            copy_board.attacksPawns[j][i] = this->attacksPawns[j][i];
        }
    }
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksBishops[i] = this->attacksBishops[i];
    }
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksRooks[i] = this->attacksRooks[i];
    }
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksBishopsNoBlockers[i] = this->attacksBishopsNoBlockers[i];
    }
    for (int i=0; i< SIZESQ; ++i) {
        copy_board.attacksRooksNoBlockers[i] = this->attacksRooksNoBlockers[i];
    }
    return copy_board;
}

int bitboard_t::eval() {
    int total = 0;
    //can calculate how many pieces are left for white and black with & all the bitboards and use the get_count function
    //define a threshold of nb of pieces that we are in endgame: ...
    U64 nb_piece = 0;
    for (int i=1; i<12; i++){
        nb_piece |= piecesBB[i];
    }
    int nb_pieces = get_count(nb_piece);
    for (int i=0; i<SIZESQ; ++i) {
        int a = this->pieceTable[i];
        if (a==1) total += 300 + scoreKnights[i];
        if (a==2) total += 300 + scoreBishops[i];
        if (a==3) total += 500 + scoreRooks[i];
        if (a==4) total += 900 + scoreQueens[i];
        if (a==7) total -= 300 - scoreKnights[((8-(i/8))*8)-(8-i%8)];
        if (a==8) total -= 300 - scoreBishops[((8-(i/8))*8)-(8-i%8)];
        if (a==9) total -= 500 - scoreRooks[((8-(i/8))*8)-(8-i%8)];
        if (a==10) total -= 900- scoreQueens[((8-(i/8))*8)-(8-i%8)];
        if (nb_pieces > 10){
            if (a==0) total += 100 + scorePawnsOpening[i];
            if (a==5) total += scoreKingOpening[i];
            if (a==11) total += scoreKingOpening[((8-(i/8))*8)-(8-i%8)];
            if (a==6) total -= 100 - scorePawnsOpening[((8-(i/8))*8)-(8-i%8)];
        }
        if (nb_pieces < 11){
            if (a==0) total += 100 + scorePawnsEnding[i];
            if (a==5) total += scoreKingEnding[i];
            if (a==11) total += scoreKingEnding[((8-(i/8))*8)-(8-i%8)];
            if (a==6) total -= 100 - scorePawnsEnding[((8-(i/8))*8)-(8-i%8)];
        }

        //add up scores from score tables
        //remember to "flip" the board for black's turn
        //check if we are in endgame or not, if so remember to use Ending tables for kings and pawns
    }
    return total;
}