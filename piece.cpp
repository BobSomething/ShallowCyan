#include "piece.hpp"

array_coords piece_t::legal_moves(board_t* board) {
    array_coords arr;
    return arr;
}

bool piece_t::check_cell(int x, int y, board_t* board) {
    if(x >= 7 || x < 0 || y >= 7 || y < 0)
        return false;

    if(board->state[x][y]->color == color)
        return false;

    return true; 
}

//implementation of legal moves
//TODO NEED TO IMPLEMENT EN PASSANT
array_coords pawn_t::legal_moves(board_t* board){
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

    if (board->state[x+d][y] == nullptr){ //if next square is empty
        arr.push_back(std::make_pair(x+d,y));

        if (x == first_time) //first move: you can move two if square is empty
            if(board->state[x+2*d][y] == nullptr)
                arr.push_back(std::make_pair(x+2*d,y));
    }
    
    //checking if the diagonal captures are legal
    if(check_cell(x+d,y+1,board)) 
        if(board->state[x+d][y+1]->color == !color)
            arr.push_back(std::make_pair(x+d,y+1));

    if(check_cell(x+d,y-1,board)) 
        if(board->state[x+d][y-1]->color == !color)
            arr.push_back(std::make_pair(x+d,y-1));

    /*
    if (x < 7){ //not go outside of the board
            if (x == 1){ //first move: you can move two if square is empty
                if (board->state[y+2][x] == nullptr){ 
                    arr.push_back(std::make_pair(y+2,x));
                }
            }
            if (board->state[y+1][x] == nullptr){ //if next square is empty
                arr.push_back(std::make_pair(y+1,x));
            }
            else if (board->state[y+1][x+1] != nullptr){ //if diagonal square is taken
                if (board->state[y+1][x+1]->color == 0){ //if diagonal piece is opposite color
                    arr.push_back(std::make_pair(y+1,x+1));
                }
            }
            else if (board->state[y+1][x-1] != nullptr){
                if (board->state[y+1][x+1]->color == 0){
                    arr.push_back(std::make_pair(y+1,x-1));
                }
            }
        }
    }else{ //if pawn is black
        int y = location.i;
        int x = location.j;
        if (x > 0){ //not go outside of the board
            if (x == 6){ //first move: you can move two if square is empty
                if (board->state[y-2][x] == nullptr){ 
                    arr.push_back(std::make_pair(y-2,x));
                }
            }
            if (board->state[y-1][x] == nullptr){ //if next square is empty
                arr.push_back(std::make_pair(y-1,x));
            }
            else if (board->state[y-1][x+1] != nullptr){ //if diagonal square is taken
                if (board->state[y-1][x+1]->color == 1){ //if diagonal piece is opposite color
                  arr.push_back(std::make_pair(y-1,x+1));
                }
            }
            else if (board->state[y-1][x-1] != nullptr){
                if (board->state[y-1][x+1]->color == 1){
                    arr.push_back(std::make_pair(y-1,x-1));
                }
            }
        }
    }
    */
    return arr;
}

array_coords bishop_t::legal_moves(board_t* board) {
    //TODO
    array_coords arr;
    return arr;
}


array_coords knight_t::legal_moves(board_t* board) {
    //TODO
    array_coords arr;
    return arr;
}


array_coords rook_t::legal_moves(board_t* board) {
    //TODO
    array_coords arr;
    return arr;
}


array_coords king_t::legal_moves(board_t* board) {
    //TODO
    array_coords arr;
    return arr;
}


array_coords queen_t::legal_moves(board_t* board) {
    //TODO
    array_coords arr;
    return arr;
}