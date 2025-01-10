#include "bitboard.hpp"

void bitboard_t::allMovesPawns(bool color, array_moves* moves){
    coords before, after;
    move_t* new_move;
    U64 different = 0, both = 0, attacks;
    int shift = color ? 6 : 0, index;
    for(int i = shift; i<6+shift; i++) {
        different |= piecesBB[i];
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }

    U64 pawns_copy = color ? piecesBB[0] : piecesBB[6];
    while(pawns_copy) {
        index = get_LSB(pawns_copy);
        
        
        before.i = index/8;
        before.j = index%8;

        int promotion_row = 6, direction = 1, first_row = 1;
        if(color == 0) {
            promotion_row = 1;
            first_row = 6;
            direction = -1;
        }

        attacks = attacksPawns[color][index] & (different);

        int id;
        while(attacks) {
            id = get_LSB(attacks);
            after.i = id/8;
            after.j = id%8;
            if(get_bit(different,id)) { // A capture move
                if(before.i == promotion_row && is_legal(new_move = new move_t(before, after, -7, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) {
                    moves->push_back(new_move);
                    moves->push_back(new move_t(before, after, -8, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]));
                    moves->push_back(new move_t(before, after, -9, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]));
                    moves->push_back(new move_t(before, after, -10, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]));
                }
                else if(before.i != promotion_row && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j])))
                    moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }
        
        //en passant captures
        if(enpassant_square != -1) {
            after.i = enpassant_square/8;
            after.j = enpassant_square%8;
            if(get_bit(attacksPawns[color][index],enpassant_square) && is_legal(new_move = new move_t(before, after, -2, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) {
                moves->push_back(new_move);
            }
        }
        pawns_copy &= (pawns_copy - 1);

        // promotion
        if(before.i == promotion_row) {
            after.i = before.i+direction;
            after.j = before.j;
            if(!get_bit(both, before.j+(before.i+direction)*8) && is_legal(new_move = new move_t(before, after, 7, pieceTable[(before.i *8)+before.j]))) {
                moves->push_back(new_move);
                moves->push_back(new move_t(before, after, 8, pieceTable[(before.i *8)+before.j]));
                moves->push_back(new move_t(before, after, 9, pieceTable[(before.i *8)+before.j]));
                moves->push_back(new move_t(before, after, 10, pieceTable[(before.i *8)+before.j]));
                continue;
            }
        }
        after.i = before.i+direction;
        after.j = before.j;
        if(!get_bit(both, before.j+(before.i+direction)*8)) {
            if(is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j])))
                moves->push_back(new_move); // normal move
            after.i = before.i+2*direction;
            if(before.i == first_row && !get_bit(both, before.j+(before.i+2*direction)*8) && is_legal(new_move = new move_t(before, after, 3, pieceTable[(before.i *8)+before.j]))) {
                moves->push_back(new_move); // double pawn push
            }
        }
    }
}

