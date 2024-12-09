#include "board.hpp"
#include <cstdlib>
#include <fstream>


std::map<std::string, int> letter_to_coords = { // Dictionary to translate input to move
	{"a" , 0},
	{"b" , 1},
	{"c" , 2},
	{"d" , 3},
	{"e" , 4},
	{"f" , 5},
	{"g" , 6},
	{"h" , 7}
};

std::map<int, std::string> coords_to_letter = { // Dictionary to translate input to move
	{0, "a"},
	{1, "b"},
	{2, "c"},
	{3, "d"},
	{4, "e"},
	{5, "f"},
	{6, "g"},
	{7, "h"}
};


board_t::board_t() {
	// DONE (probably, has not been tested. also, uses "new")
	/* Makes a starting chess board */
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			state[i][j] = nullptr;
			bool color=0;
			if (i == 0 || i == 1) {color = 1;}
			if (i == 0 || i == 7) {
				piece_t* piece;
				if (j == 0 || j == 7){
						piece = new rook_t(coords(i, j), color, this);
				}
				if (j == 1 || j == 6){
						piece = new knight_t(coords(i, j), color, this);
				}
				if (j == 2 || j == 5){
						piece = new bishop_t(coords(i, j), color, this);
				}
				if (j == 3){
						piece = new queen_t(coords(i, j), color, this);
				}
				if (j == 4){
						piece = new king_t(coords(i, j), color, this);
						king[color] = piece;
				}
				state[i][j] = piece;
			}
			if (i == 1 || i == 6) {
				state[i][j] = new pawn_t(coords(i, j), color, this);
			}
		}
	}
	//Initialise grids
	update_grids();
	turn = 1;
	fifty_moves = 0;
}

void board_t::update_grids() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid_attack[0][i][j] = 0;
			grid_attack[1][i][j] = 0;
		}
	}
	for (int i=0; i<SIZE; i++){
		for (int j=0; j<SIZE; j++){
			if (state[i][j] != nullptr){
				array_moves arr = state[i][j]->legal_moves();
				for (int k=0; k<arr.size(); k++){
					if(arr[k].type_move == 21 || arr[k].type_move == 22)
						continue;
					int ii = arr[k].after.i;
					int jj = arr[k].after.j;
					grid_attack[!state[i][j]->color][ii][jj]+=1;
				} 
			}
		}
	}
}

void board_t::print_grids(bool color){
	for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				std::cout << "\033[43m";
			else
				std::cout << "\033[100m";
			std::cout << " " << grid_attack[color][i][j] << " ";
		}
		std::cout << "\033[40m" << std::endl;
	}
	std::cout << "  ";
	for (int j = 0; j < SIZE; j++) {
		std::cout << " " + coords_to_letter[j] + " ";
	}
	std::cout << " " << std::endl;
}

//for debugging
void board_t::clear_board() {
	for(int i=0; i<SIZE; i++){ 
		for(int j=0; j<SIZE; j++){
			state[i][j] = nullptr;
		}
	}
}

void board_t::add_piece(int x, int y, piece_t* piece) {
	state[x][y] = piece;
}

void board_t::remove_piece(int x, int y) {
	state[x][y] = nullptr;
}

void board_t::print() {
	// TODO
	/* Prints the board sexily */
	for (int i = SIZE-1; i >= 0; i--) {
		std::cout << std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				std::cout << "\033[43m";
			else
				std::cout << "\033[100m";

			if (state[i][j] == nullptr){
				std::cout << "   ";
			}
			if (state[i][j] != nullptr){
				std::cout << " " << state[i][j]->display << " ";
			}
		}
		std::cout << "\033[40m" << std::endl;
	}
	std::cout << "  ";
	for (int j = 0; j < SIZE; j++) {
		std::cout << " " + coords_to_letter[j] + " ";
	}
	std::cout << " " << std::endl;
}

//to be able to put in a text file
std::string board_t::print_string() {
	// TODO
	/* Prints the board sexily */
	std::string s = "";
	for (int i = SIZE-1; i >= 0; i--) {
		s += std::to_string(i+1) + " ";
		for (int j = 0; j < SIZE; j++) {
			if((i + j) % 2 == 0)
				s += "|";
			else
				s += "|";

			if (state[i][j] == nullptr){
				s += "  ";
			}
			if (state[i][j] != nullptr){
				s += state[i][j]->display;
			}
		}
		s += "|\n";
	}
	s += "  ";
	for (int j = 0; j < SIZE; j++) {
		s += " " + coords_to_letter[j] + " ";
	}
	s += " \n";
	return s;
}


