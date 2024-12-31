#ifndef DICTIONARIES_HPP_
#define DICTIONARIES_HPP_

#include<macro.hpp>
#include<iostream>
#include<map>


std::map<std::pair<std::string,bool>,int> convertPiece = {
        {makep("p",1), 0},
        {makep("n",1), 1},
        {makep("b",1), 2},
        {makep("r",1), 3},
        {makep("q",1), 4},
        {makep("k",1), 5},
        {makep("p",0), 6},
        {makep("n",0), 7},
        {makep("b",0), 8},
        {makep("r",0), 9},
        {makep("q",0), 10},
        {makep("k",0), 11},
    };

    std::map<int, std::string> coords_to_letter = { // Dictionary to translate input to move
        {0, "a"},
        {1, "b"},
        {2, "c"},
        {3, "d"},
        {4, "e"},
        {5, "f"},
        {6, "g"},
        {7, "h"}
    };

    std::map<std::string, int> letter_to_coords = { // Dictionary to translate input to move
        {"a" , 0},
        {"b" , 1},
        {"c" , 2},
        {"d" , 3},
        {"e" , 4},
        {"f" , 5},
        {"g" , 6},
        {"h" , 7}
    };

    std::map<int,std::string> index_to_piece {
        {12, " "},
        {0, "\u265F"},
        {1, "\u265E"},
        {2, "\u265D"},
        {3, "\u265C"},
        {4, "\u265B"},
        {5, "\u265A"},
        {6, "\u2659"},
        {7, "\u2658"},
        {8, "\u2657"},
        {9, "\u2656"},
        {10, "\u2655"},
        {11, "\u2654"},
    };

    std::map<int,std::string> tofen = {
        {0, "P"},
        {1, "N"},
        {2, "B"},
        {3, "R"},
        {4, "Q"},
        {5, "K"},
        {6, "p"},
        {7, "n"},
        {8, "b"},
        {9, "r"},
        {10, "q"},
        {11, "k"}
    };

    std::map<char,int> fromfen = {
        {'P', 0},
        {'N', 1},
        {'B', 2},
        {'R', 3},
        {'Q', 4},
        {'K', 5},
        {'p', 6},
        {'n', 7},
        {'b', 8},
        {'r', 9},
        {'q', 10},
        {'k', 11}
    };

#endif