#include "bitboard.hpp"
#include "bithelpers.hpp"
#include <random>

bitboard_t bb;

/* random 64 bit integer */

U64 ru64() {
  U64 u1, u2, u3, u4;
  u1 = (U64)(random()) & 0xFFFF; u2 = (U64)(random()) & 0xFFFF;
  u3 = (U64)(random()) & 0xFFFF; u4 = (U64)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 ru64_fewbits() {
  return ru64() & ru64() & ru64();
}

U64 get_a_mask(int index, U64 attack_mask, int square) {
    U64 occupied = get_set_with_index(index,attack_mask);
    return bb.attacksRook_mask(square,occupied);
}

int get_actual_size_of_a_map(std::map<U64,U64> map) {
    return sizeof(map) + map.size() * (sizeof(decltype(map)::key_type) + sizeof(decltype(map)::mapped_type));
}

U64 get_a_magic_number_rook(int square) {
    srand(time(0));
    U64 current_magic;
    while(true) {
        current_magic = ru64_fewbits();
        //std::cout << current_magic << std::endl;
        U64 attack_mask = bb.attacksRook_mask(square,0);
        int n = get_count(attack_mask);
        U64 value, key;
        std::map<U64,U64> map;
        int i;
        for(i=0; i<=(1<<n); i++) {
            value = get_a_mask(i, attack_mask, square);
            key = (value * current_magic) >> (64 - n);
            if(map.find(key) != map.end() && map[key] != value) {
                /* for(auto& [k,v] : map) {
                    std::cout << k << " " << v <<std::endl;
                    //bb.printBBany(v);
                }
                std::cout << "WOOO " << key << " " << value << std::endl;
                //bb.printBBany(value);
                error("yo"); */
                break;
            }
            map[key] = value;
        }
        if(i == (1<<n) + 1)
            break;
    }

    return current_magic;
}

void get_magic_numbers_rook() {
    /* for(int i=0; i<64; i++) {
        std::cout << i << ": " << get_a_magic_number_rook(i) << std::endl;
    } */
    for(int i=0; i<64; i++) {
        std::cout << get_a_magic_number_rook(i) << ", ";
    }
}

U64 magicRooks[] = {1513212155661652497, 3942301228605449, 3942301228605449, 37154851561410900, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 1513212155661652497, 41236432014016578, 288274363069759617, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 9511603042286305408, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449, 3942301228605449};