#include "piece.hpp"


void error(std::string str) {
    std::cout << "Error: " << str << std::endl;
    exit(1);
}


array_coords piece_t::legal_moves() {
    array_coords arr;
    return arr;
}

bool piece_t::is_checked() {
    error("Something is wrong, it should be a king!");
}

/* 
returns 0 if the cell is out of bounds or if the cell in bounds and contains a piece with the same color 
returns 1 if the cell is in bounds and it is empty
returns 2 if the cell is in bounds and contains a piece with opposite color
*/
int check_cell(int x, int y, bool color, board_t* board) {
    if(x >= SIZE || x < 0 || y >= SIZE || y < 0) //the cell is out of bounds
        return 0;

    if(board->state[x][y] == nullptr) //the cell is empty
        return 1;

    if(board->state[x][y]->color == color) //the cell has a piece with the same color
        return 0;

    if(board->state[x][y]->color == !color) //the cell has a piece with the opposite color
        return 2;
    
    error("Somehow the board is not implement correctly");
}

//implementation of legal moves
//TODO NEED TO IMPLEMENT EN PASSANT
array_coords pawn_t::legal_moves(){
    array_coords arr;
    int d = 1; //direction
    int first_time = 1; //the pawn did not move yet
    int last_row = 7;
    if(color == 0) {
        d = -1;
        first_time = 6;
        last_row = 0;
    }

    int x = location.i;
    int y = location.j;
    //Note: x == 7 or x == 0 is not possible

    if(x+d == last_row) {
        //PROMOTION!
    }

    if(check_cell(x+d,y, color, board) == 1){ //if next square is empty
        arr.push_back(makep(x+d,y));

        if (x == first_time) //first move: you can move two if square is empty
            if(check_cell(x+2*d,y, color, board) == 1)
                arr.push_back(makep(x+2*d,y));
    }
    
    //checking if the diagonal captures are legal
    if(check_cell(x+d,y+1, color, board) == 2) 
        arr.push_back(makep(x+d,y+1));

    if(check_cell(x+d,y-1, color, board) == 2) 
        arr.push_back(makep(x+d,y-1));

    return arr;
}

//template of legal moves for rook, bishop and queen
array_coords template_legal_moves(coords location, bool color, board_t* board, std::map<coords, bool> directions) {
    array_coords arr;
    int x = location.i;
    int y = location.j;
    int it = 0;

    while(true) {
        bool is_going = false;
        it++;
        //we iterate for every possible directions
        for(auto &[key, value]: directions) {
            if(value == true) {
                is_going = true;
                //if it is out of bounds or lands on a cell with the same color piece
                if(check_cell(x+it*key.i,y+it*key.j, color, board) == 0) {
                    directions[key] = false;
                }

                //if it lands on a cell with a opposite color piece
                else if(check_cell(x+it*key.i,y+it*key.j, color, board) == 2) {
                    arr.push_back(makep(x+it*key.i,y+it*key.j));
                    directions[key] = false;
                }
                //if it lands on a empty cell
                else
                    arr.push_back(makep(x+it*key.i,y+it*key.j));
            }
        }
        if(is_going == false)
            break;
    }
    return arr;
}

array_coords bishop_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;

    return template_legal_moves(location, color, board, directions);
}


array_coords knight_t::legal_moves() {
    array_coords arr;
    int x = location.i;
    int y = location.j;
    coords directions[] = {makep(2,1), makep(2,-1), makep(-2,1), makep(-2,-1), makep(1,2), makep(1,-2), makep(-1,2), makep(-1,-2)};
    for(auto& [i, j]: directions) {
        if(check_cell(x+i,y+j, color, board) != 0) {
            arr.push_back(makep(x+i,y+j));
        }
    }
    return arr;
}


array_coords rook_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    return template_legal_moves(location, color, board, directions);
}

bool king_t::is_checked() {
    //pawn :(
    int d = 1;
    if(color == 1)
        d = -1;

    if(check_cell(location.i+d, location.j-1, color, board) == 2)
        if(board->state[location.i+d][location.j-1]->id == "p")
            return true;

    if(check_cell(location.i+d, location.j+1, color, board) == 2)
        if(board->state[location.i+d][location.j+1]->id == "p")
            return true;

    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;
    
    array_coords bishop_check = template_legal_moves(location, color, board, directions);

    for(auto &[x,y]: bishop_check)
        if(board->state[x][y] != nullptr) 
            if(board->state[x][y]->id == "b" || board->state[x][y]->id == "q") 
                return true;

    directions.clear();
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    array_coords rook_check = template_legal_moves(location, color, board, directions);

    for(auto &[x,y]: rook_check)
        if(board->state[x][y] != nullptr) 
            if(board->state[x][y]->id == "b" || board->state[x][y]->id == "q") 
                return true;

    array_coords knight_check;
    int x = location.i;
    int y = location.j;
    coords directions_knight[] = {makep(2,1), makep(2,-1), makep(-2,1), makep(-2,-1), makep(1,2), makep(1,-2), makep(-1,2), makep(-1,-2)};
    for(auto& [i, j]: directions_knight) {
        if(check_cell(x+i,y+j, color, board) != 0) {
            knight_check.push_back(makep(x+i,y+j));
        }
    }

    for(auto &[x,y]: knight_check) 
        if(board->state[x][y] != nullptr)
            if(board->state[x][y]->id == "n") 
                return true;

    return false;
}  

array_coords king_t::legal_moves() {
    //Can simplify this
    //NEED TO FIX THIS, CHECK IF THE KING IS NOT GOING ON A CHECK
    array_coords arr;
    int x = location.i;
    int y = location.j;
    int it = 0;
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;
    
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    for(auto &[key, value]: directions) {
        //if it lands on a cell with a opposite color piece or if it lands on a empty cell
        if(check_cell(x+it*key.i,y+it*key.j, color, board) == 2 || check_cell(x+it*key.i,y+it*key.j, color, board) == 1) {
            board->add_piece(x+it*key.i,y+it*key.j, new king_t(makep(x+it*key.i,y+it*key.j),color,board));
            board->remove_piece(x,y);
            if(is_checked())
                arr.push_back(makep(x+it*key.i,y+it*key.j));
            board->remove_piece(x+it*key.i,y+it*key.j);
            board->add_piece(x,y, new king_t(makep(x,y),color,board));
        }
            
    }
    return arr;
}


array_coords queen_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;

    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    return template_legal_moves(location, color, board, directions);
}