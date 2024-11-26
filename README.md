REMEMBER TO git pull FIRST, WHEN YOU WANT TO CHANGE ANYTHING!!

REMEMBER TO git push EVERYTHING AFTER YOU ARE DONE WITH THE SESSION!!

Notes about how the project will be.

Remember to name all functions/variables in a way for other to understand
+ add comments to describe the functions/things you are doing

To do list:
0. Organizing stuff
    a. piece.hpp - all types of pieces 
    b. piece.cpp - all the implementation of piece.hpp
    c. board.hpp - all board stuff related
    d. board.cpp - all the implementation of board.hpp

1. Implement all the pieces
    a. Have a struct piece_t - generic type.
    b. All struct for pawn_t, king_t, queen_t, rook_t, bishop_t, knight_t
    c. Fields: the color
    d. Methods: calculating all possible moves

2. Think how to implement the board?
    Have a 2d array, where each cell is a piece or nothing (nullptr)

3. Implement the history read, i.e. for each input move, correctly change the board.
    - Have it know if we are white or black, (checking castling)


