#include "bitboard.hpp"
#include <chrono>

//std::chrono::time_point<std::chrono::high_resolution_clock> start;

move_t* bitboard_t::search(int depth, int α, int β) {
	// Timer for 5 seconds rule
	auto start = std::chrono::high_resolution_clock::now();

	/* Alpha-beta pruning :) */
	move_t* ret = new move_t;
    if (depth == 0) {
		ret->eval = eval();
		return ret;
		//return Quiescence_search(2, α,β);
	}

    int val = (turn) ? -inf : inf;
	std::vector<move_t*> moves;
	generate_all_moves(&moves);
	
	if(moves.size() == 0) {
        int king = (turn == 0) ? 11 : 5;
		int sign = (turn == 0) ? 1 : -1;
		if(is_square_attacked(get_LSB(piecesBB[king]),!turn)) ret->eval = sign*inf; //checkmate
		else ret->eval = 0; //stalemate
		return ret;
	}
	
	ret = moves[0];
	ret->eval = val;

	for (move_t* move: moves) {
		auto now = std::chrono::high_resolution_clock::now();
    	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		if (elapsed>=7){
			break;
		}

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
		int pval;
		if (counter_hash_map[hash_current_board] >= 3){
			pval = 0;
		}
		/*else {
			std::map<U64,int>::iterator it;
			it = zobrist_hash_map.find(hash_current_board);
			if (it != zobrist_hash_map.end()){
				pval = zobrist_hash_map[hash_current_board];
			}
			//int pval = child.search_aux(depth - 1, α, β).first;
			else {
				pval = search(depth - 1, α, β)->eval;
				zobrist_hash_map[hash_current_board] = pval;
			}
		}*/
		else {
			pval = search(depth - 1, α, β)->eval;
			if (pval) pval -= ((turn == 0) ? 1 : -1);
		}
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

move_t* bitboard_t::Quiescence_search(int depth, int α, int β){
	move_t* best_move = new move_t;

	best_move->eval = eval();

    if (best_move->eval >= β) {
        return best_move;
    }
    if (best_move->eval > α) {
        α = best_move->eval;
    }

	if (depth == 0){
		return best_move;
	}
	int pval = best_move->eval;

	std::vector<move_t*> moves;
	generate_all_moves(&moves, true);

	int val = (turn) ? -inf : inf;

	for (move_t* move: moves){
		int p_before = pieceTable[move->before.i*8 + move->before.j];
		int p_after = pieceTable[move->after.i*8 + move->after.j]; 
		int ep_square = enpassant_square;
		bool w_c_kside = w_castle_kside;
		bool w_c_qside = w_castle_qside; 
		bool b_c_kside = b_castle_kside;
		bool b_c_qside = b_castle_qside;

		update(move);
		int score_Q = Quiescence_search(depth-1, α, β)->eval;
		undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);


		if (turn) {
			if (score_Q > val){
				val = score_Q;
				move->eval = val;
				best_move = move;
			} 
			if (val > β) break;
			if (val > α) α = val;
		} else {
			if (score_Q < val){
				val = score_Q;
				move->eval = val;
				best_move = move;
			} 
			if (val < α) break;
			if (val < β) β = val;
		}
	}
	return best_move;
}