#ifndef MACRO_HPP_
#define MACRO_HPP_

struct move_t;

#define coords std::pair<int,int> //location of a piece
#define array_moves std::vector<move_t*>  //vector of move_t
#define i first //number
#define j second //letter
#define makep std::make_pair
#define SIZE 8
#define SIZESQ SIZE*SIZE
#define inf 2147483647

#define U64 uint64_t
#define set_bit(bitboard, index) (bitboard |= (1ULL << index))
#define get_bit(bitboard, index) ((bitboard>>index) & 1)
#define get_LSB(bitboard) (__builtin_ctzll(bitboard)) //get the least significant bit 

#endif