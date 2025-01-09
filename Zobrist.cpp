#include "bitboard.hpp"

U64 ru64() {
  U64 u1, u2, u3, u4;
  u1 = (U64)(random()) & 0xFFFF; u2 = (U64)(random()) & 0xFFFF;
  u3 = (U64)(random()) & 0xFFFF; u4 = (U64)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

void bitboard_t::zobrist_init(){
  for (int i = 0; i < 64; i++){
    for (int j = 0; j < 12; j++){
      Zobrist_table[i][j] = ru64();
    }
  }
  Zobrist_black = ru64();
}

U64 bitboard_t::zobrist_board(){
  U64 h = 0;
  if (turn) {
    h = h ^ Zobrist_black;
  }
  for (int i = 0;i<64;i++){
    if (pieceTable[i] >= 0){
      h = h ^ Zobrist_table[i][pieceTable[i]];
    }
  }
  current_board = h;
  return h;
}

U64 bitboard_t::zobrist_update(move_t* move){
  if (move->type_move < 0) {
    
  }
}