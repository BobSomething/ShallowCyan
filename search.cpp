#include "bitboard.hpp"

eval_move bitboard_t::search_aux(int depth, int α, int β) {
	/* Alpha-beta pruning :) */
	// for now, returns an int
	// MAKE A COPY FUNCTION (should be done but im keeping this in case its wrong so we know its important)
    if (depth == 0) { // OR CHECKMATE
		return eval_move(this->eval(),nullptr);
	}

	if(no_moves()) {
		int king = (turn == 0) ? 11 : 5;
		//checkmate
		if(is_square_attacked(get_LSB(piecesBB[king]),!turn)) {
			return eval_move(-inf,nullptr);
		}
		//stalemate
		return eval_move(0,nullptr);
	}

    if (turn) {
		int val = -inf;
		eval_move end = eval_move(val, nullptr);
		std::vector<move_t*> moves;
		generate_all_moves(&moves);
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
			int pval = search_aux(depth - 1, α, β).first;

			undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);

			if (pval > val){
				val = pval;
				end = eval_move(val, move);
			} 
			if (val > β) break;
			if (val > α) α = val;
			
		}
        return end;
	}
    else {
		int val = inf;
		eval_move end = eval_move(val, nullptr);
		std::vector<move_t*> moves;
		generate_all_moves(&moves);
        for (move_t* move : moves) {
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
			int pval = search_aux(depth - 1, α, β).first;

			undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);

			if (pval < val){
				val = pval;
				end = eval_move(val, move);
			} 
			if (val < α) break;
			if (val < β) β = val;
		}
        return end;
	} 
}

move_t* bitboard_t::search(int depth){
	return search_aux(depth).second;
}