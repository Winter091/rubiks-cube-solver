#include "Cube.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>

Cube::Cube()
{
    for (int i = 0; i < 12; i++)
        edges[i] = { (uint8_t)i, 0 };

    for (int i = 0; i < 8; i++)
        corners[i] = { (uint8_t)i, 0 };
}

void Cube::print()
{
    const char* edge_lookup[12] = {
        "UF", "UL", "UB", "UR", 
        "FR", "FL", "BL", "BR", 
        "DF", "DL", "DB", "DR"
    };

    const char* corner_lookup[8] = {
        "UFR", "UFL", "UBL", "UBR", 
        "DFR", "DFL", "DBL", "DBR"
    };
    
    std::cout << "Edges:\n";
    for (int i = 0; i < 12; i++)
        std::cout << edge_lookup[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 12; i++)
        std::cout << edge_lookup[edges[i].index] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 12; i++)
        std::cout << (int)edges[i].orientation << "  ";

    std::cout << "\n\nCorners:\n";
    for (int i = 0; i < 8; i++)
        std::cout << corner_lookup[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 8; i++)
        std::cout << corner_lookup[corners[i].index] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 8; i++)
        std::cout << (int)corners[i].orientation << "   ";
    std::cout << '\n';
}

void Cube::move(const std::string& moves)
{
    std::istringstream iss(moves);
    
    while (true) {
        std::string move;
        iss >> move;

        if (!iss)
            break;

        if (move == "F")      F();
        else if (move == "B") B();
        else if (move == "L") L();
        else if (move == "R") R();
        else if (move == "D") D();
        else if (move == "U") U();

        else if (move == "F'") F_prime();
        else if (move == "B'") B_prime();
        else if (move == "L'") L_prime();
        else if (move == "R'") R_prime();
        else if (move == "D'") D_prime();
        else if (move == "U'") U_prime();

        else if (move == "F2") F2();
        else if (move == "B2") B2();
        else if (move == "L2") L2();
        else if (move == "R2") R2();
        else if (move == "D2") D2();
        else if (move == "U2") U2();

        else
            std::cerr << "Unrecognized move : " << move << '\n';
    }

}

void Cube::scramble(int num_rotations)
{
    const char* possible_moves[18] = {
        "F ", "B ", "L ", "R ", "D ", "U ",
        "F' ", "B' ", "L' ", "R' ", "D' ", "U' ",
        "F2 ", "B2 ", "L2 ", "R2 ", "D2 ", "U2 "
    };
    
    std::string curr_move;
    for (int i = 0; i < num_rotations; i++)
        curr_move.append(possible_moves[std::rand() % 18]);
    
    std::cout << "Scrambling cube with:\n" << curr_move << '\n';
    this->move(curr_move);
}


// Indices should be passed in opposite direction of the roration
void Cube::rotate_face_90(int e1, int e2, int e3, int e4, int c1, int c2, int c3, int c4)
{
    Cubie temp = edges[e1];
    edges[e1] = edges[e2];
    edges[e2] = edges[e3];
    edges[e3] = edges[e4];
    edges[e4] = temp;

    temp = corners[c1];
    corners[c1] = corners[c2];
    corners[c2] = corners[c3];
    corners[c3] = corners[c4];
    corners[c4] = temp;
}

void Cube::rotate_face_180(int e1, int e2, int e3, int e4, int c1, int c2, int c3, int c4)
{
    std::swap(edges[e1], edges[e3]);
    std::swap(edges[e2], edges[e4]);

    std::swap(corners[c1], corners[c3]);
    std::swap(corners[c2], corners[c4]);
}

void Cube::update_edge_orientation(int index) 
{
    edges[index].orientation ^= 1;
}

void Cube::update_corner_orientation(int index, int step) 
{
    corners[index].orientation += step;

    //corners[index].orientation %= 3;
    if (corners[index].orientation > 2)
        corners[index].orientation -= 3;
}

