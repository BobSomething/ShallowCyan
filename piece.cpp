#include "piece.hpp"


void error(std::string str) {
    std::cout << "Error: " << str << std::endl;
    exit(1);
}


array_moves piece_t::legal_moves() {
    array_moves arr;
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

    if(board->state[x][y]->color == !color) //the cell has a piece with the opposite color
        return 2;

    if(board->state[x][y]->color == color) //the cell has a piece with the same color
        return 3; // this is to implement attacking cells better => we will have a way to determine if a piece is protected or not!
    
    error("Somehow the board is not implement correctly");
}

//implementation of legal moves
//TODO NEED TO IMPLEMENT EN PASSANT
array_moves pawn_t::legal_moves(){
    array_moves arr;
    int d = 1; //direction
    int first_time = 1; //the pawn did not move yet
    int last_row = 7;
    int enpassant_row = 4;
    if(color == 0) {
        d = -1;
        first_time = 6;
        last_row = 0;
        enpassant_row = 3;
    }

    int x = location.i;
    int y = location.j;
    //Note: x == 7 or x == 0 is not possible

    //promotion
    if(x+d == last_row) {
        arr.push_back(move_t(location,makep(x+d,y),30)); //queen promotion
        arr.push_back(move_t(location,makep(x+d,y),31)); //rook promotion
        arr.push_back(move_t(location,makep(x+d,y),32)); //bishop promotion
        arr.push_back(move_t(location,makep(x+d,y),33)); //knight promotion
    }

    //en passant
    if(x == last_row-d) {
        if(check_cell(x,y+1, color, board) == 2)
            if(board->state[x][y+1]->id == "p" && board->state[x][y+1]->color != color && board->last_move.type_move == 22 && board->last_move.after.j == y+1)
                arr.push_back(move_t(location,makep(x+d,y+1), -2)); //en passant
        if(check_cell(x,y-1, color, board) == 2)
            if(board->state[x][y-1]->id == "p" && board->state[x][y-1]->color != color && board->last_move.type_move == 22 && board->last_move.after.j == y-1)
                arr.push_back(move_t(location,makep(x+d,y-1), -2)); //en passant
    }

    if(check_cell(x+d,y, color, board) == 1){ //if next square is empty
        arr.push_back(move_t(location, makep(x+d,y),21)); //normal move

        if (x == first_time) //first move: you can move two if square is empty
            if(check_cell(x+2*d,y, color, board) == 1)
                arr.push_back(move_t(location, makep(x+2*d,y),22)); //normal move
    }
    
    //checking if the diagonal captures are legal
    
    if(check_cell(x+d,y+1, color, board) == 2) 
        arr.push_back(move_t(location, makep(x+d,y+1),-1)); //capture
    else if(check_cell(x+d,y+1, color, board) != 0)
        arr.push_back(move_t(location, makep(x+d,y+1),-100)); //attack on the square

    if(check_cell(x+d,y-1, color, board) == 2) 
        arr.push_back(move_t(location, makep(x+d,y-1),-1)); //capture
    else if(check_cell(x+d,y-1, color, board) != 0)
        arr.push_back(move_t(location, makep(x+d,y-1),-100)); //attack on the square
        
    return arr;
}

//template of legal moves for rook, bishop and queen
array_moves template_legal_moves(coords location, bool color, board_t* board, std::map<coords, bool> directions) {
    array_moves arr;
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
                //if it is out of bounds
                if(check_cell(x+it*key.i,y+it*key.j, color, board) == 0) {
                    directions[key] = false;
                }
                
                //lands on a cell with the same color piece
                else if(check_cell(x+it*key.i,y+it*key.j, color, board) == 3) {
                    arr.push_back(move_t(location, makep(x+it*key.i,y+it*key.j), -100)); //on the same color
                    directions[key] = false;
                }

                //if it lands on a cell with a opposite color piece
                else if(check_cell(x+it*key.i,y+it*key.j, color, board) == 2) {
                    arr.push_back(move_t(location, makep(x+it*key.i,y+it*key.j), -1)); //capture
                    directions[key] = false;
                }
                //if it lands on a empty cell
                else
                    arr.push_back(move_t(location, makep(x+it*key.i,y+it*key.j), 0)); //normal move
            }
        }
        if(is_going == false)
            break;
    }
    return arr;
}

