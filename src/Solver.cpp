#include "Solver.hpp"

#include "Timer.hpp"

#include <iostream>
#include <memory>

bool move_is_unnecessary(int move, int prev_move) {
    // If first letter of the move is the same
    for (int i = 0; i < 6; i++) {
        if ((move == i || move == 6 + i || move == 12 + i)
        && (prev_move == i || prev_move == 6 + i || prev_move == 12 + i))
            return true;
    }

    // L R and simular things
    for (int i = 0; i <= 4; i += 2) {
        if ((move == i || move == 6 + i || move == 12 + i)
         && (prev_move == i + 1 || prev_move == 6 + i + 1 || prev_move == 12 + i + 1))
            return true;
    }

    return false;
}

void simplify_solution(std::vector<int>& s)
{
    while (true) {
        bool something_changed = false;

        for (std::size_t i = 1; i < s.size(); i++) {
            int m1 = s[i - 1];
            int m2 = s[i];

            for (int j = 0; j < 6; j++) {
                // X + X2 -> X'
                if (((m1 == j && m2 == j + 12) || (m1 == j + 12 && m2 == j))) {
                    s.erase(s.begin() + i);
                    s[i - 1] = j + 6;
                    something_changed = true;
                }

                // X' + X2 -> X
                else if (((m1 == j + 6 && m2 == j + 12) || (m1 == j + 12 && m2 == j + 6))) {
                    s.erase(s.begin() + i);
                    s[i - 1] = j;
                    something_changed = true;
                }
                
                // X + X' = nothing
                else if (((m1 == j && m2 == j + 6) || (m1 == j + 6 && m2 == j))) {
                    s.erase(s.begin() + i - 1, s.begin() + i + 1);
                    something_changed = true;
                }

                // X2 + X2 = nothing
                else if ((m1 == m2) && (m1 == j + 12)) {
                    s.erase(s.begin() + i - 1, s.begin() + i + 1);
                    something_changed = true;
                }
            }

        }

        if (!something_changed)
            return;
    }
}

bool dls(Cube& c, CubeGGoal* goal, int8_t limit)
{
    if (limit == 0)
        return goal->is_satisfied(c);

    for (int move : goal->allowed_moves) {
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

void iddfs(Cube& c, CubeGGoal* goal)
{
    for (int limit = 0; /* limit without a limit */ ; limit++) {
        std::cout << limit << ' ';
        std::cout.flush();

        if (dls(c, goal, limit))
            return;
    }
}

// Make a copy of the input cube
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