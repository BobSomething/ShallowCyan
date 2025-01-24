Week 1: The goal is for us to talk about what we are gonna do, and think about what our most important functions will do before coding them.
Managed to configure the CMake 

Week 2: 
Helped some comrades with Cmake
Researched the Alpha-Beta pruning algorithm, which is a optimized version of minimax.
I didnt implement it myself because Sacha Gregoire started writting it on his computer, thus i dont have any git stats for it but i helped him implement it (and i was the one doing the research, after Hai An Mai gave me the idea that we should use that algo)

Week 3:
Was sick so did not come to TD, but still worked on it at home.
Implemented bitboard attack for bishop and queen.

Week 4:
Still sick, tbh i forgot what i did :(

Holidays:
Worked on Alpha-Beta prunning and the implementation of the algorithm in our case, using the eval() method to give us who has the advantage (will make eval better, for now only uses material)
Started working on Zobrist Hashing to efficiently see if we already had this board before (usefull to check 3 fold repition and in eval as the same position happens very often)

Week 5:
Worked some more on Zobrist hashing, changed the innitialization of move_t to include the piece it moves and capture (if its a capture move) as we need it to update the hash.

Week 6:
Dont remember exactly what we did, but we thought of how to make the code better. One thing we implemented was the time, as we realised that we could go with more depths with only a few moves where the search time was longer than the time limit, so by implementing the time and returning the search function slightly before the limit is over, we could get a higher depths for most move, without risking our code timing out.

Week 7:
Worked on a new move prunning (additional) where basically we hope that the first few moves we see are "good", and so for the moves after those, we do not look the full depths for all of them , we only look at a lower depths for all, and only look through the whole depth if the move is good. We hope (and verified) that the first moves we see are good enough for the average complexity to be better, making the search function faster.
Also helped An work on the game phases, to adjust the "piece positioning score" depending on when we are in a game