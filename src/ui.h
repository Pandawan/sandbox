#ifndef UI_H
#define UI_H

#include <cstddef>
#include <vector>
#include <glm/vec3.hpp>

#include "gl.h"
#include "grid.h"
#include "cells/cell.h"
#include "render/quad.h"

class UI {
private:
    /** Window this ui is attached to. */
    __unused GLFWwindow* window;
    /** Width of ui. */
    std::size_t width;
    /** Height of ui. */
    std::size_t height;

    /** Grid to store ui on. */
    Grid grid;
    /** Quad to render ui with. */
    Quad quad;

    /** List of available cells to render in the UI. */
    std::vector<Cell> cell_kinds = {
        Cell::Sand(),
        Cell::Water(),
        Cell::Stone(),
        Cell::Wood()
    };
    /** Number of columns to render in the UI. */
    std::size_t columns = 5;
    /** Currently selected cell index. */
    std::size_t selected;
    /** Cell kind to use for selection outline. */
    Cell selected_cell_kind = Cell("ui", glm::u8vec3(252, 245, 104), CellBehavior::IMMOVABLE_SOLID, 0);

    /** Render grid with UI elements. */
    void render_grid();

public:
    /** Whether or not to display the UI. */
    bool display;

    UI(GLFWwindow* window, std::size_t width, std::size_t height);

    void update(double delta_time);
    void render();

    void set_selected(std::size_t index);
    Cell get_selected();

    void make_selection(glm::uvec2 mouse_pos);
};

#endif