#include "Cube.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>

Cube::Cube()
{
    *(uint64_t*)(data + 0)  = 0x00'00'00'00'00'00'00'00ULL;
    *(uint64_t*)(data + 8)  = 0x01'01'01'01'01'01'01'01ULL;
    *(uint64_t*)(data + 16) = 0x02'02'02'02'02'02'02'02ULL;
    *(uint64_t*)(data + 24) = 0x03'03'03'03'03'03'03'03ULL;
    *(uint64_t*)(data + 32) = 0x04'04'04'04'04'04'04'04ULL;
    *(uint64_t*)(data + 40) = 0x05'05'05'05'05'05'05'05ULL;
}

void Cube::roll90(Face f)
{
    __asm__ __volatile__ (
        "rolq $16, %[face]\n"

        : [face] "+r" (*(uint64_t*)(data + (unsigned)f * 8))
        :
        :
    );
}

void Cube::roll180(Face f)
{
    __asm__ __volatile__ (
        "rolq $32, %[face]\n"

        : [face] "+r" (*(uint64_t*)(data + (unsigned)f * 8))
        :
        :
    );
}

void Cube::roll270(Face f)
{
    __asm__ __volatile__ (
        "rolq $48, %[face]\n"

        : [face] "+r" (*(uint64_t*)(data + (unsigned)f * 8))
        :
        :
    );
}

void Cube::rotate_sides_90(int i1, int i2, int i3, int i4, int c1, int c2, int c3, int c4)
{
    uint16_t temp16 = *(uint16_t*)(data + i1);

    *(uint16_t*)(data + i1) = *(uint16_t*)(data + i2);
    *(uint16_t*)(data + i2) = *(uint16_t*)(data + i3);
    *(uint16_t*)(data + i3) = *(uint16_t*)(data + i4);
    *(uint16_t*)(data + i4) = temp16;

    Cube::Color temp8 = data[c1];
    data[c1] = data[c2];
    data[c2] = data[c3];
    data[c3] = data[c4];
    data[c4] = temp8;
}

void Cube::rotate_sides_180(int i1, int i2, int i3, int i4, int c1, int c2, int c3, int c4)
{
    std::swap(*((uint16_t*)(data + i1)), *((uint16_t*)(data + i2)));
    std::swap(*((uint16_t*)(data + i3)), *((uint16_t*)(data + i4)));
    std::swap(*((uint8_t*)(data + c1)), *((uint8_t*)(data + c2)));
    std::swap(*((uint8_t*)(data + c3)), *((uint8_t*)(data + c4)));
}

void Cube::F()
{
    roll90(Face::FRONT);
    rotate_sides_90(4, 10, 40, 30, 6, 12, 42, 24);
}

void Cube::B() 
{
    roll90(Face::BACK);
    rotate_sides_90(0, 26, 44, 14, 2, 28, 46, 8);
}

void Cube::L() 
{   
    roll90(Face::LEFT);
    rotate_sides_90(6, 34, 46, 22, 0, 36, 40, 16);
}

void Cube::R() 
{
    roll90(Face::RIGHT);
    rotate_sides_90(2, 18, 42, 38, 4, 20, 44, 32);
}

void Cube::D() 
{
    roll90(Face::DOWN);
    rotate_sides_90(12, 36, 28, 20, 14, 38, 30, 22);
}

void Cube::U() 
{
    roll90(Face::UP);
    rotate_sides_90(8, 16, 24, 32, 10, 18, 26, 34);
}

void Cube::F_prime() 
{
    roll270(Face::FRONT);
    rotate_sides_90(30, 40, 10, 4, 24, 42, 12, 6);
}

void Cube::B_prime() 
{
    roll270(Face::BACK);
    rotate_sides_90(14, 44, 26, 0, 8, 46, 28, 2);
}

void Cube::L_prime() 
{
    roll270(Face::LEFT);
    rotate_sides_90(22, 46, 34, 6, 16, 40, 36, 0);
}

void Cube::R_prime() 
{
    roll270(Face::RIGHT);
    rotate_sides_90(38, 42, 18, 2, 32, 44, 20, 4);
}