std::string board_t::move_to_string(move_t move) {
	std::string base = coords_to_letter[move.before.j] + std::to_string(move.before.i+1) + coords_to_letter[move.after.j] + std::to_string(move.after.i+1);
	//move.type_move == if it is promotion
	return base;
}



move_t board_t::string_to_move(std::string move) {
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
		
	move_t new_move {stoi(move.substr(1,1)) - 1, letter_to_coords[move.substr(0,1)], stoi(move.substr(3,1)) - 1, letter_to_coords[move.substr(2,1)],type};
	/*
	new_move.before.j = letter_to_coords[move.substr(0,1)];
	new_move.after.j = letter_to_coords[move.substr(2,1)];
	new_move.before.i = 8 - stoi(move.substr(1,1)); //stoi = string to integer
	new_move.after.i = 8 - stoi(move.substr(3,1));
	*/
	return new_move;
}

void board_t::update(std::string move, bool change_turn=true) {
	/* Updates the board with this move, assumes that the move is legal */
	//Define new position and old position
	update_with_move(string_to_move(move),change_turn);

	/*
	move_t new_move = string_to_move(move);

	if(new_move.type_move >= 30) {
		if(new_move.type_move == 30)
			state[new_move.after.i][new_move.after.j] = new queen_t(coords(new_move.after.i,new_move.after.j), turn, this);
		if(new_move.type_move == 31) {
			state[new_move.after.i][new_move.after.j] = new rook_t(coords(new_move.after.i,new_move.after.j), turn, this);
			state[new_move.after.i][new_move.after.j]->moved = true;
		}
		if(new_move.type_move == 32)
			state[new_move.after.i][new_move.after.j] = new bishop_t(coords(new_move.after.i,new_move.after.j), turn, this);
		if(new_move.type_move == 33)
			state[new_move.after.i][new_move.after.j] = new knight_t(coords(new_move.after.i,new_move.after.j), turn, this);

		state[new_move.before.i][new_move.before.j] = nullptr;
		return;
	}

	//Update the board
	state[new_move.after.i][new_move.after.j] = state[new_move.before.i][new_move.before.j];
	state[new_move.after.i][new_move.after.j]->location = new_move.after;
	state[new_move.before.i][new_move.before.j] = nullptr;
	if(change_turn)
		turn = !turn;
	last_move = new_move;
	*/

}

void board_t::update_with_move(move_t move, bool change_turn=true) {
	/* Updates the board with this move, assumes that the move is legal */
	//Update the board

	//if it is castling
	if(move.type_move == 1) {
		state[move.after.i][move.after.j] = state[move.before.i][move.before.j];
		state[move.after.i][move.after.j]->location = move.after;
		if(state[move.after.i][move.after.j]->id == "k" && state[move.after.i][move.after.j]->color == turn) {
			king[turn] = state[move.after.i][move.after.j];
		}

		state[move.before.i][move.before.j] = nullptr;
		if(move.after.j - move.before.j > 0) {
			state[move.after.i][move.after.j+1] = state[move.before.i][move.before.j+3];
			state[move.after.i+1][move.after.j+1]->location = makep(move.after.i+1,move.after.j+1);

			state[move.before.i][move.before.j+3] = nullptr;
		}
		else {
			state[move.after.i][move.after.j-1] = state[move.before.i][move.before.j-4];
			state[move.after.i-1][move.after.j-1]->location = makep(move.after.i-1,move.after.j-1);

			state[move.before.i][move.before.j-4] = nullptr;
		}
	}
	//if it is en passant
	else if(move.type_move == -2) {
		int direction = 1;
		if(turn == 0) direction = -1;
		state[move.after.i][move.after.j] = state[move.before.i][move.before.j];
		state[move.after.i][move.after.j]->location = move.after;

		state[move.before.i][move.before.j] = nullptr;
		state[move.after.i-direction][move.after.j] = nullptr;
	}

	//promotion to queen
	else if(move.type_move == 30) {
		state[move.before.i][move.before.j] = nullptr;
		state[move.after.i][move.after.j] = new queen_t(makep(move.after.i,move.after.j),turn,this);
	}
	else if(move.type_move == 31) {
		state[move.before.i][move.before.j] = nullptr;
		state[move.after.i][move.after.j] = new rook_t(makep(move.after.i,move.after.j),turn,this);
	}
	else if(move.type_move == 32) {
		state[move.before.i][move.before.j] = nullptr;
		state[move.after.i][move.after.j] = new bishop_t(makep(move.after.i,move.after.j),turn,this);
	}
	else if(move.type_move == 33) {
		state[move.before.i][move.before.j] = nullptr;
		state[move.after.i][move.after.j] = new knight_t(makep(move.after.i,move.after.j),turn,this);
	}
	else {
		state[move.after.i][move.after.j] = state[move.before.i][move.before.j];
		state[move.after.i][move.after.j]->location = move.after;
		if(state[move.after.i][move.after.j]->id == "k" && state[move.after.i][move.after.j]->color == turn) {
			king[turn] = state[move.after.i][move.after.j];
		}

		state[move.before.i][move.before.j] = nullptr;
	}
	
	if(change_turn)
		turn = !turn;

	//UPDATING GRIDS
	//Parsing every piece
	update_grids();
}

