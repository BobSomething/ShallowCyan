#include "piece.hpp"

//implementation of legal moves
array_coords pawn_t::legal_moves(board_t* board){
    array_coords arr;
    if (this.color == 1){ //if pawn is white
        int y = this.location.i
        int x = this.location.j
        if (x < 7){ //not go outside of the board
            if (x == 1){ //first move: you can move two if square is empty
                if (board->state[y+2][x] == nullptr){ 
                    arr.push_back(pair<y+2,x>)
                }
            }
            if (board->state[y+1][x] == nullptr){ //if next square is empty
                arr.push_back(pair<y+1,x>)
            }
            else if (board->state[y+1][x+1] != nullptr){ //if diagonal square is taken
                if (board->state[y+1][x+1]->color == 0){ //if diagonal piece is opposite color
                    arr.push_back(pair<y+1,x+1>)
                }
            }
            else if (board->state[y+1][x-1] != nullptr){
                if (board->state[y+1][x+1]->color == 0){
                    arr.push_back(pair<y+1,x-1>)
                }
            }
        }
    }else{ //if pawn is black
        int y = this.location.i
        int x = this.location.j
        if (x > 0){ //not go outside of the board
            if (x == 6){ //first move: you can move two if square is empty
                if (board->state[y-2][x] == nullptr){ 
                    arr.push_back(pair<y-2,x>)
                }
            }
            if (board->state[y-1][x] == nullptr){ //if next square is empty
                arr.push_back(pair<y-1,x>)
            }
            else if (board->state[y-1][x+1] != nullptr){ //if diagonal square is taken
                if (board->state[y-1][x+1]->color == 1){ //if diagonal piece is opposite color
                  arr.push_back(pair<y-1,x+1>)
                }
            }
            else if (board->state[y-1][x-1] != nullptr){
                if (board->state[y-1][x+1]->color == 1){
                    arr.push_back(pair<y-1,x-1>)
                }
            }
        }
    }
    return arr

}

