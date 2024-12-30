#ifndef MACRO_HPP_
#define MACRO_HPP_

struct move_t;

#define coords std::pair<int,int> //location of a piece
#define eval_move std::pair<int, move_t> //hold eval and move_t for alpha-beta pruning
#define array_coords std::vector<coords>
#define array_moves std::vector<move_t*>  //vector of move_t
#define i first //number
#define j second //letter
#define makep std::make_pair
#define SIZE 8
#define SIZESQ SIZE*SIZE
#define inf 2147483647

#define U64 uint64_t
#define set_bit(bitboard, index) if(index >= 0) {(bitboard |= (1ULL << index));}
#define get_bit(bitboard, index) ((bitboard>>index) & 1)
#define clear_bit(bitboard, index) (get_bit(bitboard, index) ? bitboard ^= (1ULL << index) : 0)
#define get_LSB(bitboard) (__builtin_ctzll(bitboard)) //get the least significant bit 
#define get_count(bitboard) (__builtin_popcountl(bitboard))
#endif