void Cube::D_prime() 
{
    roll270(Face::DOWN);
    rotate_sides_90(20, 28, 36, 12, 22, 30, 38, 14);
}

void Cube::U_prime() 
{
    roll270(Face::UP);
    rotate_sides_90(32, 24, 16, 8, 34, 26, 18, 10);
}

void Cube::F2() 
{
    this->roll180(Face::FRONT);
    this->rotate_sides_180(4, 40, 10, 30, 6, 42, 12, 24);
}

void Cube::B2() 
{
    this->roll180(Face::BACK);
    this->rotate_sides_180(0, 44, 26, 14, 2, 46, 28, 8);
}

void Cube::L2() 
{
    this->roll180(Face::LEFT);
    this->rotate_sides_180(6, 46, 34, 22, 0, 40, 36, 16);
}

void Cube::R2() 
{
    this->roll180(Face::RIGHT);
    this->rotate_sides_180(2, 42, 18, 38, 4, 44, 20, 32);
}

void Cube::D2() 
{
    this->roll180(Face::DOWN);
    this->rotate_sides_180(12, 28, 36, 20, 14, 30, 38, 22);
}

void Cube::U2() 
{
    this->roll180(Face::UP);
    this->rotate_sides_180(8, 24, 16, 32, 10, 26, 18, 34);
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

        else {
            std::cerr << "Unrecognized move : " << move << '\n';
        }
    }

}

void Cube::scramble(int rotations)
{
    const char* possible_moves[12] = {
        "F ", "B ", "L ", "R ", "D ", "U ",
        "F' ", "B' ", "L' ", "R' ", "D' ", "U' "
    };
    
    std::string curr_move;
    for (int i = 0; i < rotations; i++)
        curr_move.append(possible_moves[std::rand() % 12]);
    
    std::cout << "Scrambing cube with:\n" << curr_move << '\n';
    this->move(curr_move);
}


static char get_name(Cube::Color c)
{
    switch (c) {
        case Cube::Color::WHITE:   return 'W'; break;
        case Cube::Color::GREEN:   return 'G'; break;
        case Cube::Color::RED:     return 'R'; break;
        case Cube::Color::BLUE:    return 'B'; break;
        case Cube::Color::ORANGE:  return 'O'; break;
        case Cube::Color::YELLOW:  return 'Y'; break;
        default: return 'X';
    }
}

void Cube::print()
{
    auto print_top_bottom = [this](int margin, int side) {
        std::printf("%*s", 3 + margin, "");
        for (int i = 0; i < 3; i++)
            std::printf("%c", get_name(data[side * 8 + i]));
        
        std::printf("\n%*s", 3 + margin, "");
        std::printf("%c%c%c", 
            get_name(data[side * 8 + 7]), 
            get_name((Cube::Color)side),
            get_name(data[side * 8 + 3])
        );

        std::printf("\n%*s", 3 + margin, "");
        for (int i = 0; i < 3; i++)
            std::printf("%c", get_name(data[side * 8 + 6 - i]));
        
        for (int i = 0; i < margin; i++)
            std::printf("\n");
    };

    auto print_center1 = [this](int margin, int i1, int i2, int i3) {
        for (int i = 1; i <= 4; i++) {
            std::printf("%c%c%c", 
                get_name(data[i * 8 + i1]),
                get_name(data[i * 8 + i2]),
                get_name(data[i * 8 + i3])
            );

            std::printf("%*s", margin, "");
        }
        std::printf("\n");
    };

    auto print_center2 = [this](int margin, int i1, int i3) {
        for (int i = 1; i <= 4; i++) {
            std::printf("%c%c%c", 
                get_name(data[i * 8 + i1]),
                get_name((Cube::Color)i),
                get_name(data[i * 8 + i3])
            );

            std::printf("%*s", margin, "");
        }
        std::printf("\n");
    };
    
    int margin = 2;
    
    print_top_bottom(margin, 0);

    print_center1(margin, 0, 1, 2);
    print_center2(margin, 7,    3);
    print_center1(margin, 6, 5, 4);

    for (int i = 0; i < margin - 1; i++)
        std::printf("\n");
    print_top_bottom(margin, 5);
}