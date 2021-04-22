
#ifndef CUBE_H_
#define CUBE_H_

#include <cstdint>
#include <cstdio>
#include <string>

/**
   *     WWW
   *     WWW
   *     WWW
   * 
   * GGG RRR BBB OOO
   * GGG RRR BBB OOO
   * GGG RRR BBB OOO
   * 
   *     YYY
   *     YYY
   *     YYY
   * 
   * Edge orientations and colors:
   *    0  1  2  3  4  5  6  7  8  9  10 11
   *    UF UL UB UR FR FL BL BR DF DL DB DR
   *    WR WG WO WB RB RG OG OB YR YG YO YB
   * 
   * Corner orientations and colors:
   *    0   1   2   3   4   5   6   7
   *    UFR UFL UBL UBR DFR DFL DBL DBR
   *    WRB WRG WOG WOB YRB YRG YOG YOB
   * 
**/ 

struct Cubie
{
    uint8_t index;

    // 0 is right, everything else is wrong
    uint8_t orientation;
};

class Cube
{
private:
    Cubie edges[12];
    Cubie corners[8];

    void rotate_face_90(int e1, int e2, int e3, int e4, int c1, int c2, int c3, int c4);
    void rotate_face_180(int e1, int e2, int e3, int e4, int c1, int c2, int c3, int c4);
    void update_edge_orientation(int index);
    void update_corner_orientation(int index, int step);

public:
    Cube();

    void print();
    void move(const std::string& moves);
    void scramble(int num_rotations);

    void U();
    void D();
    void L();
    void R();
    void F();
    void B();

    void U_prime();
    void D_prime();
    void L_prime();
    void R_prime();
    void F_prime();
    void B_prime();

    void U2();
    void D2();
    void L2();
    void R2();
    void F2();
    void B2();

};

#endif