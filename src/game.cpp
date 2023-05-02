#include "game.h"

#include <iostream>
#include <algorithm>

Game::Game(
    GLFWwindow* window, 
    std::size_t width, 
    std::size_t height
) : window(window),
    width(width),
    height(height),
    world(window, width, height),
    ui(window, width, height)
{
    glfwSetWindowUserPointer(this->window, this);
}

void Game::update(double delta_time) {
    this->update_mouse_pos();

    // TODO: Is this good? Should we just keep updating UI anyway?
    if (this->ui.display) {
        this->ui.update(delta_time);
    } else {

        // TODO: The whole mouse_in_view is gross but I kinda have to because 
        // 1. cell_positions are unsigned, 2. mouse can have negative values
        // so need to convert between these.
        if (this->mouse_in_view) {   
            // TODO: This feels slightly gross; I feel like the World itself should worry about this stuff
            // Set cells based on mouse input
            if (this->mouse_left) {
                Cell cell_kind = this->ui.get_selected();
                this->world.get_grid()->set_cell(this->mouse_pos, cell_kind);
            } else if (this->mouse_right) {
                this->world.get_grid()->set_cell(this->mouse_pos, Cell::Empty());
            }
        }
    
        // Update world
        this->world.update(delta_time);
    }
}

void Game::update_mouse_pos() {
    double mouse_x, mouse_y;
    glfwGetCursorPos(this->window, &mouse_x, &mouse_y);
    int window_width, window_height;
    glfwGetWindowSize(this->window, &window_width, &window_height); 

    this->mouse_in_view = (
        mouse_x >= 0 && mouse_x < window_width &&
        mouse_y >= 0 && mouse_y < window_height
    );

    // Calculate grid position
    size_t grid_x = static_cast<size_t>(
        std::clamp(
            floor(mouse_x / (double)window_width * this->width),
            0.0, (double)this->width
        )
    );
    size_t grid_y = static_cast<size_t>(
        std::clamp(
            this->height - floor(mouse_y / (double)window_height * this->height),
            0.0, (double)this->height
        )
    );

    this->mouse_pos = glm::uvec2(grid_x, grid_y);

}

void Game::render() {
    if (this->ui.display) {
        this->ui.render();
    } else {
        this->world.render();
    }
}

// TODO: I wish the UI and World could handle their own key/mouse events.
// This could be changed if the "selected cell" was handled by the `Game`?
void Game::on_key(int key, __unused int scancode, int action, __unused int mods) {
    // Tab to open ui
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        this->ui.display = !this->ui.display;
    }
    // Number keys to select elements
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        this->ui.set_selected(0);
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        this->ui.set_selected(1);
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        this->ui.set_selected(2);
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        this->ui.set_selected(3);
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        this->ui.set_selected(4);
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        this->ui.set_selected(5);
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
        this->ui.set_selected(6);
    }
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
        this->ui.set_selected(7);
    }
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
        this->ui.set_selected(8);
    }
    // R to clear
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (this->ui.display == false) {
            this->world.get_grid()->clear();
        }
    }
}

void Game::on_mouse(__unused int button, __unused int action, __unused int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            this->mouse_left = true;

            if (this->ui.display) {
                // TODO: This feels a little un-clean but w/e?
                this->ui.make_selection(this->mouse_pos);
            }
        }
        else if (action == GLFW_RELEASE) {
            this->mouse_left = false;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            this->mouse_right = true;
        }
        else if (action == GLFW_RELEASE) {
            this->mouse_right = false;
        }
    }
}
