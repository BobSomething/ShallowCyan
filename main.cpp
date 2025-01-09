#include "bitboard.hpp"
//#include "bithelpers.hpp"
//#include "magicbitboard.cpp"
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
    bitboard_t bboard;

    //Reading the history file
    std::ifstream input;
    input.open(history);
    std::string line;
    while (getline(input, line)) {
        bboard.update_string(line);
        bboard.nb_turns++;
    }
    input.close();

    //Outputs the next move
    std::ofstream output;
    output.open(move);
    output << bboard.next_move();
    output.close();

    bboard.printBB();

    //array_moves moves;
    //bboard.generate_all_moves(&moves);

    
    /* bitboard_t board_test;

    board_test.update_with_fen(epcases);

    std::cout << board_test.generate_fen() << std::endl;

    board_test.printBB();

    for(int i=1; i<7; i++)
        std::cout << board_test.perft(i) << std::endl;

    std::cout << board_test.generate_fen() << std::endl; */

    return 0;
}