//This one not updated, might break!!
void board_t::undo(std::string move, piece_t* captured, bool change_turn=true) {
	undo_with_move(string_to_move(move), captured, change_turn);
}

void board_t::undo_with_move(move_t move, piece_t* captured, bool change_turn=true) {
	//undoing castling
	if(move.type_move == 1) {
		state[move.before.i][move.before.j] = state[move.before.i][move.before.j];
		state[move.before.i][move.before.j]->location = move.before;
		if(state[move.after.i][move.after.j]->id == "k" && state[move.before.i][move.before.j]->color == turn)
			king[turn] = state[move.before.i][move.before.j];

		state[move.after.i][move.after.j] = nullptr;
		if(move.after.j - move.before.j > 0) {
			state[move.before.i][move.before.j+3] = state[move.before.i][move.before.j+3];
			state[move.before.i][move.before.j+3]->location = makep(move.before.i,move.before.j+3);

			state[move.after.i][move.after.j+1] = nullptr;
		}
		else {
			state[move.before.i][move.before.j-4] = state[move.before.i][move.before.j-4];
			state[move.before.i][move.before.j-4]->location = makep(move.before.i,move.before.j-4);

			state[move.after.i][move.after.j-1] = nullptr;
		}
	}

	//undoing en passant
	else if(move.type_move == -2) {
		int direction = 1;
		if(turn == 0) direction = -1;

		state[move.before.i][move.before.j] = state[move.after.i][move.after.j];
		state[move.before.i][move.before.j]->location = move.before;

		state[move.after.i][move.after.j] = nullptr;
		state[move.after.i-direction][move.after.j] = captured;

		if(captured != nullptr)
			state[move.after.i][move.after.j]->location = makep(move.after.i-direction, move.after.j);
	}

	//undoing promotion
	else if(move.type_move >= 30 && move.type_move <= 33) {
		state[move.after.i][move.after.j] = nullptr;
		state[move.before.i][move.before.j] = new pawn_t(makep(move.before.i,move.before.j),turn,this);
	}
	else {
		state[move.before.i][move.before.j] = state[move.after.i][move.after.j];
		state[move.before.i][move.before.j]->location = move.before;
		if(state[move.before.i][move.before.j]->id == "k" && state[move.before.i][move.before.j]->color == turn) {
			king[turn] = state[move.before.i][move.before.j];
		}
		state[move.after.i][move.after.j] = captured; //undoing a capture if there was a capture
		if(captured != nullptr) {
			state[move.after.i][move.after.j]->location = move.after;
		}
	}
	if(change_turn)
		turn = !turn;
	update_grids();
}

//if(!(abs(current_move.before.i - x) == abs(current_move.before.j - y) || current_move.before.i == x || current_move.before.j == y))
		//return false;

//if(grid_attack[color][x][y] == 0 && !(abs(current_move.before.i - x) == abs(current_move.before.j - y) || current_move.before.i == x || current_move.before.j == y))
	//return false;

