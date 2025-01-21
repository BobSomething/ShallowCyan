#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <iostream>
#include <map>
#include <chrono>
#include "macro.hpp"

/*
If first time read this:
For bit operations use & |. and NOT && ||
You can also use the macros get_bit(), set_bit(), get_LSB()
To test your implementation of a bitboard you can use printBBany(bitboard)
*/


/* Interesting test positions with fen notation - to use it with update_with_fen, and it will update the board */
//All positions from https://www.chessprogramming.org/Perft_Results
const std::string kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
const std::string epcases = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
const std::string p4white = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const std::string p4black = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
const std::string edwards1 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
const std::string edwards2 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";


struct move_t {
	coords before;
	coords after;
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

	int type_move; 
	int score = 0; //for move ordering
	int eval = 0; //for search
    int piece = -1;
    int capture_piece = -2; //-1 if not capture, piece number otherwise

	move_t(int before_i, int before_j, int after_i, int after_j, int type_move)
		: before {makep(before_i,before_j)}, after {makep(after_i, after_j)}, type_move {type_move} {}
	
	move_t(coords before, coords after)
		: before {before}, after {after}, type_move {0} {}

	move_t(coords before, coords after, int type_move)
		: before {before}, after {after}, type_move {type_move} {}

    move_t(coords before, coords after, int type_move, int piece)
        : before {before}, after {after}, type_move {type_move}, piece {piece}, capture_piece{-1} {}

    move_t(coords before, coords after, int type_move, int piece, int capture_piece)
        : before {before}, after {after}, type_move {type_move}, piece {piece}, capture_piece {capture_piece} {}

	move_t()
		: before {makep(-10,-10)}, after {makep(-10,-10)}, type_move {-10} {} //not initialized
};


struct bitboard_t {
    array_coords u64_to_coords(U64 u);
    std::vector<int> u64_to_index(U64 u);

    //pretty scuffed way, but I guess it works, having dictionaries in a .hpp (I cannot add it outside the struct, because it yells, because the maps are defined multiple times, if you guys have any other solution, feel free to fix this)
    #include <dictionaries.hpp>
    
    /*
    -1 - No piece
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
    int enpassant_square = -1;      // the square, of which the pawn could en passant the opposite pawn, -1 = if enpassant is not possible
    
    bool w_castle_kside = true;     // white kingside castling rights
    bool w_castle_qside = true;     // white queenside castling rights
    bool b_castle_kside = true;     // black kingside castling rights 
    bool b_castle_qside = true;     // black queenside castling rights

    int nb_turns = 0;               // number of turns = number of moves in the game

    bitboard_t();                   // Initializing the bitboards
    
    //zobrist-hashing
    U64 Zobrist_table[64][12];
    U64 Zobrist_black;
    U64 hash_current_board;
    void zobrist_init();
    U64 zobrist_board();
    U64 zobrist_update(move_t* move);
    std::map<U64,int> zobrist_hash_map;
    std::map<U64,int> counter_hash_map;


    /* Precomputed data of all moves */
    /* USE THESE TABLES FOR THE POSSIBLE ATTACKS */
    U64 attacksKing[SIZESQ];    
    U64 attacksPawns[2][SIZESQ];
    U64 attacksKnights[SIZESQ];

