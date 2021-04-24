#include "Solver.hpp"

#include "Timer.hpp"

#include <iostream>
#include <memory>

bool move_is_unnecessary(int move, int prev_move) {
    for (int i = 0; i < 6; i++) {
        if ((move == i || move == 6 + i || move == 12 + i)
        && (prev_move == i || prev_move == 6 + i || prev_move == 12 + i))
            return true;
    }

    auto helper = [&](int i, int j) {
        if ((move == i || move == 6 + i || move == 12 + i)
         && (prev_move == j || prev_move == j + 6 || prev_move == j + 12))
            return true;
        return false;
    };

    return helper(2, 3) || helper(0, 1) || helper(4, 5);
}

void simplify_solution(std::vector<int>& s)
{
    for (int n = 0; n < 50; n++) {
        for (std::size_t i = 1; i < s.size(); i++) {
        int m1 = s[i - 1];
        int m2 = s[i];
        
        for (int j = 0; j < 6; j++) {
            // X + X2 -> X'
            if (((m1 == j && m2 == j + 12) || (m1 == j + 12 && m2 == j))) {
                s.erase(s.begin() + i);
                s[i - 1] = j + 6;
            }
            
            // X + X' = nothing
            else if (((m1 == j && m2 == j + 6) || (m1 == j + 6 && m2 == j))) {
                s.erase(s.begin() + i - 1, s.begin() + i + 1);
            }

            // X2 + X2 = nothing
            else if ((m1 == m2) && (m1 == j + 12)) {
                s.erase(s.begin() + i - 1, s.begin() + i + 1);
            }
        }
    }
    }
    
}

bool dls(Cube& c, CubeGGoal* goal, int8_t limit)
{
    if (limit == 0) {
        if (goal->is_satisfied(c))
            return true;
        return false;
    }

    for (int move : goal->allowed_moves_to_reach) {
        if (goal->moves_done.empty() || !move_is_unnecessary(move, goal->moves_done.back())) {
            c.move_indexed(move);
            goal->moves_done.push_back(move);

            if (dls(c, goal, limit - 1))
                return true;
            
            goal->moves_done.pop_back();
            c.unmove_indexed(move);
        }
    }

    return false;
}

bool iddfs(Cube& c, CubeGGoal* goal)
{
    for (int limit = 0; /* limit without a limit */ ; limit++) {
        std::cout << limit + 1 << ' ';
        std::cout.flush();

        if (dls(c, goal, limit))
            return true;
    }

    return false;
}

// Take by value, so make a copy
void find_solution(Cube c)
{
    std::vector<int> cube_solution;
    std::vector<CubeGGoal*> goals = {
        new CubeG0_G1Goal{},
        new CubeG1_G2Goal{},
        new CubeG2_G3Goal{},
        new CubeG3_G4Goal{}
    };
    
    Timer t_outer;
    for (int i = 0; i < 4; i++) {
        std::cout << "Solving G" << i << " -> G" << i + 1 << ": ";
        Timer t_inner;
        iddfs(c, goals[i]);
        std::cout << t_inner << '\n';
        cube_solution.insert(cube_solution.end(), 
            goals[i]->moves_done.begin(), goals[i]->moves_done.end());
    }
    std::cout << "\nThe entire solution took " << t_outer << '\n';
    std::cout << "The found solution has length " 
        << cube_solution.size() << ":\n";

    for (int move : cube_solution)
        std::cout << c.get_indexed_move_name(move) << ' ';
    std::cout << '\n';

    std::size_t old_len = cube_solution.size();
    simplify_solution(cube_solution);
    if (old_len != cube_solution.size()) {
        std::cout << "There's a simplified version with length " 
            << cube_solution.size() << ":\n";
        for (int move : cube_solution)
            std::cout << c.get_indexed_move_name(move) << ' ';
        std::cout << '\n';
    }
}