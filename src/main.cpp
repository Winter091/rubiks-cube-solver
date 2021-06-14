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
    {
        Timer t;
        std::vector<int> solution = find_solution(c1);
        time1 = t.get_time() / 1000.0;

        for (int move : solution)
            c1.move_indexed(move);

        if (!check_cube(c1))
        {
            std::cout << "BAD! NON_PARALLEL!\n";
            std::exit(0);
        }
    }

    double time2;
    {
        Timer t;
        std::vector<int> solution = find_solution_parallel(c2);
        time2 = t.get_time() / 1000.0;

        for (int move : solution)
            c2.move_indexed(move);

        if (!check_cube(c2))
        {
            std::cout << "BAD! PARALLEL!\n";
            std::exit(0);
        }
    }

    return std::make_pair(time1, time2);
}

int main(int argc, const char *argv[])
{
    std::srand(std::time(0));

    std::size_t const count = 25;

    std::vector<std::size_t> X;
    X.reserve(count);

    std::vector<double> seq_times, par_times;
    seq_times.reserve(X.size());
    par_times.reserve(X.size());
    
    for (std::size_t i = 0; i < count; i++) {

        auto [seq, par] = measure_time();
        std::cout << i << '\n';

        X.push_back(i);
        seq_times.push_back(seq);
        par_times.push_back(par);
    }

    auto print_vec = [](std::ostream& os, auto const& vec, const char* name) {
        os << name << " = [";
        for (auto elem : vec) {
            os << elem << ", ";
        }
        os << "]\n";
    };

    std::ofstream out_file{"data.txt"};
    print_vec(out_file, X, "X");
    print_vec(out_file, seq_times, "seq_times");
    print_vec(out_file, par_times, "par_times");

    return 0;

    // Scramble where G2 -> G3 takes very long (716 sec on i5-3470)
    /*
    c.move("R' L2 B L' B R F' R U' L2 L D2 B R' B2 U2 D' "
    " L' U' R' D2 R L D2 U' R2 U' U2 R2 D2 F' R' R2 "
    " B2 D' D2 U2 L2 L D' U' D F' F2 U' L R' B L' B "
    " L' D U' F R2 F' U' B2 B B' U2 D' D F2 F L U' "
    " R2 L B L' D F' F L2 L' R2 L2 R' F' F B2 R2 F "
    " L' B R' B L L2 U2 D' F R' D' F B U B L");
    */
}