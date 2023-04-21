#ifndef GRID_H
#define GRID_H

#include <cstddef>
#include <glm/vec2.hpp>

#include "gl.h"
#include "cells/cell.h"

/**
 * The Grid. A ditigal frontier.
 * One small step for sand, one giant leap for sand-kind
 */
class Grid {

private:
    Cell* grid;

    std::size_t height;
    std::size_t width;

    /** OpenGL id to the generated grid texture. */
    GLuint texture;

    /** Generate the texture from the current grid state. */
    void generate_texture();

    /** Updates each Cell element within the grid. */
    void update_grid();

public:
    
    // Determines what kind of cell we're going to place down.
    CellType toggled_cell;

    Grid(std::size_t width, std::size_t height);
    ~Grid();

    void set_cell(glm::ivec2 position, Cell value);

    Cell& get_cell(glm::ivec2 position);

    GLuint get_texture();

    /** Simulate the grid for one tick. */
    void update(double delta_time);
};
#endif
