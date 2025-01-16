#include "bitboard.hpp"

move_t* bitboard_t::search(int depth, int α, int β) {
	/* Alpha-beta pruning :) */
	move_t* ret = new move_t;
    if (depth == 0) { 
		ret->eval = this->eval();
		return ret;
	}

	if(no_moves()) {
		int king = (turn == 0) ? 11 : 5;
		int sign = (turn == 0) ? 1 : -1;
		if(is_square_attacked(get_LSB(piecesBB[king]),!turn)) ret->eval = sign*inf; //checkmate
		else ret->eval = 0; //stalemate
		return ret;
	}

    int val = (turn) ? -inf : inf;
	std::vector<move_t*> moves;
	generate_all_moves(&moves);
	ret->eval = val;
	for (move_t* move: moves) {
		int p_before = pieceTable[move->before.i*8 + move->before.j];
		int p_after = pieceTable[move->after.i*8 + move->after.j]; 
		int ep_square = enpassant_square;
		bool w_c_kside = w_castle_kside;
		bool w_c_qside = w_castle_qside; 
		bool b_c_kside = b_castle_kside;
		bool b_c_qside = b_castle_qside;

		//bitboard_t child = this->copy();
		//child.update(move);
		update(move);

		//int pval = child.search_aux(depth - 1, α, β).first;
		int pval = search(depth - 1, α, β)->eval;

		undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
		
		if (turn) {
			if (pval > val){
				val = pval;
				move->eval = val;
				ret = move;
			} 
			if (val > β) break;
			if (val > α) α = val;
		} else {
			if (pval < val){
				val = pval;
				move->eval = val;
				ret = move;
			} 
			if (val < α) break;
			if (val < β) β = val;
		}
	}
	return ret;
}