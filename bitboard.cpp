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
    hash_current_board = zobrist_board();
    counter_hash_map[hash_current_board]++;
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

    //update the Zobrist hash
    counter_hash_map[hash_current_board]--;
    hash_current_board = zobrist_update(move);

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
    //updating the zobrist-hash at every update
    hash_current_board = zobrist_update(move);
    counter_hash_map[hash_current_board]++;


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

//seaching
std::string bitboard_t::next_move() {
    std::time_t tracker = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return move_to_string(search(4, -inf, inf, tracker));
    //move_t* next = search(4);
    //std::cout << "Evaluation: " << next->eval << std::endl;
    //return move_to_string(next);
}

//calculates the game phase
int bitboard_t::game_phase() {
    int all_score = 0;
    
    for(int i=1; i<=4; i++)
        all_score += get_count(piecesBB[i]) * material_pieces[0][i];
    for(int i=7; i<=10; i++)
        all_score += get_count(piecesBB[i]) * -material_pieces[0][i];
    return all_score;
}

int bitboard_t::eval() {
    //three-fold
    if (counter_hash_map[hash_current_board] >= 3){
		return 0;
	}

    //0 - opening, 2 - midgame, 1 - endgame
    int phase = 0;
    int game_phase_score = game_phase();
    if (game_phase_score < opening_phase_score) phase+=2;
    if (game_phase_score < ending_phase_score) phase--;
    
    int total = 0;

    for (int i=0; i<SIZESQ; ++i) {
        int piece = this->pieceTable[i];

        //mirrored
        int mirrored = ((8-(i/8))*8)-(8-i%8);

        U64 sameColor = 0;
        int shift = (turn) ? 0 : 6;

        if (piece == -1) continue;

        if(phase == 2) { //midgame
            total += (
                    material_pieces[0][piece] * game_phase_score +
                    material_pieces[1][piece] * (opening_phase_score - game_phase_score)
                ) / opening_phase_score;
        }
        else total += material_pieces[phase][piece];

        switch(piece) {
            case 1 ... 3: //white knights bishops rooks
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece][0][i] * game_phase_score +
                        scorePiecePositional[piece][1][i] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total += scorePiecePositional[piece][0][i];
                break;
            
            case 7 ... 9: //black knights bishops rooks
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece-6][0][mirrored] * game_phase_score +
                        scorePiecePositional[piece-6][1][mirrored] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total -= scorePiecePositional[piece-6][0][mirrored];
                break;
            
            case 0: //white pawn
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece][0][i] * game_phase_score +
                        scorePiecePositional[piece][1][i] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total += scorePiecePositional[piece][phase][i];
                break;
            
            case 6: //black pawn
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece-6][0][mirrored] * game_phase_score +
                        scorePiecePositional[piece-6][1][mirrored] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total -= scorePiecePositional[piece-6][phase][mirrored];
                break;
            
            case 4: //white queen
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece][0][i] * game_phase_score +
                        scorePiecePositional[piece][1][i] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total += scorePiecePositional[piece][phase][i];
                break;

            case 10: //black queen
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece-6][0][mirrored] * game_phase_score +
                        scorePiecePositional[piece-6][1][mirrored] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total -= scorePiecePositional[piece-6][phase][mirrored];
                break;

            case 5: //white king
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece][0][i] * game_phase_score +
                        scorePiecePositional[piece][1][i] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total += scorePiecePositional[piece][phase][i];

                //king safety
                for (int p=shift; p < shift + 6; p++) sameColor |= piecesBB[p];
                total += get_count(attacksKing[i] & sameColor) * 4;
                
                break;

            case 11: //black king
                if(phase == 2) {
                    total += (
                        scorePiecePositional[piece-6][0][mirrored] * game_phase_score +
                        scorePiecePositional[piece-6][1][mirrored] * (opening_phase_score - game_phase_score)
                    ) / opening_phase_score;
                }
                else
                    total -= scorePiecePositional[piece-6][phase][mirrored];

                //king safety
                for (int p=shift; p < shift + 6; p++) sameColor |= piecesBB[p];
                total -= get_count(attacksKing[i] & sameColor) * 4;
                break;
        }
    }
    return total;
}

int bitboard_t::score_move(move_t* move){
    //killer moves
    int score = 0;
    int square_before = move->before.i*8+move->before.j;
    int move_piece_type = pieceTable[move->before.i*8+move->before.j];
    int captured_piece_type = pieceTable[move->after.i*8+move->after.j];

    if (captured_piece_type != -1){     //Captured something
        int shift = (turn) ? 6 : 0;
        U64 allPieces = 0;
        U64 oppPieces = 0;
        for(int p=0; p<12; p++) 
            allPieces |= piecesBB[p];
        for(int p=shift; p<shift+6; p++) 
            oppPieces |= piecesBB[p];

        //if recapture
        U64 oppAttacks = attacksKing[square_before] | attacksPawns[turn][square_before] | attacksKnights[square_before] | attacksBishops[square_before][allPieces] | attacksRooks[square_before][allPieces];
        if(get_bit(oppAttacks & oppPieces, square_before))
            score += MVV_LVA[move_piece_type][captured_piece_type] +
            (pieces_to_points[captured_piece_type] >= pieces_to_points[move_piece_type]) ? 600 : 100;
        else
            score += MVV_LVA[move_piece_type][captured_piece_type] + 600;
    }
    else {
        if(killer_moves[current_depth][0] == move) {
            score += 2000;
        }
        else if(killer_moves[current_depth][1] == move) {
            score += 1000;
        }
        score += history_moves[move_piece_type][move->after.i*8+move->after.j];
    }
    
    bool color = (move_piece_type < 6) ? 1 : 0;
    if (is_square_attacked(move->after.i*8+move->after.j, !color)){
        score -= pieces_to_points[move_piece_type];
    }
    if (is_square_attacked(move->before.i*8+move->before.j, !color)){
        score += pieces_to_points[move_piece_type]/3;
    }

    int p_before = pieceTable[move->before.i*8 + move->before.j];
    int p_after = pieceTable[move->after.i*8 + move->after.j]; 
    int ep_square = enpassant_square;
    bool w_c_kside = w_castle_kside;
    bool w_c_qside = w_castle_qside; 
    bool b_c_kside = b_castle_kside;
    bool b_c_qside = b_castle_qside;
    update(move);
    int king = (turn == 0) ? 11 : 5;
    if (is_square_attacked(get_LSB(piecesBB[king]),!turn)){
        score += std::max(80,score/20);
    }
    undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);

    //move->score = score;
    return score;
}