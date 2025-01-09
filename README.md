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

CURRENT PROGRESS:

We are now operating on bitboards.

Some explanations:

1. We represent the board as bitboards of pieces - which are unsigned 64-bit integer.
2. Every attack move is precomputed, hence we don't need to calculate the squares on the fly.
3. Every move was checked by multiple perft checks, so the move generation should be okay.

Our algorithm right now is:

1. At the beginning we return a random legal move.
2. Then we use the search function to find the best moves.

The search function:
1. alpha-beta prunning, no improvements right now.

TODO:
1. improve alpha-beta prunning, make some good guesses, to be able to prune faster, like promotion, capture, checks - good
2. zobrist hashing - hash positions, so we can see if the state was already reached or not, and store the eval calculated
3. openings book moves => probably encode these with fen notation, or zobrist hashing
4. heuristics for endgames and openings -> where to move the pieces:
    like kings want to castle in the beginning, knights want to be more in the center and so on.
