#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

#include "Cube.hpp"
#include "Solver.hpp"
#include "Timer.hpp"

bool check_cube(Cube const &c)
{
    for (int i = 0; i < 12; i++)
    {
        if (c.get_edge_index(i) != i || c.get_edge_orientation(i) != 0)
            return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (c.get_corner_index(i) != i || c.get_corner_orientation(i) != 0)
            return false;
    }

    return true;
}

std::pair<double, double> measure_time()
{
    Cube c;
    c.scramble(100);

    Cube c1 = c;
    Cube c2 = c;

    double time1;
    std::cout << "\nSequental:\n";
    {
        Timer t;
        std::vector<int> solution = find_solution(c1, algo_type::sequental);
        time1 = t.get_time() / 1000.0;

        for (int move : solution)
            c1.move_indexed(move);

        if (!check_cube(c1))
        {
            std::cout << "BAD! NON_PARALLEL!\n";
            std::exit(0);
        }
    }
    std::cout <<'\n';

    double time2;
    std::cout << "\nParallel:\n";
    {
        Timer t;
        std::vector<int> solution = find_solution(c2, algo_type::parallel);
        time2 = t.get_time() / 1000.0;

        for (int move : solution)
            c2.move_indexed(move);

        if (!check_cube(c2))
        {
            std::cout << "BAD! PARALLEL!\n";
            std::exit(0);
        }
    }
    std::cout << '\n';

    return std::make_pair(time1, time2);
}

int main(int argc, const char *argv[])
{
    std::srand(std::time(0));

    Cube c;
    
    if (argc < 2) {
        std::cout << "Solving random scramble...\n";
        c.scramble(25);
    } else {
        std::cout << "Solving cube from file...\n";
        c.set_from_file(argv[1]);
    }

    std::vector<int> solution = find_solution(c, algo_type::decide_best);

    std::cout << "The found solution has length " << solution.size() << ":\n";
    for (int move : solution)
        std::cout << Cube::get_indexed_move_name(move) << ' ';
    std::cout << '\n';
}