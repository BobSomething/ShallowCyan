# Chess Bot in C++

Final Project for C++ course. Name of our chess bot: ShallowCyan

Done by: Bui Minh Duc, Sacha Gregoire, Yonas Grossard-Amin, Hai An Mai

# For the professor:

Bob - bob@bob.a is Hai An Mai
My PC - mypc@macbook-air-84.local is Bui Minh Duc

REMEMBER TO git pull FIRST, WHEN YOU WANT TO CHANGE ANYTHING!!

REMEMBER TO git push EVERYTHING AFTER YOU ARE DONE WITH THE SESSION!!

To use CMake: 
First time:
1-mkdir build
2-cd build
3-cmake ../
Then afterwards:
1-cd build
2-make

THE FINAL PROGRESS:

We are using bitboards to generate all legal moves

Some explanations:

1. We represent the board as bitboards of pieces - which each are unsigned 64-bit integer.
2. Every attack move is precomputed, hence we don't need to calculate the squares on the fly.
3. We use magic bitboards for sliding pieces, i.e. rooks, bishops and queens.
4. Every move was checked by multiple perft checks, so the move generation should be okay (and tested by multiple games).

Our algorithm:
1. Depends on the search function and the eval function.
2. To make our bot faster, we use move ordering to be able to prune more branches in the search function.
3. We use score_move function order the moves, that includes MVV-LVA and heuristics like scoring higher for capturing, checks and moving pieces, which are attacked.

Search function:
1. It is based on the alpha-beta prunning algorithm, where we go in depth, to find the best move.
2. We tried Quiescence search (same are search, but only captures), but we decided to leave it, for the time issues.
3. We also added storing killer moves and history moves for move ordering from the search function.

Evaluation function:
1. We using material evaluation and positional evaluation.
2. For both of evaluation, we use the game_phase function, where depending in which phase we are in, it computes the the interpolated score.
3. Material evaluation, computes how much each piece is worth.
4. Positional evaluation, computes how good a piece is located.

Miscellaneous:
1. We implemented Zobrist hashing, however we were skeptical of it's use in the search function, that's why we only use it to detect three-fold repetitions.
2. To implement magic bitboards, we experimented in generating random hashing keys, and brute-forcing the correct one.
3. We also implemented the timer of 10s to make sure that we won't time out (we lowered the threshold to 8s, to make sure we are not going over the time for any unexpected reason).
4. We moved the old files to old folder, i.e. the first implementation.
5. We also wanted to test our implementation, by testing the old versions, but we dropped that idea, because it was not efficient.
6. We implemented lichess API, to be able to test the bot with nice interactive environment.
