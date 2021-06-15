#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

#include "Cube.hpp"
#include "Solver.hpp"
#include "Timer.hpp"

int main(int argc, const char *argv[])
{
    std::srand(std::time(0));

    Cube c;
    
    if (argc < 2) {
        std::cout << "Solving random scramble...\n";
        c.scramble(100);
    } else {
        std::cout << "Solving cube from file...\n";
        c.set_from_file(argv[1]);
    }

    // Run parallel version of the algorithm
    std::vector<int> solution = find_solution(c, algo_type::parallel);

    std::cout << "The found solution has length " << solution.size() << ":\n";
    for (int move : solution)
        std::cout << Cube::get_indexed_move_name(move) << ' ';
    std::cout << '\n';
}