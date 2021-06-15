#include "Solver.hpp"

#include "Timer.hpp"

#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <algorithm>

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

std::atomic<bool> static solution_found;
std::vector<int>  static curr_goal_solution;

bool static dls_sequental(Cube& c, CubeGGoal* goal, std::vector<int>& moves_done, int8_t limit)
{
    if (limit == 0)
        return goal->is_satisfied(c);

    for (int move : goal->allowed_moves) {
        if (moves_done.empty() || !Cube::move_is_unnecessary(move, moves_done.back())) {
            c.move_indexed(move);
            moves_done.push_back(move);

            if (dls_sequental(c, goal, moves_done, limit - 1))
                return true;
            
            moves_done.pop_back();
            c.unmove_indexed(move);
        }
    }

    return false;
}

void static iddfs_sequental(Cube& c, CubeGGoal* goal)
{
    std::vector<int> moves_done;
    
    for (int limit = 0; /* limit without a limit */ ; limit++) {
        if (dls_sequental(c, goal, moves_done, limit)) {
            curr_goal_solution = std::move(moves_done);
            return;
        }
    }
}

bool static dls_parallel(Cube& c, CubeGGoal* goal, std::vector<int>& moves_done, int8_t limit)
{
    // Return if another thread has found the solution
    if (solution_found)
        return true;
    
    if (limit == 0) {        
        if (!goal->is_satisfied(c))
            return false;

        // Return if another thread has found the solution
        // while this one was checking goal->is_satisfied()
        if (std::atomic_exchange(&solution_found, true))
            return true;
        
        curr_goal_solution = std::move(moves_done);
        return true;
    }

    for (int move : goal->allowed_moves) {
        if (moves_done.empty() || !Cube::move_is_unnecessary(move, moves_done.back())) {
            c.move_indexed(move);
            moves_done.push_back(move);

            if (dls_parallel(c, goal, moves_done, limit - 1))
                return true;
            
            moves_done.pop_back();
            c.unmove_indexed(move);
        }
    }

    return false;
}

// This IDDFS recieves a couple of "top" nodes of a tree,
// not just one, so it has to traverse all of them for each depth limit
void static iddfs_parallel_impl(std::vector<Cube> starting_cubes, 
    std::vector<std::vector<int>> starting_moves, CubeGGoal* goal)
{
    for (int limit = 0; /* limit without a limit */ ; limit++) {
        for (std::size_t i = 0; i < starting_cubes.size(); i++)
            if (dls_parallel(starting_cubes[i], goal, starting_moves[i], limit))
                return;
    }
}

void static iddfs_parallel(Cube& c, CubeGGoal* goal, unsigned num_threads)
{
    solution_found = false;
    curr_goal_solution.clear();

    if (goal->is_satisfied(c))
        return;

    std::vector<std::thread> threads;

    // Start IDDFS from depth = 1, so we have 
    // 18/14/10/6 (dependent on current goal) 
    // different cubes (nodes) to work with
    std::vector<Cube> starting_cubes;
    std::vector<std::vector<int>> starting_moves;

    double cubes_per_thread 
        = static_cast<double>(goal->allowed_moves.size()) / num_threads;
    
    std::size_t curr_move_index = 0;
    unsigned cubes_started_total = 0;
    double curr_sum = 0.0;

    // Start other threads
    for (unsigned i = 0; i < num_threads - 1; i++) {
        
        // Determine amount of nodes for this thread
        curr_sum += cubes_per_thread;
        unsigned this_thread_count = curr_sum - cubes_started_total;
        cubes_started_total += this_thread_count;

        // Don't create threads that don't 
        // have work to do. Brilliant!
        if (this_thread_count == 0)
            continue;

        for (unsigned j = 0; j < this_thread_count; j++, curr_move_index++) {
            Cube copy = c;
            copy.move_indexed(goal->allowed_moves[curr_move_index]);
            starting_cubes.push_back(copy);
            starting_moves.push_back(std::vector<int>{});
            starting_moves.back().push_back(goal->allowed_moves[curr_move_index]);
        }

        threads.emplace_back(iddfs_parallel_impl, std::move(starting_cubes),
                            std::move(starting_moves), goal);

        starting_cubes.clear();
        starting_moves.clear();
    }

    // Start IDDFS on this thread
    for (; cubes_started_total < goal->allowed_moves.size(); cubes_started_total++) {
        Cube new_c = c;
        new_c.move_indexed(goal->allowed_moves[curr_move_index]);
        starting_cubes.push_back(new_c);
        starting_moves.push_back(std::vector<int>{});
        starting_moves.back().push_back(goal->allowed_moves[curr_move_index++]);
    }

    iddfs_parallel_impl(std::move(starting_cubes), std::move(starting_moves), goal);

    for (int move : curr_goal_solution) 
        c.move_indexed(move);

    for (auto& t: threads)
        t.join();
}

std::vector<int> find_solution(Cube c, algo_type type)
{
    std::vector<int> cube_solution;
    std::vector<CubeGGoal*> static goals = {
        new CubeG0_G1Goal,
        new CubeG1_G2Goal,
        new CubeG2_G3Goal,
        new CubeG3_G4Goal
    };
    
    // More than 18 threads won't give a performance 
    // boost, because IDDFS with depth = 1 splits 
    // on 18 nodes at max
    unsigned int static num_threads 
        = std::clamp(std::thread::hardware_concurrency(), 1u, 18u);

    if (type == algo_type::decide_best) {
        if (num_threads < 2)
            type = algo_type::sequental;
        else 
            type = algo_type::parallel;
    }
    
    Timer timer;
    for (int i = 0; i < 4; i++) {
        std::cout << "Solving G" << i << " -> G" << i + 1 << "...";
        std::cout.flush();

        if (type == algo_type::sequental)
            iddfs_sequental(c, goals[i]);
        else
            iddfs_parallel(c, goals[i], num_threads);

        cube_solution.insert(cube_solution.end(), 
            curr_goal_solution.begin(), curr_goal_solution.end());

        std::cout << '\n';
    }
    std::cout << "\nThe entire solution took " << timer << '\n';

    simplify_solution(cube_solution);
    return cube_solution;
}