#include "board.hpp"
#include "piece.hpp"
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
    board_t board;


    //Reading the history file
    std::ifstream input;
    input.open(history);
    std::string line;
    getline(input,line);
    while (getline(input, line)) {
        board.update(line,true);
    }
    input.close();

    //Outputs the next move
    std::ofstream output;
    output.open(move);
    output << board.next_move();
    output.close();


    /* bitboard_t board_test;
    U64 occupied;
    set_bit(occupied, 12);
    set_bit(occupied, 30);
    set_bit(occupied, 25);
    set_bit(occupied, 40);
    set_bit(occupied, 19);
    set_bit(occupied, 14);
    board_test.printBBany(occupied);
    board_test.printBBany(board_test.attacksQueensMagic(28,occupied));  */


    /* for(int i =0;i <100; i++) {
        U64 x = board_test.attacksRook_mask(21,temp);
        board_test.printBBany(get_set_with_index(i,board_test.attacksRook_mask(21,temp)));
        board_test.printBBany(get_a_mask(i, x, 21));
        getchar();
    } */
    //board.print();
    //board.update_with_move(board.string_to_move("a2a3"),true);
    //board.print_grids(1);
    // std::cout << board.state[0][4]->legal_moves().size();
    //board.update("b1c3",true);
    //std::cout << board.nb_moves(4); 
    

    return 0;
}