    /* Magic numbers for the rooks and bishops on each square */
    U64 magicRooks[64] = {9979994641325359136ULL, 90072129987412032ULL, 180170925814149121ULL, 72066458867205152ULL, 144117387368072224ULL, 216203568472981512ULL, 9547631759814820096ULL, 2341881152152807680ULL, 140740040605696ULL, 2316046545841029184ULL, 72198468973629440ULL, 81205565149155328ULL, 146508277415412736ULL, 703833479054336ULL, 2450098939073003648ULL, 576742228899270912ULL, 36033470048378880ULL, 72198881818984448ULL, 1301692025185255936ULL, 90217678106527746ULL, 324684134750365696ULL, 9265030608319430912ULL, 4616194016369772546ULL, 2199165886724ULL, 72127964931719168ULL, 2323857549994496000ULL, 9323886521876609ULL, 9024793588793472ULL, 562992905192464ULL, 2201179128832ULL, 36038160048718082ULL, 36029097666947201ULL, 4629700967774814240ULL, 306244980821723137ULL, 1161084564161792ULL, 110340390163316992ULL, 5770254227613696ULL, 2341876206435041792ULL, 82199497949581313ULL, 144120019947619460ULL, 324329544062894112ULL, 1152994210081882112ULL, 13545987550281792ULL, 17592739758089ULL, 2306414759556218884ULL, 144678687852232706ULL, 9009398345171200ULL, 2326183975409811457ULL, 72339215047754240ULL, 18155273440989312ULL, 4613959945983951104ULL, 145812974690501120ULL, 281543763820800ULL, 147495088967385216ULL, 2969386217113789440ULL, 19215066297569792ULL, 180144054896435457ULL, 2377928092116066437ULL, 9277424307650174977ULL, 4621827982418248737ULL, 563158798583922ULL, 5066618438763522ULL, 144221860300195844ULL, 281752018887682ULL}; 
    int rookShifts[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

    U64 magicBishops[64] = {18018832060792964ULL, 9011737055478280ULL, 4531088509108738ULL, 74316026439016464ULL, 396616115700105744ULL, 2382975967281807376ULL, 1189093273034424848ULL, 270357282336932352ULL, 1131414716417028ULL, 2267763835016ULL, 2652629010991292674ULL, 283717117543424ULL, 4411067728898ULL, 1127068172552192ULL, 288591295206670341ULL, 576743344005317120ULL, 18016669532684544ULL, 289358613125825024ULL, 580966009790284034ULL, 1126071732805635ULL, 37440604846162944ULL, 9295714164029260800ULL, 4098996805584896ULL, 9223937205167456514ULL, 153157607757513217ULL, 2310364244010471938ULL, 95143507244753921ULL, 9015995381846288ULL, 4611967562677239808ULL, 9223442680644702210ULL, 64176571732267010ULL, 7881574242656384ULL, 9224533161443066400ULL, 9521190163130089986ULL, 2305913523989908488ULL, 9675423050623352960ULL, 9223945990515460104ULL, 2310346920227311616ULL, 7075155703941370880ULL, 4755955152091910658ULL, 146675410564812800ULL, 4612821438196357120ULL, 4789475436135424ULL, 1747403296580175872ULL, 40541197101432897ULL, 144397831292092673ULL, 1883076424731259008ULL, 9228440811230794258ULL, 360435373754810368ULL, 108227545293391872ULL, 4611688277597225028ULL, 3458764677302190090ULL, 577063357723574274ULL, 9165942875553793ULL, 6522483364660839184ULL, 1127033795058692ULL, 2815853729948160ULL, 317861208064ULL, 5765171576804257832ULL, 9241386607448426752ULL, 11258999336993284ULL, 432345702206341696ULL, 9878791228517523968ULL, 4616190786973859872ULL};   
    int bishopShifts[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

    /* Maps for the precomputed data => all possible moves, for all possible formation of blockers */
    std::map<U64,U64> attacksBishops[SIZESQ];
    std::map<U64,U64> attacksRooks[SIZESQ];


    /*

    FUNCTIONS IMPLEMENTED IN BITBOARD.CPP
    
    */

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

    /* Checks if the square is attacked by the color */
    bool is_square_attacked(int square, bool color);

    /* Checks, if there is no moves for the current player */
    bool no_moves(); 

    /* Outputs the next move */
    std::string next_move();

    /* Create a copy of the bitboard */
    bitboard_t copy();

    /* Evaluates the current state of the board */
    int eval();


    //you can get inspired from https://www.chessprogramming.org/Simplified_Evaluation_Function
    //you can implement for only white, for black's turn we can "flip" the board to get correct values
    //TODO
    int scorePawnsOpening[64] ={ 0,  0,  0,  0,  0,  0,  0,  0, 
                                 5, 10, 10,-20,-20, 10, 10,  5,
                                 5, -5,-10,  0,  0,-10, -5,  5,
                                 0,  0,  0, 30, 30,  0,  0,  0,
                                 5,  5, 10, 25, 25, 10,  5,  5,
                                 10, 10, 20, 30, 30, 20, 10, 10,
                                 50, 50, 50, 50, 50, 50, 50, 50,
                                 0,  0,  0,  0,  0,  0,  0,  0 }; //flipped compared to the real board

    int scorePawnsEnding[64] = { 0,  0,  0,  0,  0,  0,  0,  0, 
                                 10, 10, 10, 10, 10, 10, 10, 10, 
                                 20, 20, 20, 20, 20, 20, 20, 20,
                                 30, 30, 30, 30, 30, 30, 30, 30,
                                 40, 40, 40, 40, 40, 40, 40, 40,
                                 50, 50, 50, 50, 50, 50, 50, 50,
                                 100,100,100,100,100,100,100,100,
                                 0,  0,  0,  0,  0,  0,  0,  0};

    int scoreRooks[64] = { 0,  0,  0,  5,  5,  0,  0,  0,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                          -5,  0,  0,  0,  0,  0,  0, -5,
                           5, 10, 10, 10, 10, 10, 10,  5,
                           0,  0,  0,  0,  0,  0,  0,  0};

    int scoreKnights[64] = {-50,-40,-30,-30,-30,-30,-40,-50,
                            -40,-20,  0,  5,  5,  0,-20,-40,
                            -30,  5, 10, 15, 15, 10,  5,-30,
                            -30,  0, 15, 20, 20, 15,  0,-30,
                            -30,  5, 15, 20, 20, 15,  5,-30,
                            -30,  0, 10, 15, 15, 10,  0,-30,
                            -40,-20,  0,  0,  0,  0,-20,-40,
                            -50,-40,-30,-30,-30,-30,-40,-50};

    int scoreBishops[64] = {-20,-10,-10,-10,-10,-10,-10,-20,
                            -10,  5,  0,  0,  0,  0,  5,-10,
                            -10, 10, 10, 10, 10, 10, 10,-10,
                            -10,  0, 10, 10, 10, 10,  0,-10,
                            -10,  5,  5, 10, 10,  5,  5,-10,
                            -10,  0,  5, 10, 10,  5,  0,-10,
                            -10,  0,  0,  0,  0,  0,  0,-10,
                            -20,-10,-10,-10,-10,-10,-10,-20};

    int scoreQueensOpening[64]={-20,-10,-10, -5, -5,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,-50,-50,-50,-50,  0,-10,
                                 -5,  0,-50,-50,-50,-50,  0, -5,
                                 -5,  0,-50,-50,-50,-50,  0, -5,
                                -10,  0,-50,-50,-50,-50,  0,-10,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -20,-10,-10, -5, -5,-10,-10,-20};

    int scoreQueensEnding[64] ={-20,-10,-10,  0, -5,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5,  5,  5,  5,  0,-10,
                                 -5,  0,  5,  5,  5,  5,  0, -5,
                                 -5,  0,  5,  5,  5,  5,  0, -5,
                                -10,  0,  5,  5,  5,  5,  0,-10,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -20,-10,-10, -5, -5,-10,-10,-20};

    int scoreKingOpening[64] = { 20, 30, 10, -5, -5, 10, 30, 20,
                                 20, 20,  0,  0,  0,  0, 20, 20,
                                -10,-20,-20,-20,-20,-20,-20,-10,
                                -20,-30,-30,-40,-40,-30,-30,-20,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30,
                                -30,-40,-40,-50,-50,-40,-40,-30};

    int scoreKingEnding[64] = {-50,-30,-30,-30,-30,-30,-30,-50,
                               -30,-30,  0,  0,  0,  0,-30,-30,
                               -30,-10, 20, 30, 30, 20,-10,-30,
                               -30,-10, 30, 40, 40, 30,-10,-30,
                               -30,-10, 30, 40, 40, 30,-10,-30,
                               -30,-10, 20, 30, 30, 20,-10,-30,
                               -30,-20,-10,  0,  0,-10,-20,-30,
                               -50,-40,-30,-20,-20,-30,-40,-50};


    /*
    
    FUNCTIONS IMPLEMENTED IN BITMOVES.CPP

    */

    //DO NOT USE THESE FUNCTIONS, WE HAVE PRECOMPUTED EVERY POSSIBLE ATTACK WITH THE TABLES ABOVE

    /* Attack masks */
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

    /* Table for attacks of bishops and rooks when there are no blockers -> USED ONLY for the functions below */
    U64 attacksBishopsNoBlockers[SIZESQ];
    U64 attacksRooksNoBlockers[SIZESQ];

    /* To find possible attacks of sliding pieces, USE THESE FUNCTIONS */
    /* BIG NOTE: This gives us possible attack squares, + additional care needed to remove the possibility of capturing a piece, with the same color*/
    U64 attacksBishopsMagic(int square, U64 occupied);
    U64 attacksRooksMagic(int square, U64 occupied);
    U64 attacksQueensMagic(int square, U64 occupied);


    /*
    
    FUNCTIONS IMPLEMENTED IN BITALLMOVES.CPP

    */


    void allMovesPawns(bool color, array_moves* moves, bool only_capture = false);
    void allMovesKnights(bool color, array_moves* moves, bool only_capture = false);
    void allMovesKing(bool color, array_moves* moves, bool only_capture = false);
    void allMovesBishop(bool color, array_moves* moves, bool only_capture = false);
    void allMovesRooks(bool color, array_moves* moves, bool only_capture = false);
    void allMovesQueens(bool color, array_moves* moves, bool only_capture = false);

    void generate_all_moves(array_moves* moves, bool only_capture = false);


    /*
    
    FUNCTIONS IMPLEMENTED IN PERFT_FEN.CPP
    
    */


    //Perft = performance test, to test the number of legal moves in some position, at some depth
    U64 perft(int depth);

    //Breakdown of perft, counts the number of moves after each root move (each first move from the position)
    void perft_bases(int depth);

    //Use it for quicker debugging, fen notation look on https://www.chessprogramming.org/Forsyth-Edwards_Notation for more info
    std::string generate_fen(); // did not implement half-clock stuff
    void update_with_fen(std::string fen);


    /*
    
    FUNCTIONS IMPLEMENTED IN SEARCH.CPP
    
    */


    //create the search function for alpha-beta prunning
    move_t* search(int depth, int α = -inf, int β = inf, double time = 0);
    move_t* Quiescence_search(int depth, int α = -inf, int β = inf, double time = 0);
    int score_move(move_t* move);

    //Map for translate pieces to their correspond points
    std::map<int, int> pieces_to_points = {
        {0, 100},       //white pawns
        {1, 300},       //white knights
        {2, 300},       //white bishops
        {3, 500},       //white rooks
        {4, 900},       //white queen
        {6, 100},       //black pawns
        {7, 300},       //black knights
        {8, 300},       //black bishops
        {9, 500},       //black rooks
        {10,900}       //black queen
    };

    int MVV_LVA[12][12] = {
        105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

        105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
    };
}; 

#endif