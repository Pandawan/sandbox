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

    /** Move Cell from current location to new location */
    bool move(Cell cell, const size_t &cell_x, const size_t &cell_y, const size_t &new_x, const size_t &new_y);

    /** Move Cell from current location to left */
    bool move_left(Cell cell_type, const size_t &x, const size_t &y);

    /** Move Cell from current location to right */
    bool move_right(Cell cell_type, const size_t &x, const size_t &y);

    /** Move Cell from current location to below */
    bool move_down(Cell cell_type, const size_t &x, const size_t &y);

    /** Move Cell from current location to above */
    bool move_up(Cell cell_type, const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally down left */
    bool move_down_left(Cell cell, const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally down right */
    bool move_down_right(Cell cell, const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally up left */
    bool move_up_left(Cell cell, const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally up right */
    bool move_up_right(Cell cell, const size_t &x, const size_t &y);

    /** Logic for solid movement */
    bool move_logic_solid(Cell cell, const size_t &cur_x, const size_t &cur_y);

    /** Logic for liquid movement */
    bool move_logic_liquid(Cell cell, const size_t &cur_x, const size_t &cur_y);

    /** liquid movement to empty cell */
    bool flow_down(Cell cell, const size_t &cur_x, const size_t &cur_y);

    /** Swapping two cells */
    bool swap_cell(Cell cell,const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y);

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
