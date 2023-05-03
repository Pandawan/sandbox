#include "ui.h"

#include <algorithm>
#include <iostream>

#include "render/font8x8_basic.h"

UI::UI(
    GLFWwindow* window,
    std::size_t width,
    std::size_t height
) : window(window),
    width(width),
    height(height),
    grid(width, height),
    quad(window, GL_TEXTURE1, grid.get_texture()),
    selected(0),
    display(false)
{
    // UI grid should not perform updates
    grid.paused = true;
}

void UI::update(double delta_time) {
    this->grid.clear();
    this->render_text();
    this->render_grid();

    grid.update(delta_time);
}

void UI::render() {
    quad.render();
}

void UI::render_text() {
    std::string text = this->get_selected().name;
    std::size_t max_text_len = (this->width - text_row_margin * 2) / 8;

    std::size_t x = text_row_margin;
    std::size_t y = this->height - text_row_count + text_row_margin;

    for(std::size_t i = 0; i < text.size() && i < max_text_len; i++) {
        char c = text[i];

        unsigned char* bitmap = font8x8_basic[static_cast<std::size_t>(c)];

        for (std::size_t local_x = 0; local_x < 8; local_x++) {
            for (std::size_t local_y = 0; local_y < 8; local_y++) {
                bool set = bitmap[local_y] & 1 << local_x;
                glm::uvec2 cell_pos = glm::uvec2(x + local_x, y + (7 - local_y));

                if (set) {
                    this->grid.set_cell(cell_pos, selected_cell_kind);
                }
            }
        }

        x += 8;
    }
}

void UI::render_grid() {
    std::size_t available_height = this->height - text_row_count;

    std::size_t rows = (this->cell_kinds.size() + this->columns) / this->columns;

    std::size_t region_width = this->width / this->columns;
    std::size_t region_height = std::min(available_height / rows, region_width);

    std::size_t x = 0;
    std::size_t y = available_height - region_height;

    for (std::size_t i = 0; i < this->cell_kinds.size(); i++) {
        Cell cell_kind = this->cell_kinds[i];

        // Loop through region
        for(std::size_t local_x = 0; local_x < region_width; local_x++) {
            for(std::size_t local_y = 0; local_y < region_width; local_y++) {
                glm::uvec2 cell_pos = glm::uvec2(x + local_x, y + local_y);

                // Selected outline
                if (
                    selected == i && (
                        local_x == 0 || local_x == region_width - 1 || 
                        local_y == 0 || local_y == region_height - 1
                    )
                ) {
                    this->grid.set_cell(cell_pos, selected_cell_kind);
                }
                // Actual tile selection
                else {
                    this->grid.set_cell(cell_pos, cell_kind);
                }
            }
        }

        // Moving to a new row
        if (i % (this->columns - 1) == 0 && i != 0) {
            x = 0;
            y -= region_height;
        }
        // Moving to next column
        else {
            x += region_width;
        }
    }
}

void UI::set_selected(std::size_t index) {
    if (index < this->cell_kinds.size()) {
        this->selected = index;
    }
}

Cell UI::get_selected() {
    return this->cell_kinds[this->selected];
}

void UI::make_selection(glm::uvec2 mouse_pos) {
    std::size_t available_height = this->height - text_row_count;

    std::size_t rows = (this->cell_kinds.size() + this->columns) / this->columns;

    std::size_t region_width = this->width / this->columns;
    std::size_t region_height = std::min(available_height / rows, region_width);

    std::size_t x = 0;
    std::size_t y = available_height - region_height;

    for (std::size_t i = 0; i < this->cell_kinds.size(); i++) {
        // Within bounds of this "region"
        if (
            x <= mouse_pos.x && mouse_pos.x < x + region_width &&
            y <= mouse_pos.y && mouse_pos.y < y + region_height
        ) {
            set_selected(i);
        }

        // Moving to a new row
        if (i % (this->columns - 1) == 0 && i != 0) {
            x = 0;
            y -= region_height;
        }
        // Moving to next column
        else {
            x += region_width;
        }
    }
}
