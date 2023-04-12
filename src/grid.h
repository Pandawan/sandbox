#ifndef GRID_H
#define GRID_H

#include <cstddef>
#include <glm/vec2.hpp>

#include "gl.h"

typedef bool Cell;

/**
 * The Grid. A ditigal frontier.
 */
class Grid {

private:
    Cell* grid;

    std::size_t height;
    std::size_t width;

    /** OpenGL id to the generated grid texture. */
    GLuint texture;

public:

    Grid(std::size_t width, std::size_t height);
    ~Grid();

    void set_cell(glm::ivec2 position, Cell value);

    Cell& get_cell(glm::ivec2 position);

    GLuint generate_texture();
};
#endif