void bitboard_t::allMovesKing(bool color, array_moves* moves) {
    coords before, after;
    move_t* new_move;
    U64 same = 0, different = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }
    different = both ^ same;


    U64 king_copy = color ? piecesBB[5] : piecesBB[11];
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
            if(get_bit(different,index) && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) { // A capture move
                moves->push_back(new_move);
            }
            else if(!get_bit(different,index) && is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j]))){
                moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }

        //castle
        if(color == 1) {
            if(w_castle_kside) {
                if(!get_bit(both, 5) && !get_bit(both, 6)) {
                    after.i = before.i;
                    after.j = before.j + 2;
                    if(!is_square_attacked(before.j + before.i*8,!color) && !is_square_attacked(before.j+1 + before.i*8,!color) \
                    && is_legal(new_move = new move_t(before, after, 1, pieceTable[(before.i *8)+before.j]))) {
                        moves->push_back(new_move);
                    }
                }
            }
            if(w_castle_qside) {
                if(!get_bit(both, 3) && !get_bit(both, 2) && !get_bit(both, 1)) {
                    after.i = before.i;
                    after.j = before.j - 2;
                    if(!is_square_attacked(before.j + before.i*8,!color) && !is_square_attacked(before.j-1 + before.i*8,!color) \
                    && is_legal(new_move = new move_t(before, after, 1, pieceTable[(before.i *8)+before.j]))) {
                        moves->push_back(new_move);
                    }
                }
            }
        }
        else {
            if(b_castle_kside) {
                if(!get_bit(both, 61) && !get_bit(both, 62)) {
                    after.i = before.i;
                    after.j = before.j + 2;
                    if(!is_square_attacked(before.j + before.i*8,!color) && !is_square_attacked(before.j+1 + before.i*8,!color) \
                    && is_legal(new_move = new move_t(before, after, 1, pieceTable[(before.i *8)+before.j]))) {
                        moves->push_back(new_move);
                    }
                }
            }
            if(b_castle_qside) {
                if(!get_bit(both, 59) && !get_bit(both, 58) && !get_bit(both, 57)) {
                    after.i = before.i;
                    after.j = before.j - 2;
                    if(!is_square_attacked(before.j + before.i*8,!color) && !is_square_attacked(before.j-1 + before.i*8,!color) \
                    && is_legal(new_move = new move_t(before, after, 1, pieceTable[(before.i *8)+before.j]))) {
                        moves->push_back(new_move);
                    }
                }
            }
        }
    }
}


void bitboard_t::allMovesKnights(bool color, array_moves* moves){
    coords before, after;
    move_t* new_move;
    U64 same = 0, different = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i];
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }
    different = both ^ same;

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
            if(get_bit(different,index) && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) { // A capture move
               moves->push_back(new_move);
            }
            else if(!get_bit(different,index) && is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j]))){
                moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }
    }
};

//Bishop, basically copy paste from knight
void bitboard_t::allMovesBishop(bool color, array_moves* moves){
    coords before, after;
    move_t* new_move;
    U64 same = 0, different = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }
    different = both ^ same;

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
            if(get_bit(different,index) && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) { // A capture move
                moves->push_back(new_move);
            }
            else if(!get_bit(different,index) && is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j]))){
                moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }
    }
}

void bitboard_t::allMovesRooks(bool color, array_moves* moves){
    coords before, after;
    move_t* new_move;
    U64 same = 0, different = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }
    different = both ^ same;
    

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
            if(get_bit(different,index) && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) { // A capture move
                moves->push_back(new_move);
            }
            else if (!get_bit(different,index) && is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j]))) {
                moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }
    }
};

void bitboard_t::allMovesQueens(bool color, array_moves* moves){
    coords before, after;
    move_t* new_move;
    U64 same = 0, different = 0, both = 0, attacks;
    int shift = color ? 0 : 6, index;
    for(int i = shift; i<6+shift; i++) {
        same |= piecesBB[i]; 
    }
    for(int i = 0; i<12; i++) {
        both |= piecesBB[i]; 
    }
    different = both ^ same;
    

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
            //move_t* new_move2 = new move_t(before, after, 0);
            if(get_bit(different,index) && is_legal(new_move = new move_t(before, after, -1, pieceTable[(before.i *8)+before.j], pieceTable[(after.i *8)+after.j]))) { // A capture move
                moves->push_back(new_move);
            }
            else if(!get_bit(different,index) && is_legal(new_move = new move_t(before, after, 0, pieceTable[(before.i *8)+before.j]))){
                moves->push_back(new_move);
            }
            attacks &= (attacks - 1);
        }
    }
}

void bitboard_t::generate_all_moves(array_moves* moves) {
    allMovesPawns(turn, moves);
    allMovesKnights(turn, moves);
    allMovesBishop(turn, moves);
    allMovesRooks(turn, moves);
    allMovesQueens(turn, moves);
    allMovesKing(turn, moves);
}
