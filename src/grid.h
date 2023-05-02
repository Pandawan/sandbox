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

    /** Whether or not the cell has updated within one call of simulate_grid */
    bool* cell_dirty;

    std::size_t width;
    std::size_t height;

    /** Whether or not the grid has changed since the last update. */
    bool dirty;

    /** Generate the texture from the current grid state. */
    void generate_texture();

    /** Simulate each Cell element within the grid. */
    void simulate_grid(double delta_time);

    /**
     * Simulates a freaking bee
     * @returns True if the beed
     */
    bool simulate_bee(glm::uvec2 position, double delta_time);

    /** 
     * Simulate a solid cell at the given position.
     * @returns True if the cell did update/move.
    */
    bool simulate_solid(glm::uvec2 position, double delta_time);

    /** 
     * Simulate a liquid cell at the given position.
     * @returns True if the cell did update/move.
     */
    bool simulate_liquid(glm::uvec2 position, double delta_time);

    /**
     * Simulate a gas cell at the given position.
     * @returns True if the cell did update/move. 
     */
    bool simulate_gas(glm::uvec2 position, double delta_time);

    /**
     * Simulate a plasma cell at the given position.
     * @returns True if cell did update/move
     */
    bool simulate_plasma(glm::uvec2 position, double delta_time);

    /** Swap two cells in the grid. */
    void swap_cells(glm::uvec2 first, glm::uvec2 second);

    /** 
     * Used to proliferate @spread onto @victim and sets lifetime of @victim to @lifetime
     * @returns True if proliferated successfully
     */
    bool proliferate(Cell* spread, Cell* victim, glm::uvec2 victim_pos, double lifetime);

public:
    /** OpenGL id to the generated grid texture. */
    GLuint texture;
    /** Whether or not to pause the simulation. */
    bool paused;

    Grid(std::size_t width, std::size_t height);
    ~Grid();

    void set_cell(glm::uvec2 position, Cell value);

    Cell* get_cell(glm::uvec2 position);

    /* Clear the entire grid with the given cell */
    void clear();

    GLuint get_texture() const;

    /** Simulate the grid. */
    void update(double delta_time);
};
#endif
