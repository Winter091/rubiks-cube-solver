#include "Cube.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <cassert>
#include <string_view>

Cube::Cube()
{
    for (int i = 0; i < 12; i++)
        edges[i] = { (uint8_t)i, 0 };

    for (int i = 0; i < 8; i++)
        corners[i] = { (uint8_t)i, 0 };
}

std::ostream& operator<<(std::ostream& os, const Cube& c)
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
    
    os << "Edges:\n";
    for (int i = 0; i < 12; i++)
        os << edge_lookup[i] << ' ';
    os << '\n';
    for (int i = 0; i < 12; i++)
        os << edge_lookup[c.edges[i].index] << ' ';
    os << '\n';
    for (int i = 0; i < 12; i++)
        os << (int)c.edges[i].orientation << "  ";

    os << "\n\nCorners:\n";
    for (int i = 0; i < 8; i++)
        os << corner_lookup[i] << ' ';
    os << '\n';
    for (int i = 0; i < 8; i++)
        os << corner_lookup[c.corners[i].index] << ' ';
    os << '\n';
    for (int i = 0; i < 8; i++)
        os << (int)c.corners[i].orientation << "   ";
    
    os << "\n\n\n";
    return os;
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

void Cube::scramble(int num_moves)
{
    const char* possible_moves[18] = {
        "F ", "B ", "L ", "R ", "D ", "U ",
        "F' ", "B' ", "L' ", "R' ", "D' ", "U' ",
        "F2 ", "B2 ", "L2 ", "R2 ", "D2 ", "U2 "
    };
    
    std::string curr_move;
    int prev = -1;
    for (int i = 0; i < num_moves; i++) {
        // Make sure there's at least no repeated moves
        int rand;
        while ((rand = std::rand() % 18) == prev);
        prev = rand;
        
        curr_move.append(possible_moves[rand]);
    }
    
    std::cout << "Scrambling cube using " << num_moves << " moves:\n" << curr_move << "\n\n";
    this->move(curr_move);
}

void Cube::restore()
{
    for (int i = 0; i < 12; i++)
        edges[i] = { (uint8_t)i, 0 };

    for (int i = 0; i < 8; i++)
        corners[i] = { (uint8_t)i, 0 };
}

int Cube::get_corner_orientation(int c_index) const
{
    assert(c_index >= 0 && c_index <= 7);
    return corners[c_index].orientation;
}

int Cube::get_edge_orientation(int e_index) const
{
    assert(e_index >= 0 && e_index <= 11);
    return edges[e_index].orientation;
}

int Cube::get_corner_index(int c_index) const  
{
    assert(c_index >= 0 && c_index <= 7);
    return corners[c_index].index;
}

int Cube::get_edge_index(int e_index) const 
{
    assert(e_index >= 0 && e_index <= 11);
    return edges[e_index].index;
}

void Cube::move_indexed(int index)
{
    assert(index >= 0 && index <= 17);
    
    switch (index) {
        case 0: L(); break;
        case 1: R(); break;
        case 2: F(); break;
        case 3: B(); break;
        case 4: U(); break;
        case 5: D(); break;

        case 6:  L_prime(); break;
        case 7:  R_prime(); break;
        case 8:  F_prime(); break;
        case 9:  B_prime(); break;
        case 10: U_prime(); break;
        case 11: D_prime(); break;

        case 12: L2(); break;
        case 13: R2(); break;
        case 14: F2(); break;
        case 15: B2(); break;
        case 16: U2(); break;
        case 17: D2(); break;
    }
}

void Cube::unmove_indexed(int index)
{
    assert(index >= 0 && index <= 17);
    
    switch (index) {
        case 0: L_prime(); break;
        case 1: R_prime(); break;
        case 2: F_prime(); break;
        case 3: B_prime(); break;
        case 4: U_prime(); break;
        case 5: D_prime(); break;

        case 6:  L(); break;
        case 7:  R(); break;
        case 8:  F(); break;
        case 9:  B(); break;
        case 10: U(); break;
        case 11: D(); break;

        case 12: L2(); break;
        case 13: R2(); break;
        case 14: F2(); break;
        case 15: B2(); break;
        case 16: U2(); break;
        case 17: D2(); break;
    };
}

const char* Cube::get_indexed_move_name(int index)
{
    assert(index >= 0 && index <= 17);
    
    switch (index) {
        case 0: return "L";
        case 1: return "R";
        case 2: return "F";
        case 3: return "B";
        case 4: return "U";
        case 5: return "D";

        case 6:  return "L'";
        case 7:  return "R'";
        case 8:  return "F'";
        case 9:  return "B'";
        case 10: return "U'";
        case 11: return "D'";

        case 12: return "L2";
        case 13: return "R2";
        case 14: return "F2";
        case 15: return "B2";
        case 16: return "U2";
        case 17: return "D2";
    };

    return "How did we get here?";
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