array_moves bishop_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;

    return template_legal_moves(location, color, board, directions);
}


array_moves knight_t::legal_moves() {
    array_moves arr;
    int x = location.i;
    int y = location.j;
    coords directions[] = {makep(2,1), makep(2,-1), makep(-2,1), makep(-2,-1), makep(1,2), makep(1,-2), makep(-1,2), makep(-1,-2)};
    for(auto& [i, j]: directions) {
        if(check_cell(x+i,y+j, color, board) == 1) 
            arr.push_back(move_t(location, makep(x+i,y+j), 0)); //normal move
        else if(check_cell(x+i,y+j, color, board) == 2)
            arr.push_back(move_t(location, makep(x+i,y+j), -1)); //capture
        else if(check_cell(x+i,y+j, color, board) == 3)
            arr.push_back(move_t(location, makep(x+i,y+j), -100)); //on the same color

    }
    return arr;
}


array_moves rook_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,1)] = true;
    directions[makep(0,-1)] = true;

    return template_legal_moves(location, color, board, directions);
}

//outdated, we use the grid_attack now instead
/*
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
    
    array_moves bishop_check = template_legal_moves(location, color, board, directions);

    for(auto &[x,y]: bishop_check)
        if(board->state[x][y] != nullptr) 
            if(board->state[x][y]->id == "b" || board->state[x][y]->id == "q") 
                return true;

    directions.clear();
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    array_moves rook_check = template_legal_moves(location, color, board, directions);

    for(auto &[x,y]: rook_check)
        if(board->state[x][y] != nullptr) 
            if(board->state[x][y]->id == "b" || board->state[x][y]->id == "q") 
                return true;

    array_moves knight_check;
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
*/

array_moves king_t::legal_moves() {
    //Can simplify this
    //NEED TO FIX THIS, CHECK IF THE KING IS NOT GOING ON A CHECK
    array_moves arr;
    int x = location.i;
    int y = location.j;
    int it = 0;

    coords directions[] = {makep(1,1), makep(-1,1), makep(1,-1), makep(-1,-1), 
                            makep(1,0), makep(-1,0), makep(0,-1), makep(0,1)};

    for(auto d: directions) {
        //if it lands on a cell with a opposite color piece or if it lands on a empty cell
        if(check_cell(x+d.i,y+d.j, color, board) == 1) {
            if(board->grid_attack[color][x+d.i][y+d.j] == 0)
                arr.push_back(move_t(location,makep(x+d.i,y+d.j),0));
        }
        else if(check_cell(x+d.i,y+d.j, color, board) == 2) {
            if(board->grid_attack[color][x+d.i][y+d.j] == 0)
                arr.push_back(move_t(location,makep(x+d.i,y+d.j),-1));
        }

        else if(check_cell(x+d.i,y+d.j, color, board) == 3) {
            arr.push_back(move_t(location,makep(x+d.i,y+d.j),-100)); //on the same color 
        }    
    }

    //castling
    if(!moved && y == 4) {
        //short castle
        if(board->state[x][y+1] == nullptr && board->state[x][y+2] == nullptr && board->state[x][y+3] != nullptr) {
            if(board->state[x][y+3]->id == "r" && !board->state[x][y+3]->moved) {
                if(board->grid_attack[color][x][y+1] == 0 && board->grid_attack[color][x][y+2] == 0)
                    arr.push_back(move_t(location, makep(x,y+2), 1)); //castling
            }
        }

        //long castle
        if(board->state[x][y-1] == nullptr && board->state[x][y-2] == nullptr && board->state[x][y-3] == nullptr && board->state[x][y-4] != nullptr) {
            if(board->state[x][y-4]->id == "r" && !board->state[x][y-4]->moved) {
                if(board->grid_attack[color][x][y-1] == 0 && board->grid_attack[color][x][y-2] == 0)
                    arr.push_back(move_t(location, makep(x,y-2), 1)); //castling
            }
        }
    }

    return arr;
}


array_moves queen_t::legal_moves() {
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;

    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,1)] = true;

    return template_legal_moves(location, color, board, directions);
}