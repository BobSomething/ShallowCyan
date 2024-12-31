set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_minimum_required(VERSION 3.15...3.31)

project(shallowcyan
  VERSION 1.0
  LANGUAGES CXX)

include_directories(.)

#board.cpp board.hpp piece.cpp piece.hpp

add_library(libraries  macro.hpp dictionaries.hpp 
bitboard.hpp bitboard.cpp 
bitmoves.cpp bitallmoves.cpp 
bithelpers.hpp magicbitboard.cpp
perft_fen.cpp
search.cpp)

add_executable(shallowcyan main.cpp)

target_link_libraries(shallowcyan PRIVATE libraries)