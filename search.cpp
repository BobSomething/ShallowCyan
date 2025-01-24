#include "bitboard.hpp"

move_t* bitboard_t::search(int depth, int α, int β, std::time_t time) {
	/* Alpha-beta pruning :) */
	move_t* ret = new move_t;
    if (depth == 0) {
		ret->eval = eval();
		return ret;
		//return Quiescence_search(2, α,β, time);
	}

    int val = (turn) ? -inf : inf;
	std::vector<move_t*> moves;
	generate_all_moves(&moves);
	
	if(moves.size() == 0) {
        int king = (turn == 0) ? 11 : 5;
		int sign = (turn == 0) ? 1 : -1;
		if(is_square_attacked(get_LSB(piecesBB[king]),!turn)) ret->eval = sign*(inf + depth); //checkmate
		else ret->eval = 0; //stalemate
		return ret;
	}
	
	ret = moves[0];
	ret->eval = val;

	int move_counter = 0;
	for (move_t* move: moves) {
		auto trackered = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    	if (trackered - time > MAX_TIME){
			time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			break;
		}
		int p_before = pieceTable[move->before.i*8 + move->before.j];
		int p_after = pieceTable[move->after.i*8 + move->after.j]; 
		int ep_square = enpassant_square;
		bool w_c_kside = w_castle_kside;
		bool w_c_qside = w_castle_qside; 
		bool b_c_kside = b_castle_kside;
		bool b_c_qside = b_castle_qside;

		update(move);
		current_depth++;

		int pval;
		if (counter_hash_map[hash_current_board] >= 3){
			pval = 0;
		}

		else if (move_counter >= 7){
			if (depth == 5) {
				pval = search(depth-2, α, β, time)->eval;

				if (pval > val) {
					pval = search(depth - 1, α, β, time)->eval;
				}

			} else {
				pval = search(depth - 1, α, β, time)->eval;
			}
		} else {
			pval = search(depth - 1, α, β, time)->eval;
		}

		current_depth--;
		undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
		
		//if(depth == 5) {
			//std::cout << move_to_string(move) << " " << score_move(move) << ": " << pval << " " << val << std::endl;
		//}

		if (turn) {
			if (pval > val){
				val = pval;
				move->eval = val;
				ret = move;
			} 
			if (val > β) {
				killer_moves[current_depth][1] = killer_moves[current_depth][0];
				killer_moves[current_depth][0] = move;
				break;
			}
			if (val > α) {
				history_moves[pieceTable[move->before.i*8+move->before.j]][move->before.i*8+move->before.j] += 5*depth;
				α = val;
			}
		} else {
			if (pval < val){
				val = pval;
				move->eval = val;
				ret = move;
			} 
			if (val < α) {
				killer_moves[current_depth][1] = killer_moves[current_depth][0];
				killer_moves[current_depth][0] = move;
				break;
			}
			if (val < β) {
				history_moves[pieceTable[move->before.i*8+move->before.j]][move->before.i*8+move->before.j] += 5*depth;
				β = val;
			}
		}
		move_counter++;
	}
	return ret;
}

/*
move_t* bitboard_t::Quiescence_search(int depth, int α, int β, std::time_t time){
	// Timer
	move_t* best_move = new move_t;

	best_move->eval = eval();

	if (depth == 0){
		return best_move;
	}
	int pval = best_move->eval;

	std::vector<move_t*> moves;
	generate_all_moves(&moves, true);

	if(moves.size() == 0) {
        int king = (turn == 0) ? 11 : 5;
		int sign = (turn == 0) ? 1 : -1;
		if(is_square_attacked(get_LSB(piecesBB[king]),!turn)) best_move->eval = sign*(inf + depth); //checkmate
		// TESTING
		else best_move->eval = 0; //stalemate
		return best_move;
	}

	int val = (turn) ? -inf : inf;
	
	best_move = moves[0];
	best_move->eval = val;
	
	for (move_t* move: moves){
		auto trackered = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    	if (trackered - time > MAX_TIME){
			time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			break;
		}

		int p_before = pieceTable[move->before.i*8 + move->before.j];
		int p_after = pieceTable[move->after.i*8 + move->after.j]; 
		int ep_square = enpassant_square;
		bool w_c_kside = w_castle_kside;
		bool w_c_qside = w_castle_qside; 
		bool b_c_kside = b_castle_kside;
		bool b_c_qside = b_castle_qside;

		current_depth++;
		update(move);
		int score_Q = Quiescence_search(depth-1, α, β, time)->eval;

		current_depth--;
		undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);


		if (turn) {
			if (score_Q > val){
				val = score_Q;
				move->eval = val;
				best_move = move;
			} 
			if (val > β) {
				killer_moves[current_depth][1] = killer_moves[current_depth][0];
				killer_moves[current_depth][0] = move;
				break;
			}
			if (val > α) {
				history_moves[pieceTable[move->before.i*8+move->before.j]][move->before.i*8+move->before.j] += 5*depth;
				α = val;
			}
		} else {
			if (score_Q < val){
				val = score_Q;
				move->eval = val;
				best_move = move;
			} 
			if (val < α) {
				killer_moves[current_depth][1] = killer_moves[current_depth][0];
				killer_moves[current_depth][0] = move;
				break;
			}
			if (val < β) {
				history_moves[pieceTable[move->before.i*8+move->before.j]][move->before.i*8+move->before.j] += 5*depth;
				β = val;
			}
		}
	}
	return best_move;
}
*/