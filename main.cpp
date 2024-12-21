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
    while (getline(input, line)) {
        board.update(line,true);
    }
    input.close();

    //Outputs the next move
    std::ofstream output;
    output.open(move);
    output << board.next_move();
    output.close();
    bitboard_t board_test;
    
    // array_moves moves;
    
    // clear_bit(board_test.piecesBB[0],9);
    // set_bit(board_test.piecesBB[0],25);
    // board_test.pieceTable[9] = -1;
    // board_test.pieceTable[25] = 0;
    // board_test.turn = 0;
    // board_test.printBB();
    

    // std::cout << board_test.perft(2) << std::endl; 


    //  board_test.generate_all_moves(&moves);
    // for(auto& move : moves) {
    //     board_test.printBB();

    //     int p_before = board_test.pieceTable[move->before.i*8 + move->before.j];
    //     int p_after = board_test.pieceTable[move->after.i*8 + move->after.j]; 
    //     int ep_square = board_test.enpassant_square;
    //     bool w_c_kside = board_test.w_castle_kside;
    //     bool w_c_qside = board_test.w_castle_qside; 
    //     bool b_c_kside = board_test.b_castle_kside;
    //     bool b_c_qside = board_test.b_castle_qside;

    //     board_test.update(move);
    //     board_test.printBB();
    //     std::cout << move->before.i << " " << move->before.j << "  " << move->after.i  << " " << move->after.j << std::endl;
    //     std::cout << p_before << " " << p_after << "  " << move->type_move << std::endl;

    //     for(int k=0; k<64; k++) {
    //         std::cout << board_test.pieceTable[k] << " ";
    //         if(k % 8 == 7)
    //             std::cout << std::endl;
    //     }

    //     getchar();
        
    //     board_test.undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
    // } 
    // U64 occupied;
    // set_bit(occupied, 12);
    // set_bit(occupied, 30);
    // set_bit(occupied, 25);
    // set_bit(occupied, 40);
    // set_bit(occupied, 19);
    // set_bit(occupied, 14);
    // board_test.printBBany(occupied);
    // board_test.printBBany(board_test.attacksQueensMagic(28,occupied));  */
    //array_coords ar = board_test.u64_to_coords(3);
    //std::cout << ar[1].i << ar[1].j;


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
