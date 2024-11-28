cmake_minimum_required(VERSION 3.15...3.31)

project(shallowcyan
  VERSION 1.0
  LANGUAGES CXX)

include_directories(.)

add_executable(chess main.cpp)

target_link_libraries(chess)