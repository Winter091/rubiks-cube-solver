#include "Cube.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string_view>
#include <vector>

Cube::Cube()
{
    for (int i = 0; i < 12; i++)
        edges[i] = { (uint8_t)i, 0 };

    for (int i = 0; i < 8; i++)
        corners[i] = { (uint8_t)i, 0 };
}

Cube::Cube(const char* filepath)
{
    set_from_file(filepath);
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
            std::cerr << "Unrecognized move: " << move << '\n';
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
        // Make sure there's no unnecessary moves
        int move;
        while (true) {
            move = std::rand() % 18;
            if (!Cube::move_is_unnecessary(move, prev))
                break;
        }
        prev = move;
        
        curr_move.append(possible_moves[move]);
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

void Cube::check_colors_array(char colors[54])
{
    // Check that red is in front, white is on top
    if (colors[4] != 'W' || colors[25] != 'R') {
        std::cerr << "Red should be in front, white on top!\n";
        throw std::runtime_error{"Cube::check_colors_array: bad cube orientation"};
    }
    
    auto map_to_index = [](char color) -> int {
        switch (color) {
            case 'W': return 0;
            case 'G': return 1;
            case 'R': return 2;
            case 'B': return 3;
            case 'O': return 4;
            case 'Y': return 5;
            default:  return 6;
        }
    };

    char map_to_color[] = { 'W', 'G', 'R', 'B', 'O', 'Y' };
    
    // 0 1 2 3 4 5   6
    // W G R B O Y Other
    int color_counts[7] = {0};

    // Check that there's exactly 9 tiles of each color
    for (int i = 0; i < 54; i++)
        color_counts[map_to_index(colors[i])]++;

    for (int i = 0; i < 6; i++) {
        if (color_counts[i] != 9) {
            std::cerr << "Error in data: there's " << color_counts[i] 
                << " '" << map_to_color[i] << "' tiles instead of 9!\n";
            throw std::runtime_error("Cube::check_colors_array: bad number of colors\n");
        }
    }
}

void Cube::set_from_colors(char colors[54])
{
    check_colors_array(colors);
    
    int edge_indices[12][2] = {
        {  7, 13 },   // UF 0
        {  3, 10 },   // UL 1
        {  1, 19 },   // UB 2 
        {  5, 16 },   // UR 3
        { 26, 27 },   // FR 4
        { 24, 23 },   // FL 5
        { 32, 21 },   // BL 6
        { 30, 29 },   // BR 7
        { 46, 37 },   // DF 8
        { 48, 34 },   // DL 9
        { 52, 43 },   // DB 10
        { 50, 40 }    // DR 11
    };

    char edge_colors[12][2] = {
        { 'W', 'R' },   // UF 0
        { 'W', 'G' },   // UL 1
        { 'W', 'O' },   // UB 2 
        { 'W', 'B' },   // UR 3
        { 'B', 'R' },   // FR 4
        { 'G', 'R' },   // FL 5
        { 'G', 'O' },   // BL 6
        { 'O', 'B' },   // BR 7
        { 'Y', 'R' },   // DF 8
        { 'Y', 'G' },   // DL 9
        { 'Y', 'O' },   // DB 10
        { 'Y', 'B' }    // DR 11
    };

    // c1 is up/down for up/down layers and front/back for middle layer
    auto get_edge_orientation = [](char c1, char c2) -> uint8_t {
        if (c1 == 'G' || c1 == 'B')
            return 1;
        
        if (c1 == 'R' || c1 == 'O')
            if (c2 == 'W' || c2 == 'Y')
                return 1;

        return 0;
    };

    for (int i = 0; i < 12; i++) {
        char c1 = colors[edge_indices[i][0]];
        char c2 = colors[edge_indices[i][1]];

        for (int j = 0; j < 12; j++) {
            if ((c1 == edge_colors[j][0] && c2 == edge_colors[j][1])
             || (c1 == edge_colors[j][1] && c2 == edge_colors[j][0])) {
                 edges[i] = { (uint8_t)j, get_edge_orientation(c1, c2) };
                 break;
            }
        }
    }

    int corner_indices[8][3] = {
        {  8, 15, 14 },     // UFR 0
        {  6, 11, 12 },     // UFL 1
        {  0,  9, 20 },     // UBL 2
        {  2, 17, 18 },     // UBR 3
        { 47, 39, 38 },     // DFR 4
        { 45, 35, 36 },     // DFL 5
        { 51, 33, 44 },     // DBL 6
        { 53, 41, 42 }      // DBR 7
    };

    char corner_colors[8][3] = {
        { 'W', 'R', 'B' },     // UFR 0
        { 'W', 'R', 'G' },     // UFL 1
        { 'W', 'G', 'O' },     // UBL 2
        { 'W', 'O', 'B' },     // UBR 3
        { 'Y', 'R', 'B' },     // DFR 4
        { 'Y', 'R', 'G' },     // DFL 5
        { 'Y', 'G', 'O' },     // DBL 6
        { 'Y', 'O', 'B' }      // DBR 7
    };

    auto get_corner_orientation = [](int index, char c1, char c2, char c3) -> uint8_t {
        if (c1 == 'W' || c1 == 'Y')
            return 0;
        
        switch (index) {
            case 2: 
            case 0:
            case 7:
            case 5:
                return (c2 == 'W' || c2 == 'Y') ? 1 : 2;
            
            case 1:
            case 3:
            case 4:
            case 6:
                return (c3 == 'W' || c3 == 'Y') ? 1 : 2;
        }

        return 255;
    };

    auto get_corner_index = [corner_colors](char c1, char c2, char c3) -> uint8_t {
        std::vector<char> perms = { c1, c2, c3 };

        std::sort(perms.begin(), perms.end());
        
        do {
            for (int i = 0; i < 8; i++) {
                if (perms[0] == corner_colors[i][0] && perms[1] == corner_colors[i][1] 
                 && perms[2] == corner_colors[i][2])
                    return i;
            }
        } while (std::next_permutation(perms.begin(), perms.end()));

        return 255;
    };

    for (int i = 0; i < 8; i++) {
        char c1 = colors[corner_indices[i][0]];
        char c2 = colors[corner_indices[i][1]];
        char c3 = colors[corner_indices[i][2]];

        uint8_t index = get_corner_index(c1, c2, c3);
        uint8_t orientation = get_corner_orientation(i, c1, c2, c3);

        corners[i] = { index, orientation };
    }
}

void Cube::set_from_file(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "File " << filepath << " was not found.\n";
        throw std::runtime_error("Cube::Cube: file was not found\n");
    }

    char colors[54];
    for (int index = 0; index < 54; index++) {
        file >> colors[index];
        if (!file) {
            std::cerr << "File \"" << filepath << "\" has less than 54 colors "
                << "(only " << index << "), aborting\n";
            throw std::runtime_error("Cube::Cube: bad number of colors\n");
        }
    }

    file.close();
    set_from_colors(colors);
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

bool Cube::move_is_unnecessary(int move, int prev_move) {
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