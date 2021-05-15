
#ifndef CUBE_H_
#define CUBE_H_

#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

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
   * 
   *            00 01 02
   *            03 04 05
   *            06 07 08
   * 
   * 09 10 11   12 13 14   15 16 17   18 19 20
   * 21 22 23   24 25 26   27 28 29   30 31 32
   * 33 34 35   36 37 38   39 40 41   42 43 44
   * 
   *            45 46 47
   *            48 49 50
   *            51 52 53
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
**/ 

struct Cubie
{
    uint8_t index;

    // 0 is good, everything else is bad
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
    
    void check_colors_array(char colors[54]);

public:
    Cube();
    Cube(const char* filepath);

    friend std::ostream& operator<<(std::ostream& os, const Cube& c);
    
    void move(const std::string& moves);
    void scramble(int num_moves);
    void restore();
    void set_from_colors(char colors[54]);
    void set_from_file(const char* filepath);

    int get_corner_orientation(int c_index) const;
    int get_edge_orientation(int e_index) const;
    int get_corner_index(int c_index) const;
    int get_edge_index(int e_index) const;

    void move_indexed(int index);
    void unmove_indexed(int index);

    static const char* get_indexed_move_name(int index);
    static bool move_is_unnecessary(int move, int prev_move);

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