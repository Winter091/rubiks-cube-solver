#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Cube.hpp"
#include "Solver.hpp"

int main()
{
    std::srand(std::time(0));

    Cube c;

    // Scramble where G2 -> G3 takes very long (716 sec)
    /*
    c.move("R' L2 B L' B R F' R U' L2 L D2 B R' B2 U2 D' "
    " L' U' R' D2 R L D2 U' R2 U' U2 R2 D2 F' R' R2 "
    " B2 D' D2 U2 L2 L D' U' D F' F2 U' L R' B L' B "
    " L' D U' F R2 F' U' B2 B B' U2 D' D F2 F L U' "
    " R2 L B L' D F' F L2 L' R2 L2 R' F' F B2 R2 F "
    " L' B R' B L L2 U2 D' F R' D' F B U B L");
    */
   
    c.scramble(100);
    find_solution(c);
}