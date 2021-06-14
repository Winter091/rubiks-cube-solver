#ifndef SOLVER_H_
#define SOLVER_H_

#include "Cube.hpp"
#include <vector>
#include <iostream>

class CubeGGoal
{
public:
    std::vector<int> moves_done;
    std::vector<int> allowed_moves;
    virtual bool is_satisfied(const Cube& c) = 0;
};

class CubeG0_G1Goal : public CubeGGoal
{
public:
    CubeG0_G1Goal()
    {
        // All turns
        for (int i = 0; i < 18; i++)
            allowed_moves.push_back(i);
    }

    bool is_satisfied(const Cube& c) override
    {
        // All edges have to be oriented correctly
        for (int i = 0; i < 12; i++)
            if (c.get_edge_orientation(i) != 0)
                return false;
    
        return true;
    }
};

class CubeG1_G2Goal : public CubeGGoal
{
public:
    CubeG1_G2Goal()
    {
        // everything but F, F', B, B'
        allowed_moves = {
            0, 1, 4, 5,
            6, 7, 10, 11,
            12, 13, 14, 15, 16, 17
        };
    }

    bool is_satisfied(const Cube& c) override
    {
        // All corners have to be oriented correctly
        for (int i = 0; i < 8; i++)
            if (c.get_corner_orientation(i) != 0)
                return false;
        
        // Edges 4, 5, 6, 7 should be on positions 4, 5, 6, 7
        int lookup[] = {4, 5, 6, 7};
        for (int i = 0; i < 4; i++) {
            int index = c.get_edge_index(lookup[i]);

            if (index != 4 && index != 5 && index != 6 && index != 7)
                return false;
        }

        return true;
    }
};

class CubeG2_G3Goal : public CubeGGoal
{
public:
    CubeG2_G3Goal()
    {
        // All *2 and U, U', D, D'
        allowed_moves = {
            4, 5, 10, 11,
            12, 13, 14, 15, 16, 17,
        };
    }

    bool is_satisfied(const Cube& c) override
    {
        // Pairs {UFR, UBL}, {UFL, UBR}, {DFR, DBL}, {DFL, DBR}
        // should be achieved
        uint8_t c_ind;

        c_ind = c.get_corner_index(0);
        if (c_ind != 0 && c_ind != 2)
            return false;
        c_ind = c.get_corner_index(2);
        if (c_ind != 0 && c_ind != 2)
            return false;
        
        c_ind = c.get_corner_index(1);
        if (c_ind != 1 && c_ind != 3)
            return false;
        c_ind = c.get_corner_index(3);
        if (c_ind != 1 && c_ind != 3)
            return false;

        c_ind = c.get_corner_index(4);
        if (c_ind != 4 && c_ind != 6)
            return false;
        c_ind = c.get_corner_index(6);
        if (c_ind != 4 && c_ind != 6)
            return false;

        // Edges 0, 2, 8, 10 should be on positions 0, 2, 8, 10
        int lookup[] = {0, 2, 8, 10};
        for (int i = 0; i < 4; i++) {
            int index = c.get_edge_index(lookup[i]);

            if (index != 0 && index != 2 && index != 8 && index != 10)
                return false;
        }

        //Parity must be even
        uint8_t parity = 0;
        uint8_t order[] = {2, 3, 0, 1, 5, 6, 7, 4};
        uint8_t order_reverse[] = {2, 3, 0, 1, 7, 4, 5, 6};

        for (int i = 0; i < 8; i++)
            for (int j = i + 1; j < 8; j++)
                parity ^= (order_reverse[c.get_corner_index(order[i])]
                            < order_reverse[c.get_corner_index(order[j])]);
    
        return parity == 0;
    }
};

class CubeG3_G4Goal : public CubeGGoal
{
public:
    CubeG3_G4Goal()
    {
        // Only *2 moves
        allowed_moves = {
            12, 13, 14, 15, 16, 17
        };
    }

    bool is_satisfied(const Cube& c) override
    {
        for (int i = 0; i < 12 - 2; i++)
            if (c.get_edge_index(i) != i)
                return false;

        for (int i = 0; i < 8 - 3; i++)
            if (c.get_corner_index(i) != i)
                return false;

        return true;
    }
};

std::vector<int> find_solution(Cube c);
std::vector<int> find_solution_parallel(Cube c);

#endif