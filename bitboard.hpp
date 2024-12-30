#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <cstdint>   //added this cause the CMake wasnt happy
#include <iostream>
#include <map>
#include "macro.hpp"
#include "board.hpp"


/*
If first time read this:
For bit operations use & |. and NOT && ||
You can also use the macros get_bit(), set_bit(), get_LSB()
To test your implementation of a bitboard you can use printBBany(bitboard)
*/

/*

USEFUL BIT FUNCTIONS

*/


/*

FOR MOVE_T TYPES:
-1 = capture
 0 = normal (quiet)
 1 = castle
-2 = en passant
 3 = double pawn push
 7 = promotion knight, -7 = if it is with a capture
 8 = promotion bishop, -8 = if it is with a capture
 9 = promotion rook,   -9 = if it is with a capture
10 = promotion queen, -10 = if it is with a capture

*/


/* Interesting test positions with fen notation - to use it with update_with_fen, and it will update the board */
//All positions from https://www.chessprogramming.org/Perft_Results
const std::string kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";

const std::string epcases = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";

const std::string p4white = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

const std::string p4black = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";

const std::string edwards1 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

const std::string edwards2 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";


struct bitboard_t {
    array_coords u64_to_coords(U64 u);
    std::vector<int> u64_to_index(U64 u);

    //pretty scuffed way, but I guess it works, having dictionaries in a .hpp (I cannot add it outside the struct, because it yells, because the maps are defined multiple times, if you guys have any other solution, feel free to fix this)
    #include <dictionaries.hpp>
    
    /*
    0 - white pawns
    1 - white knights
    2 - white bishops
    3 - white rooks
    4 - white queens
    5 - white king
    6 - black pawns
    7 - black knights
    8 - black bishops
    9 - black rooks
    10 - black queens
    11 - black king 
    */
    U64 piecesBB[12] {};            // bitboards for all the pieces, the description is above
    bool turn;                      // turn
    int pieceTable[SIZESQ];         // keep track where are the pieces easily: output is a number correspoding to the description above
    int kingWhere[2];               // keep track where is the king - location 0 -> 63
    int enpassant_square = -1;      // the square, of which the pawn could en passant the opposite pawn, -1 = if enpassant is not possible
    bool w_castle_kside = 1;
    bool w_castle_qside = 1;
    bool b_castle_kside = 1;
    bool b_castle_qside = 1;

    bitboard_t();                   // Initializing the bitboards

    /* To debug */
    void printBBpiece(int index);   // print the current bitboard of specific piece on the board
    void printBBany(U64 u64);       // print any U64 integer, thus any kind of bitboard
    void printBB();                 // print the current board
    void printBBattacked(bool color); // print the board and the square attacked by color


    /* Turns the type move_t to a move in form of the string */
	std::string move_to_string(move_t* move);
	/* Turns a move in form of the string to the type move_t */
	move_t* string_to_move(std::string move);

    /* Updates the move, with a string notation, assumes it is always true */
    void update_string(std::string move);

    /* Updates the state with the move, returns true if legal, else false */ //kinda stupid implementation, sorry
    bool update(move_t* move);

    /* Checks if the move is legal, i.e. if in the end the king is not in check */
    bool is_legal(move_t* move);

    /* Undoes the state with the move inputs every information stored before */
    void undo(move_t* move, int p_before, int p_after, int ep_square, bool w_c_kside, bool w_c_qside, bool b_c_kside, bool b_c_qside);

    /* Checks, if it is a checkmate for the current player */
    bool is_checkmate(); 

    /* Outputs the next move */
    std::string next_move();


    /* Attack masks */ // implementation in bitmoves.cpp
    /* i.e. where a piece can attack, this is just the mask, to find the attacking moves refer below */
    U64 attacksKing_mask(int square);
    U64 attacksPawns_mask(int square, bool color); // need to specify the color, for different masks
    U64 attacksKnight_mask(int square);

    /* masks for sliding pieces, THE ONLY USE is to initialize the board, to find attacking moves of a piece refer to magic */
    U64 attacksBishop_mask(int square, U64 occupied);
    U64 attacksRook_mask(int square, U64 occupied);
    U64 attacksQueen_mask(int square, U64 occupied);

    /* masks that stops 1 before the edge and assumes that there are no blockers - ONLY USED to initialize the board*/
    U64 attacksBishop_mask_short(int square);
    U64 attacksRook_mask_short(int square);

    /* Precomputed data of all moves */
    /* USE THESE TABLES FOR THE POSSIBLE ATTACKS */
    U64 attacksKing[SIZESQ];
    U64 attacksPawns[2][SIZESQ];
    U64 attacksKnights[SIZESQ];
    
    //7 = 3206564034472059008ULL
    //56 = 6790656835977289ULL

