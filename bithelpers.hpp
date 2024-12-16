#ifndef BITHELPERS_HPP_
#define BITHELPERS_HPP_

#include "macro.hpp"
#include <iostream>
#include <map>

U64 get_set_with_index(int index, U64 mask) {
    U64 set = 0ULL;
    int nb_bits = get_count(mask);
    for(int i=0; i<nb_bits; i++) {
        int id = get_LSB(mask);
        clear_bit(mask,id);
        if (index & (1 << i)) {
            set |= (1ULL << id);
        }
    }
    return set;
}




#endif
