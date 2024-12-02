#include "board.hpp"
#include "piece.hpp"
#include <fstream>
#include <cstring>

int main(int argc, char* argv[]) {
    bool state = true;
    std::string history, move;
    //Taking all the arguments
    for(int i = 1; i < argc; i++) {
        // Checking all if the next argument is a history file or a move file
        if(strcmp(argv[i],"-H") == 0) {
            state = true;
            continue;
        }
        if(strcmp(argv[i],"-m") == 0) {
            state = false;
            continue;
        }
        if(state == true) history = argv[i];
        else move = argv[i];
    }

    //Initializing the board
    board_t board;


    //Reading the history file
    std::ifstream input;
    input.open(history);
    std::string line;
    getline(input,line);
    while (getline(input, line)) {
        board.update(line);
    }
    input.close();

    //Outputs the next move
    std::ofstream output;
    output.open(move);
    output << board.next_move();
    output.close();

    /*
    board.clear_board();
    board.add_piece(3,3, new bishop_t(makep(3,3),1,&board));
    board.add_piece(1,2, new bishop_t(makep(1,2),1,&board));
    board.add_piece(0,0, new rook_t(makep(0,0),1,&board));
    
    std::cout << typeid(board.state[1][2]).name() << " " << board.state[1][2]->id << std::endl;
    std::cout << typeid(board.state[3][3]).name() << " " << board.state[3][3]->id << std::endl;
    std::cout << typeid(board.state[0][0]).name() << " " << board.state[0][0]->id << std::endl;

    for(auto& [x, y]: board.state[3][3]->legal_moves()) {
        std::cout << x << " " << y << std::endl;
    }
    */
    

    return 0;
}
