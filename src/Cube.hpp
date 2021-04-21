
#ifndef CUBE_H_
#define CUBE_H_

#include <cstdint>
#include <cstdio>
#include <string>

/**
   * A raw Rubik's Cube model.  This is _not_ what is drawn.  It's a fairly
   * fast Rubik's Cube model used by the solver, and kept in sync with the
   * drawable Rubik's Cube WorldObject.
   *
   * The cube is laid out as follows.
   *
   * The sides:
   *
   *    U
   *  L F R B
   *    D
   *
   * Color wise:
   *
   *          W W W
   *          W W W
   *          W W W
   *
   *  G G G   R R R   B B B   O O O
   *  G G G   R R R   B B B   O O O
   *  G G G   R R R   B B B   O O O
   *
   *          Y Y Y
   *          Y Y Y
   *          Y Y Y
   *
   * Index wise:
   *
   *
   *              0  1  2
   *              7     3
   *              6  5  4
   *
   *   8  9 10   16 17 18   24 25 26   32 33 34
   *  15    11   23    19   31    27   39    35
   *  14 13 12   22 21 20   30 29 28   38 37 36
   *
   *             40 41 42
   *             47    43
   *             46 45 44
   *
   * Centers:
   *
   *    0
   *  1 2 3 4
   *    5
   */

class Cube
{
public:
    using Color = enum class Color : uint8_t { WHITE = 0u, GREEN, RED, BLUE, ORANGE, YELLOW };
    using Face = enum class Face : uint8_t { UP, LEFT, FRONT, RIGHT, BACK, DOWN };

private:
    Color data[48];

public:
    Cube();

    void roll90(Face f);
    void roll180(Face f);
    void roll270(Face f);

    void rotate_sides_90(int i1, int i2, int i3, int i4, int c1, int c2, int c3, int c4);
    void rotate_sides_180(int i1, int i2, int i3, int i4, int c1, int c2, int c3, int c4);

    void F();
    void B();
    void L();
    void R();
    void D();
    void U();

    void F_prime();
    void B_prime();
    void L_prime();
    void R_prime();
    void D_prime();
    void U_prime();

    void F2();
    void B2();
    void L2();
    void R2();
    void D2();
    void U2();

    void move(const std::string& moves);
    void scramble(int rotations);

    void print();

};

#endif