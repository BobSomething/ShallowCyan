#include "bitboard.hpp"
//#include "bithelpers.hpp"
//#include "magicbitboard.cpp"
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip> // For std::put_time


int main(int argc, char* argv[]) {
    bool state = true;
    std::string history, move;
    auto tracker = std::chrono::high_resolution_clock::now().time_since_epoch().count();

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
    //bboard.update_with_fen("r1bqkbnr/ppp2ppp/2n1p3/3p4/3P1B2/4PN2/PPP2PPP/RN1QKB1R b KQkq - 0 4");

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

    //std::vector<move_t*> moves;
	//bboard.generate_all_moves(&moves);

    //bboard.printBB();

    auto trackered = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::cout << "Time elapsed " << (double)(trackered-tracker)/1000000000 << " seconds" << std::endl;

    /* bitboard_t board_test;

    board_test.update_with_fen(epcases);

    std::cout << board_test.generate_fen() << std::endl;

    board_test.printBB();

    for(int i=1; i<7; i++)
        std::cout << board_test.perft(i) << std::endl;

    std::cout << board_test.generate_fen() << std::endl; */

    return 0;
}