/*if(!(abs(current_move.before.i - x) == abs(current_move.before.j - y) || current_move.before.i == x || current_move.before.j == y)) {
	return false;
}*/

bool board_t::is_check(bool color, piece_t* captured, move_t current_move) {
	if(state[current_move.before.i][current_move.before.j]->id == "k") {
		if(grid_attack[color][current_move.after.i][current_move.after.j] == 0) {
			return false;
		}
		return true;
	}

	int x = king[color]->location.i;
	int y = king[color]->location.j;

	if(grid_attack[color][x][y] == 0) {
		int a = current_move.before.i;
		int b = current_move.before.j;
		int aa = current_move.after.i;
		int bb = current_move.after.j;
		if(!(abs(a - x) == abs(b - y) || a == x || b == y)) {
			return false;
		}
		if((a - x)*(bb - y) == (aa - x)*(b - y)) //check if it still on the same diagonal
			return false;
		if((a == x && a == aa) || (b == y && b == bb)) //check if it is still on the same row/column
			return false;
		
		if((a == x) && grid_attack[color][a][b] > 0) {
			int d = 1;
			if(x - a < 0) d = -1;
			int it = 1;
			while(a+d*it >= 0 && a+d*it < SIZE) {
				if(state[a+d*it][b] != nullptr) {
					if(state[a+d*it][b]->color == color)
						return false;
					if(state[a+d*it][b]->id == "r" || state[a+d*it][b]->id == "q") {
						return true;
					}
					else
						return false;
				}
				it+=1;
			}
			return false;
		}

		if((b == y) && grid_attack[color][a][b] > 0) {
			int d = 1;
			if(b - y < 0) d = -1;
			int it = 1;
			while(b+d*it >= 0 && b+d*it < SIZE) {
				if(state[a][b+d*it] != nullptr) {
					if(state[a][b+d*it]->color == color)
						return false;
					if(state[a][b+d*it]->id == "r" || state[a][b+d*it]->id == "q") {
						return true;
					}
					else
						return false;
				}
				it+=1;
			}
			return false;
		}

		/*if(abs(a - x) == abs(b - y) && grid_attack[color][a][b] > 0) {
			int dx = 1;
			int dy = 1;
			if(a - x < 0) dx = -1;
			if(b - y < 0) dy = -1;
			int it = 1;
			while(a+dx*it >=0 && a+dx*it < SIZE && b+dy*it >= 0 && b+dy*it < SIZE) {
				if(state[a+dx*it][b+dy*it] != nullptr) {
					if(state[a+dx*it][b+dy*it]->color == color)
						return false;
					if(state[a+dx*it][b+dy*it]->id == "b" || state[a+dx*it][b+dy*it]->id == "q") {
						return true;
					}
					else
						return false;
				}
				it+=1;
			}
			return false;
		}*/

	}

	update_with_move(current_move, false);

	if(grid_attack[color][x][y] == 0) {
		undo_with_move(current_move, captured, false);
		return false;
	}
	undo_with_move(current_move, captured, false);
	return true;
		
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if(state[i][j] != nullptr)
				if(state[i][j]->color == turn && state[i][j]->id == "k") {
					if(x != i && y != j) {
						std::cout << x << " " << y << "  " << i << " " << j << std::endl;
						print();
						error("king is flying");
					}
					if(grid_attack[color][i][j] == 0)
						return false;
					return true;
				}
		}
	}
	error("Why there is no king?");
}


bool board_t::check_valid_move_with_check(move_t current_move) {
	bool res = false;
	//check if captures
	piece_t* captured = state[current_move.after.i][current_move.after.j];
	
	//if it is en passant -> weird capture
	if(current_move.type_move == -2) {
		if(turn == 1)
			captured = state[current_move.after.i-1][current_move.after.j];
		else
			captured = state[current_move.after.i+1][current_move.after.j];
	}
	if(!is_check(turn, captured, current_move)) {
		res = true;
	}	
	
	return res;
}


