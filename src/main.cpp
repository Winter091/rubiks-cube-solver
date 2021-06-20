#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

#include "Cube.hpp"
#include "Solver.hpp"
#include "Timer.hpp"

bool check_cube(Cube const& c)
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

int main()
{
    std::srand(std::time(0));

    std::vector<int> X;
    std::vector<double> seq, par;

    for (int i = 0; i < 100; i++) {
        X.emplace_back(i + 1);
        std::cout << "============== " << i + 1 << " ===============\n";

        auto [seq_t, par_t] = measure_time();
        seq.push_back(seq_t);
        par.push_back(par_t);
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
    print_vec(out_file, seq, "seq_t");
    print_vec(out_file, par, "par_t");
}