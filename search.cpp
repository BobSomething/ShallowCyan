#include "bitboard.hpp"

eval_move bitboard_t::search_aux(int depth, int α, int β) {
	/* Alpha-beta pruning :) */
	// for now, returns an int
	// MAKE A COPY FUNCTION (should be done but im keeping this in case its wrong so we know its important)
    if (depth == 0) { // OR CHECKMATE
		return eval_move(this->eval(),nullptr);
	}
    if (turn) {
		int val = -inf;
		eval_move end = eval_move(val, nullptr);
		std::vector<move_t*> moves;
		generate_all_moves(&moves);
        for (move_t* move: moves) {
			bitboard_t child = this->copy();
			child.update(move);
			int pval = child.search_aux(depth - 1, α, β).first;
			if (pval > val){
				val = pval;
				eval_move end = eval_move(val, move);
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
			bitboard_t child = this->copy();
			child.update(move);
			int pval = child.search_aux(depth - 1, α, β).first;
			if (pval < val){
				val = pval;
				eval_move end = eval_move(val, move);
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