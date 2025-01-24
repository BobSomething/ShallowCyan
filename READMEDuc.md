Week 1: The goal is for us to talk about what we are gonna do, and think about what our most important functions will do before coding them.
Managed to configure the CMake 

Week 2: 
Worked on a function to printing the board and updating it with a move

Week 3:
Implemented the grid that is being attacked by all pieces except for pawn because their move forward cannot capture but still in legal moves, will modify when migrate to move_t instead of array_coords

Week 4:
We switch from array representation of the board to bitboard so I implemented legal move for rooks kings knights and pawns
Finished all moves and create a visualisation for checking which square is being attacked

Holidays:
Was enjoying my holiday so just keep up with what my teamates are doing

Week 5:
Improved the eval function to take into account the positions of the pieces instead of just counting them
Sorting the generate all move so that the alpha beta go through the "good" moves first

Week 6:
Implemented eval_moves so that we can evaluate each move to order them in the generate_all_moves function so that alpha-beta pruning go through the good move first.

Week 7:
Added a timer for the 5s rule and modify the eval_moves to prioritse check