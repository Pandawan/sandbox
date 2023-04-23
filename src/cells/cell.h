/**
 * This is the base class that all other subclasses will inherit from. 
 */
#ifndef CELL_H
#define CELL_H

#include <vector>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

enum CellType 
{
    EMPTY = 0,
    SAND,
    WATER,
    WOOD,
    UI
};

enum StateOfMatter 
{
    liquid,
    movable_solid,
    immovable_solid,
    gas, 
    none
};

class Cell
{
    public:
        Cell();
        ~Cell();
        glm::dvec2 velocity;
        glm::dvec4 RGBA;
        CellType cell_type;
        StateOfMatter cell_state;

        /* Given this pixel's cell type, we'll set the corresponding RGBA values*/
        void set_color();

        /* Given color in r, g, b, set them according to glm RGBA*/
        void set_rgba(const float &r, const float &g, const float &b, const float &a);
};

#endif