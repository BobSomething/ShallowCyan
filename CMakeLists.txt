cmake_minimum_required(VERSION 3.15...3.31)

project(shallowcyan
  VERSION 1.0
  LANGUAGES CXX)

include_directories(.)

add_library(libraries board.cpp board.hpp piece.cpp piece.hpp macro.hpp)

add_executable(shallowcyan main.cpp)

target_link_libraries(shallowcyan PRIVATE libraries)