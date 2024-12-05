#include "board.hpp"

move_t board_t::search(int depth, int α = -inf, int β = inf) {
	/* Alpha-beta pruning :) */
	// for now, returns an int
	// MAKE A COPY FUNCTION
    if (depth == 0) { // OR CHECKMATE
		return eval();
	}
    if (turn) {
		int val = -inf;
        for (move_t move : generate_all_moves()) {
			board_t child = copy();
			child.update(move);
			int pval = child.search(depth - 1, α, β);
			if (pval > val) val = pval;
			if (val > β) break;
			if (val > α) α = val;
		}
        return val;
	}
    else {
		int val = inf;
        for (move_t move : generate_all_moves()) {
			board_t child = copy();
			child.update(move);
			int pval = child.search(depth - 1, α, β);
			if (pval < val) val = pval;
			if (val < α) break;
			if (val < β) β = val;
		}
        return val;
	}
}