# 3x3 Rubik's cube solver

This solver uses slightly modified Thistlethwaite's algorithm in order 
to find the solution. Basically, it's just a depth-first search done
in a clever way: there are 5 different "groups", you start from G0
(which is any cube) and try to improve that group by running DFS.
When the group is improved, certain moves are no longer needed and the 
branching factor of the next DFS is lowered. G4 is the solved cube.

This algorithm uses iterative deeping depth-first search (IDDFS), because
it combines the behaviour of breadth-first search (so we find the shortest
solution) and the low memory consumption (compared to BFS) of depth-first 
search.

The parallel version of the IDDFS firstly branches one time from the top 
node and then evenly splits resulting nodes between threads, so each 
thread performs IDDFS on the some part of the tree. If one thread has 
found the answer, all remaining stop their work.

What about performance? Well, it certainly could be better... It looks 
like IDDFS doesn't do a good job here - there are too many 
branches to consider even to just quickly solve G2 -> G3. 
Multithreading helps a lot, but still - you may and will encounter 
scrambles that will take 1 min, 10 min, even hours in the worst of the 
worst case. But *usually* it's in the order of seconds. 

Bidirectional BFS will perform much better here - it will solve everything
almost instantly (< 1 sec).
