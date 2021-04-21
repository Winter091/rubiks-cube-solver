#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Cube.hpp"
 
int main()
{
    std::srand(std::time(0));

    Cube c;
    c.scramble(99);
    c.print();
}