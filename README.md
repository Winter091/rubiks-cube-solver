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

I haven't yet done the extensive testing, but it seems that the performance
boost when using parallel version is something like (0.72 * thread_count) 
times, compared to non-parallel version. Specifically, on 6-core ryzen 5
4500u, it's 4.34x faster, thus making this implementation very fast
compared to others (3-4 seconds avg to find the solution).
