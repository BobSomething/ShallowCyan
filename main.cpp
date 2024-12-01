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

    return 0;
}
