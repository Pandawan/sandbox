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

    /** Generate the texture from the current grid state. */
    void generate_texture();

    /** Updates each Cell element within the grid. */
    void update_grid(__unused double delta_time);

    /** Move Cell from current location to new location */
    bool move(const size_t &cell_x, const size_t &cell_y, const size_t &new_x, const size_t &new_y);

    /** Move Cell from current location to left */
    bool move_left(const size_t &x, const size_t &y);

    /** Move Cell from current location to right */
    bool move_right(const size_t &x, const size_t &y);

    /** Move Cell from current location to below */
    bool move_down(const size_t &x, const size_t &y);

    /** Move Cell from current location to above */
    bool move_up(const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally down left */
    bool move_down_left(const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally down right */
    bool move_down_right(const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally up left */
    bool move_up_left(const size_t &x, const size_t &y);

    /** Move Cell from current location to diagonally up right */
    bool move_up_right(const size_t &x, const size_t &y);

    /** Logic for solid movement */
    bool move_logic_solid(const size_t &cur_x, const size_t &cur_y, const double &delta_time);

    /** Logic for liquid movement */
    bool move_logic_liquid(const size_t &cur_x, const size_t &cur_y, const double &delta_time);

    /** liquid movement to empty cell */
    bool flow_down(const size_t &cur_x, const size_t &cur_y, const double &delta_time);

    /** Swapping two cells */
    bool swap_cell(const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y);

public:
    /** OpenGL id to the generated grid texture. */
    GLuint texture;

    // Determines what kind of cell we're going to place down.
    CellType toggled_cell;

    Grid(std::size_t width, std::size_t height);
    ~Grid();

    void set_cell(glm::ivec2 position, Cell value);

    Cell& get_cell(glm::ivec2 position);

    /* Clear the entire grid with the given cell */
    void clear();

    GLuint get_texture();

    /** Simulate the grid for one tick. */
    void update(double delta_time);

    /** Simulate the grid for the UI. */
    void ui_update();
};
#endif