void Cube::U()
{
    rotate_face_90(3, 2, 1, 0, 3, 2, 1, 0);
}

void Cube::D()
{
    rotate_face_90(8, 9, 10, 11, 4, 5, 6, 7);
}

void Cube::L() 
{
    rotate_face_90(1, 6, 9, 5, 1, 2, 6, 5);
    update_corner_orientation(6, 1);
    update_corner_orientation(5, 2);
    update_corner_orientation(1, 1);
    update_corner_orientation(2, 2);
}

void Cube::R() 
{
    rotate_face_90(3, 4, 11, 7, 0, 4, 7, 3);
    update_corner_orientation(7, 2);
    update_corner_orientation(4, 1);
    update_corner_orientation(0, 2);
    update_corner_orientation(3, 1);
}

void Cube::F() 
{
    rotate_face_90(0, 5, 8, 4, 1, 5, 4, 0);

    update_corner_orientation(1, 2);
    update_corner_orientation(0, 1);
    update_corner_orientation(4, 2);
    update_corner_orientation(5, 1);

    update_edge_orientation(0);
    update_edge_orientation(5);
    update_edge_orientation(8);
    update_edge_orientation(4);
}

void Cube::B() 
{
    rotate_face_90(2, 7, 10, 6, 3, 7, 6, 2);

    update_corner_orientation(2, 1);
    update_corner_orientation(3, 2);
    update_corner_orientation(7, 1);
    update_corner_orientation(6, 2);

    update_edge_orientation(2);
    update_edge_orientation(7);
    update_edge_orientation(10);
    update_edge_orientation(6);
}

void Cube::U_prime() 
{
    rotate_face_90(0, 1, 2, 3, 0, 1, 2, 3);
}

void Cube::D_prime() 
{
    rotate_face_90(11, 10, 9, 8, 7, 6, 5, 4);
}

void Cube::L_prime() 
{
    rotate_face_90(5, 9, 6, 1, 5, 6, 2, 1);
    update_corner_orientation(6, 1);
    update_corner_orientation(5, 2);
    update_corner_orientation(1, 1);
    update_corner_orientation(2, 2);
}

void Cube::R_prime() 
{
    rotate_face_90(7, 11, 4, 3, 3, 7, 4, 0);
    update_corner_orientation(7, 2);
    update_corner_orientation(4, 1);
    update_corner_orientation(0, 2);
    update_corner_orientation(3, 1);
}

void Cube::F_prime() 
{
    rotate_face_90(4, 8, 5, 0, 0, 4, 5, 1);

    update_corner_orientation(1, 2);
    update_corner_orientation(0, 1);
    update_corner_orientation(4, 2);
    update_corner_orientation(5, 1);

    update_edge_orientation(0);
    update_edge_orientation(5);
    update_edge_orientation(8);
    update_edge_orientation(4);
}

void Cube::B_prime() 
{
    rotate_face_90(6, 10, 7, 2, 2, 6, 7, 3);

    update_corner_orientation(2, 1);
    update_corner_orientation(3, 2);
    update_corner_orientation(7, 1);
    update_corner_orientation(6, 2);

    update_edge_orientation(2);
    update_edge_orientation(7);
    update_edge_orientation(10);
    update_edge_orientation(6);
}

void Cube::U2() 
{
    rotate_face_180(3, 2, 1, 0, 3, 2, 1, 0);
}

void Cube::D2() 
{
    rotate_face_180(8, 9, 10, 11, 4, 5, 6, 7);
}

void Cube::L2() 
{
    rotate_face_180(1, 6, 9, 5, 1, 2, 6, 5);
}

void Cube::R2() 
{
    rotate_face_180(3, 4, 11, 7, 0, 4, 7, 3);
}

void Cube::F2() 
{
    rotate_face_180(0, 5, 8, 4, 1, 5, 4, 0);
}

void Cube::B2() 
{
    rotate_face_180(2, 7, 10, 6, 3, 7, 6, 2);
}

