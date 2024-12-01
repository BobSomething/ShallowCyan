#include "piece.hpp"


void error(std::string str) {
    std::cout << "Error: " << str << std::endl;
    exit(1);
}


array_coords piece_t::legal_moves() {
    array_coords arr;
    return arr;
}

int piece_t::check_cell(int x, int y) {
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

    if(check_cell(x+d,y) == 0){ //if next square is empty
        arr.push_back(makep(x+d,y));

        if (x == first_time) //first move: you can move two if square is empty
            if(check_cell(x+2*d,y) == 0)
                arr.push_back(makep(x+2*d,y));
    }
    
    //checking if the diagonal captures are legal
    if(check_cell(x+d,y+1) == 2) 
        arr.push_back(makep(x+d,y+1));

    if(check_cell(x+d,y-1) == 2) 
        arr.push_back(makep(x+d,y-1));

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

array_coords bishop_t::legal_moves() {
    array_coords arr;
    int x = location.i;
    int y = location.j;
    int it = 0;
    std::map<coords, bool> directions;
    directions[makep(1,1)] = true;
    directions[makep(-1,1)] = true;
    directions[makep(1,-1)] = true;
    directions[makep(-1,-1)] = true;

    while(true) {
        bool is_going = false;
        it++;
        //we iterate for every possible directions
        for(auto &[key, value]: directions) {
            if(value == true) {
                is_going = true;
                //if it is out of bounds or lands on a cell with the same color piece
                if(check_cell(x+it*key.i,y+it*key.j) == 0) {
                    directions[key] = false;
                }

                //if it lands on a cell with a opposite color piece
                else if(check_cell(x+it*key.i,y+it*key.j) == 2) {
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


array_coords knight_t::legal_moves() {
    array_coords arr;
    int x = location.i;
    int y = location.j;
    coords directions[] = {makep(2,1), makep(2,-1), makep(-2,1), makep(-2,-1), makep(1,2), makep(1,-2), makep(-1,2), makep(-1,-2)};
    for(auto& [i, j]: directions) {
        if(check_cell(x+i,y+j) != 0) {
            arr.push_back(makep(x+i,y+j));
        }
    }
    return arr;
}


array_coords rook_t::legal_moves() {
    array_coords arr;
    int x = location.i;
    int y = location.j;
    int it = 0;
    std::map<coords, bool> directions;
    directions[makep(1,0)] = true;
    directions[makep(-1,0)] = true;
    directions[makep(0,-1)] = true;
    directions[makep(0,-1)] = true;

    while(true) {
        bool is_going = false;
        it++;
        //we iterate for every possible directions
        for(auto &[key, value]: directions) {
            if(value == true) {
                is_going = true;
                //if it is out of bounds or lands on a cell with the same color piece
                if(check_cell(x+it*key.i,y+it*key.j) == 0) {
                    directions[key] = false;
                }

                //if it lands on a cell with a opposite color piece
                else if(check_cell(x+it*key.i,y+it*key.j) == 2) {
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
        if(check_cell(x+it*key.i,y+it*key.j) == 2 || check_cell(x+it*key.i,y+it*key.j) == 1)
            arr.push_back(makep(x+it*key.i,y+it*key.j));
    }
    return arr;
}


array_coords queen_t::legal_moves() {
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

    while(true) {
        bool is_going = false;
        it++;
        //we iterate for every possible directions
        for(auto &[key, value]: directions) {
            if(value == true) {
                is_going = true;
                //if it is out of bounds or lands on a cell with the same color piece
                if(check_cell(x+it*key.i,y+it*key.j) == 0) {
                    directions[key] = false;
                }

                //if it lands on a cell with a opposite color piece
                else if(check_cell(x+it*key.i,y+it*key.j) == 2) {
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