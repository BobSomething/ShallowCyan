Week 1:
Worked on board.hpp and board.cpp, defining basic fields and methods (constructor, print, eval, etc.) and implementing them.

December: 
Made the skeleton code for alpha-beta pruning (search function).
I do not remember that well, but overall helped structure the different classes used in the project.
Specifically, spent quite a while structuring the classes for pieces that we no longer use since we switched to bitboards.

January:
Optimized code, including making search function and move generating functions more efficient.
Worked on improving board evaluation function. Also worked on move scoring function.
Implemented move ordering in order to look at specific moves before others (in order to prune lines more effectively).
Fixed bugs regarding finding checkmate and correctly avoiding it if it was for the opponent, and playing it if it was your own mate.
Helped implement the timing feature to make sure the bot does not time out (which hopefully works).
Tried very hard to understand why our bot plays awful moves sometimes. Failed.