long long board_t::nb_moves(int depth) {
	long long count = 0;
	if(depth == 1) {
		//print();
		return generate_all_moves().size();
	}
	for(auto move: generate_all_moves()) {
		piece_t* captured = state[move.after.i][move.after.j];
		update_with_move(move);
		count += nb_moves(depth-1);
		
		/*
		if(depth == 2) {
			std::ofstream o;
			o.open("moves.txt", std::ios_base::app);
			
			o << print_string();
			o.close();
		}
		*/
		print();
		undo_with_move(move,captured);
	}
	return count;
}

std::vector<move_t> board_t::generate_all_moves() {
	std::vector<move_t> all_moves;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if(state[i][j] != nullptr) {
				if(state[i][j]->color == turn) {
					/*
					//castling
					if(state[i][j]->id == "k" && !state[i][j]->moved) {
						if(state[i][j+3]->id == "r" && !state[i][j+3]->moved && state[i][j+1] == nullptr && state[i][j+2] == nullptr) {
							move_t current_move {makep(i,j), makep(i,j+2),1};
							if(check_valid_move_with_check(current_move))
								all_moves.push_back(current_move);
						}
					}
					if(state[i][j]->id == "k" && !state[i][j]->moved) {
						if(state[i][j-4]->id == "r" && !state[i][j-4]->moved && state[i][j-1] == nullptr && state[i][j-2] == nullptr && state[i][j-3] == nullptr) {
							move_t current_move {makep(i,j), makep(i,j-2),1};
							if(check_valid_move_with_check(current_move))
								all_moves.push_back(current_move);
						}
					}
					
					//en passant
					int direction = 1;
					if(turn == 0) direction = -1;
					if(state[i][j]->id == "p" && last_move.type_move != -10) {
						if(state[last_move.after.i][last_move.after.j]->id == "p" && abs(last_move.after.j - last_move.before.j) == 2 && last_move.after.i == i && abs(last_move.after.j - j) == 1) {
							move_t current_move {makep(i,j), makep(i+direction,last_move.after.j),-2};
							if(check_valid_move_with_check(current_move))
								all_moves.push_back(current_move);
						}
					}

					//promotion
					int last_row = 6;
					if(turn == 0) last_row = 1;
					if(state[i][j]->id == "p" && i == last_row) {
						if(state[i+direction][j] == nullptr) {
							move_t current_move {makep(i,j), makep(i+direction,j), 3};
							if(check_valid_move_with_check(current_move)) {
								current_move.type_move = 30;
								all_moves.push_back(current_move);
								current_move.type_move = 31;
								all_moves.push_back(current_move);
								current_move.type_move = 32;
								all_moves.push_back(current_move);
								current_move.type_move = 33;
								all_moves.push_back(current_move);
							}
						}
					}
					*/
					
					array_moves legal_moves_current = state[i][j]->legal_moves();
					for(auto move : legal_moves_current) {
						if(move.type_move == -100)
							continue;
						if(check_valid_move_with_check(move))
							all_moves.push_back(move);
					}

					/*
					for(auto next_location: legal_moves_current) {
						move_t current_move {current_location, next_location};
						//check if it is a capture
						if(state[current_move.after.i][current_move.after.j] != nullptr)
							if(state[current_move.after.i][current_move.after.j]->color != turn) 
								current_move.type_move = -1;

						if(check_valid_move_with_check(current_move))
							all_moves.push_back(current_move);
					}
					*/
				}
			}
		}
	}

	return all_moves;
}



int board_t::eval() {
	// DONE (for now, we will change how boards are evaluated later)
	/* Evaluates the current position. Positive score means white is winning, negative score means black */
	int ret;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int indicator = -1;
			if (state[i][j]->color) indicator = 1;
			ret += state[i][j]->points * indicator;
		}
	}
	return ret;
}

std::string board_t::next_move() {
	//TODO
	/* Calculates the next move */
	//Returning random legal move
	return "a2a4";
	/*std::vector<piece_t*> our_pieces;
	for(int i=0; i < SIZE; i++) {
		for(int j=0; j < SIZE; j++) {
			if(state[i][j]->color == turn)
				our_pieces.push_back(state[i][j]);
		}
	}
	int n = our_pieces.size();
	while(true) {
		array_coords random_pick = our_pieces[rand() % n]->legal_moves();
		if(!random_pick.empty()) {
			return random_pick[rand() % random_pick.size()].print();
		}
	}*/
}