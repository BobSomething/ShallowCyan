#include "bitboard.hpp"

U64 bitboard_t::perft(int depth) {
    array_moves moves;
    U64 count = 0;
    if(depth == 0)
        return 1ULL;
    
    generate_all_moves(&moves);
    for(auto& move : moves) {
        int p_before = pieceTable[move->before.i*8 + move->before.j];
        int p_after = pieceTable[move->after.i*8 + move->after.j]; 
        int ep_square = enpassant_square;
        bool w_c_kside = w_castle_kside;
        bool w_c_qside = w_castle_qside; 
        bool b_c_kside = b_castle_kside;
        bool b_c_qside = b_castle_qside;
        update(move);
            /* if(depth == 1){
                printBB();
                std::cout << p_before << " " << p_after << std::endl;
                std::cout << move->before.i << " " << move->before.j << "  " << move->after.i << " " << move->after.j << " " << move->type_move << std::endl;
                //getchar(); 
            } */
        count += perft(depth-1);
        undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
    }
    return count;
}

void bitboard_t::perft_bases(int depth) {
    array_moves moves;
    U64 count = 0;
    generate_all_moves(&moves);
    for(auto& move : moves) {
        int p_before = pieceTable[move->before.i*8 + move->before.j];
        int p_after = pieceTable[move->after.i*8 + move->after.j]; 
        int ep_square = enpassant_square;
        bool w_c_kside = w_castle_kside;
        bool w_c_qside = w_castle_qside; 
        bool b_c_kside = b_castle_kside;
        bool b_c_qside = b_castle_qside;

        update(move);
        U64 branch = perft(depth-1);
        count += branch;
        std::cout << move_to_string(move) << " " << move->type_move << " : " << branch << std::endl; 
        undo(move,p_before,p_after,ep_square,w_c_kside,w_c_qside,b_c_kside,b_c_qside);
    }
    std::cout << "Summary: " << count << std::endl;
}


//These functions are a bit scuffed, feel free to improve them
std::string bitboard_t::generate_fen() {
    std::string fen = "";
    std::string temp = "";
    int counter = 0;
    for(int i=63; i>=0; i--) {
        if(pieceTable[i] != -1) {
            if(counter !=0) 
                temp = std::to_string(counter) + temp;
            temp = tofen[pieceTable[i]] + temp;
            counter = 0;
        }
        else {
            counter += 1;
        }
        if(i % 8 == 0) {
            if(counter !=0)
                temp = std::to_string(counter) + temp;
            counter = 0;
            fen += temp + "/";
            temp = "";
        }
    }
    fen.pop_back();
    fen += (turn == 1) ? " w " : " b ";
    int l = fen.size();
    if(w_castle_kside) fen += "K";

    if(w_castle_qside) fen += "Q";

    if(b_castle_kside) fen += "k";

    if(b_castle_qside) fen += "q";

    if(fen.size() == l) fen += "-";

    
    if(enpassant_square != -1) {
        int x = enpassant_square/8;
        int y = enpassant_square%8;
        fen += " " + coords_to_letter[y] + std::to_string(x+1);
    }
    else fen += " -";
    fen += " 0 1";
    return fen;
}

void bitboard_t::update_with_fen(std::string fen) {
    int index=0;
    char cur = fen[index];
    int i=7, j=0;
    for(int k=0; k<12; k++)
        piecesBB[k] = 0ULL;
    for(int k=0; k<64; k++)
        pieceTable[k] = -1;

    while(cur != ' ') {
        if('1' <= cur && cur <= '8') {
            j+= (cur - '0');
        }
        else if(cur == '/') {
            j = 0;
            i-=1;
        }
        else {
            pieceTable[i*8+j] = fromfen[cur];
            set_bit(piecesBB[fromfen[cur]],i*8+j);
            j+=1;
        }
        index+=1;
        cur = fen[index];
    }
    index++;
    cur = fen[index];
    if(cur == 'w') turn = 1;
    else turn = 0;

    index+=2;
    cur = fen[index];

    w_castle_kside = 0;
    w_castle_qside = 0;
    b_castle_kside = 0;
    b_castle_qside = 0;
    if(cur != '-') {
        while(cur != ' '){
            if(cur == 'K') w_castle_kside = 1;
            if(cur == 'Q') w_castle_qside = 1;
            if(cur == 'k') b_castle_kside = 1;
            if(cur == 'q') b_castle_qside = 1;
            index++;
            cur = fen[index];
        }
    }
    else {
        index++;
        cur = fen[index];
    }

    index++;
    cur = fen[index];
    enpassant_square = -1;
    if(cur != '-') {
        int y = letter_to_coords[std::to_string(cur)];
        int x = int(fen[index+1]);
        enpassant_square = x*8+y; 
    }
}