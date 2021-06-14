#include "Solver.hpp"

#include "Timer.hpp"

#include <iostream>
#include <memory>
#include <atomic>
#include <thread>

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
        if (goal->moves_done.empty() || !Cube::move_is_unnecessary(move, goal->moves_done.back())) {
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
    for (int limit = 0;  ; limit++) {
        //std::cout << limit << ' ';
        //std::cout.flush();

        if (dls(c, goal, limit))
            return;
    }
}

std::atomic<bool> solution_found;
std::vector<int> solution;

bool dls_parallel(Cube& c, CubeGGoal* goal, std::vector<int>& moves, int8_t limit)
{
    if (solution_found)
        return true;
    
    if (limit == 0) {
        if (solution_found)
            return true;
        
        if (!goal->is_satisfied(c))
            return false;
        
        solution_found = true;
        solution = moves;
        return true;
    }

    for (int move : goal->allowed_moves) {
        if (moves.empty() || !Cube::move_is_unnecessary(move, moves.back())) {
            c.move_indexed(move);
            moves.push_back(move);

            if (dls_parallel(c, goal, moves, limit - 1))
                return true;
            
            moves.pop_back();
            c.unmove_indexed(move);
        }
    }

    return false;
}

void thread_work(std::vector<Cube> starting_cubes, 
    std::vector<std::vector<int>> starting_moves, CubeGGoal* goal)
{
    for (int depth = 0; ; depth++) {
        for (std::size_t i = 0; i < starting_cubes.size(); i++)
            if (dls_parallel(starting_cubes[i], goal, starting_moves[i], depth))
                return;
    }
}

void iddfs_parallel(Cube& c, CubeGGoal* goal)
{
    solution_found = false;
    solution.clear();

    if (goal->is_satisfied(c)) {
        return;
    }
    
    int num_threads = 6;
    int cubes_per_thread = goal->allowed_moves.size() / num_threads;

    std::vector<std::thread> threads;
    std::size_t curr_move_index = 0;
    for (int i = 0; i < num_threads - 1; i++) {

        std::vector<Cube> starting_cubes;
        std::vector<std::vector<int>> starting_moves;
        for (int j = 0; j < cubes_per_thread; j++) {
            Cube new_c = c;
            new_c.move_indexed(goal->allowed_moves[curr_move_index]);
            starting_cubes.push_back(new_c);
            starting_moves.push_back(std::vector<int>{});
            starting_moves.back().push_back(goal->allowed_moves[curr_move_index++]);
        }

        threads.emplace_back(thread_work, std::move(starting_cubes),
                            std::move(starting_moves), goal);
    }

    std::vector<Cube> starting_cubes;
    std::vector<std::vector<int>> starting_moves;
    while (curr_move_index < goal->allowed_moves.size()) {
        Cube new_c = c;
        new_c.move_indexed(goal->allowed_moves[curr_move_index]);
        starting_cubes.push_back(new_c);
        starting_moves.push_back(std::vector<int>{});
        starting_moves.back().push_back(goal->allowed_moves[curr_move_index++]);

        //std::cout << '\n' << new_c << '\n';
    }

    thread_work(std::move(starting_cubes), std::move(starting_moves), goal);

    for (auto& t: threads)
        t.join();
}

// Make a copy of the input cube
std::vector<int> find_solution_parallel(Cube c)
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
        //std::cout << "Solving G" << i << " -> G" << i + 1 << ": ";

        Timer t_inner;
        iddfs_parallel(c, goals[i]);
        //std::cout << t_inner << '\n';

        cube_solution.insert(cube_solution.end(), 
            solution.begin(), solution.end());

        for (int move : solution) {
            c.move_indexed(move);
            //std::cout << c.get_indexed_move_name(move) << ' ';
        }

        //std::cout << '\n';
    }

    //std::cout << "\nThe entire solution took " << t_outer << '\n';
    simplify_solution(cube_solution);
    return cube_solution;
}

// Make a copy of the input cube
std::vector<int> find_solution(Cube c)
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
        //std::cout << "Solving G" << i << " -> G" << i + 1 << ": ";

        Timer t_inner;
        iddfs(c, goals[i]);
        //std::cout << t_inner << '\n';

        cube_solution.insert(cube_solution.end(), 
            goals[i]->moves_done.begin(), goals[i]->moves_done.end());
    }

    //std::cout << "\nThe entire solution took " << t_outer << '\n';
    simplify_solution(cube_solution);
    return cube_solution;
}