    /* Magic numbers for the rooks and bishops on each square */
    U64 magicRooks[64] = {2341871943977798784ULL, 2449975935508709640ULL, 72066669340581890ULL, 648523848749154312ULL, 180152783337684994ULL, 2449984654625079369ULL, 144159752690073884ULL, 270357282336932352ULL, 41236086239789923ULL, 9228649761425350656ULL, 1154329016946148368ULL, 140771849142402ULL, 9367768734267408400ULL, 292874721857634432ULL, 281479271809028ULL, 1155736271570616450ULL, 630539681963966593ULL, 49539870783184904ULL, 1171363063652163584ULL, 2252349837967368ULL, 1171220676762144772ULL, 432628138850255872ULL, 2380156801246167312ULL, 9044582654353540ULL, 70379481759776ULL, 73747552002392195ULL, 45036548178051104ULL, 318866962518336ULL, 2030015145698656512ULL, 1154047413137180688ULL, 577023728027173891ULL, 18208470901739780ULL, 36064256277348496ULL, 11610280182962716712ULL, 2305878200053403648ULL, 4037479265230389504ULL, 2467306265380864ULL, 9147945341420544ULL, 9223943787934388752ULL, 864695666121644097ULL, 36346007198531588ULL, 2339761281089536ULL, 4503875042214144ULL, 1441169477240160288ULL, 1126449796907013ULL, 9228438623553192016ULL, 88272349167617ULL, 181270520662196225ULL, 1189550699407491584ULL, 1189550699407491584ULL, 2314868080047554816ULL, 4504699274273024ULL, 5638295694377088ULL, 4398080098432ULL, 2508287124480ULL, 180144539153203712ULL, 2815853729948160ULL, 4688282467335930113ULL, 36890857621880897ULL, 9083066094977081ULL, 324540716900941829ULL, 72339172127670275ULL, 9007754513616900ULL, 290623209815151138ULL};
    int rookShifts[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

    U64 magicBishops[64] = {9317948771500558592ULL, 14078164666958848ULL, 9227876874657465292ULL, 4901060170157719618ULL, 1165619831506432ULL, 14430099111061495808ULL, 186345316648484912ULL, 569823042232324ULL, 9530778209361592401ULL, 5188181989497274922ULL, 14078164666958848ULL, 36670954761355264ULL, 9223517190106384658ULL, 4611708592863582720ULL, 720580409360130176ULL, 569823042232324ULL, 14078164666958848ULL, 81078023170032705ULL, 22588650416767010ULL, 7280068799833899008ULL, 162412172083726336ULL, 90494227695550465ULL, 2594146528679698432ULL, 4630043473163324418ULL, 1738496118523954177ULL, 72347938191837189ULL, 155462148628415104ULL, 11263947273535492ULL, 76706329476997120ULL, 2256816369058816ULL, 1193739877359945756ULL, 144419754009166016ULL, 3459336277048959008ULL, 1176020063179768096ULL, 4620838357513011266ULL, 2342172007275429952ULL, 1126175053447424ULL, 193524786069632ULL, 4616757245261137928ULL, 1177709463843438690ULL, 144749626690772994ULL, 18614736228851752ULL, 9044720894380041ULL, 1153494633767133312ULL, 653030918222528576ULL, 4612829649066598464ULL, 9227876839081910528ULL, 38324785612587044ULL, 186345316648484912ULL, 576531414448081408ULL, 18032549062311990ULL, 4611686019534815232ULL, 180152857188764704ULL, 70377401288704ULL, 11285666659172360ULL, 14078164666958848ULL, 569823042232324ULL, 569823042232324ULL, 9223381658724213760ULL, 1152921578160293952ULL, 74344578358117376ULL, 4899916575102999584ULL, 9530778209361592401ULL, 9317948771500558592ULL};
    int bishopShifts[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

    /* Maps for the precomputed data => all possible moves, for all possible formation of blockers */
    std::map<U64,U64> attacksBishops[SIZESQ];
    std::map<U64,U64> attacksRooks[SIZESQ];

    /* Table for attacks of bishops and rooks when there are no blockers -> USED ONLY for the functions below */
    U64 attacksBishopsNoBlockers[SIZESQ];
    U64 attacksRooksNoBlockers[SIZESQ];

    /* To find possible attacks of sliding pieces, USE THESE FUNCTIONS */
    /* BIG NOTE: This gives us possible attack squares, + additional care needed to remove the possibility of capturing a piece, with the same color*/
    U64 attacksBishopsMagic(int square, U64 occupied);
    U64 attacksRooksMagic(int square, U64 occupied);
    U64 attacksQueensMagic(int square, U64 occupied);

    /* Use precomputed data */
    bool is_square_attacked(int square, bool color);


    //TODO
    //For pawns, need to remove the attacking the same color pieces + en passant + moving 1 forward + moving 2 forward
    //USE attacksPawns table
    void allMovesPawns(bool color, array_moves* moves);
    
    //For knights, need to remove the attacking the same color pieces
    //USE attacksKnights table
    void allMovesKnights(bool color, array_moves* moves);

    //For kings, need to remove the attacking the same color pieces + checking is_squared_attacked
    //USE attacksKing table
    void allMovesKing(bool color, array_moves* moves);
    
    //For sliding pieces, need to remove the attacking the same color pieces
    //USE attacks...Magic functions!
    void allMovesBishop(bool color, array_moves* moves);
    void allMovesRooks(bool color, array_moves* moves);
    void allMovesQueens(bool color, array_moves* moves);

    //Generates all pseudo-legal moves (we don't check king's check yet)  (in bitallmoves.cpp)
    void generate_all_moves(array_moves* moves);

    void generate_all_legal_moves(array_moves* moves);

    //Perft = performance test, to test the number of legal moves in some position, at some depth
    U64 perft(int depth);

    //Breakdown of perft, counts the number of moves after each root move (each first move from the position)
    void perft_bases(int depth);

    //Use it for quicker debugging, fen notation look on https://www.chessprogramming.org/Forsyth-Edwards_Notation for more info
    std::string generate_fen();
    void update_with_fen(std::string fen);

    // Create a copy of the bitboard
    bitboard_t copy();

    //create the search function for alpha-beta prunning
    eval_move search_aux(int depth, int α = -inf, int β = inf);
    move_t* search(int depth);

    int eval();
}; 

#endif