#include "callbacks.h"
#include "cells/cell.h"

#include <sstream>
#include <stdexcept>
#include <vector>

// Tech Debt: The state tracking should be done in a class for neatness sake
// TODO: Yeah this could all use some refactoring

/*
 * We have these variables to keep track of the state of our mouse
 * since GLFW does not explicitly support holding down a mouse button.
 */
bool left_click_down = false;
bool toggle_ui = false; // TODO: Rename this, not descriptive enough
double prev_xpos = 0;
double prev_ypos = 0;
int window_width;
int window_height;
std::vector<Region> ui_region; // TODO: Bruh
CellType cell_type; // TODO: CAN WE USE DESCRIPTIVE VAR NAMES??
bool clear_grid = false; // TODO: I hate this

// TODO: You could have put this in a separate file at least
void ui_partition(int window_width, int window_height)
{
    int width = 0;
    int height = 0;
    int width_offset = window_width / 5;
    int height_offset = window_height / 5;
    // First, initialize the number of UI elements
    for (int i = EMPTY; i != LAST; i++)
    {
        CellType cell_type = static_cast<CellType>(i);
        Region cell_region = Region {
            width, 
            height, 
            width + width_offset, 
            height + height_offset, 
            cell_type
        };
        if (i % 5 == 0 && i != EMPTY)
        {
            width = 0;
            height += height_offset;
        }
        else 
        {
            width += width_offset;
        }
        ui_region.push_back(cell_region);
        // std::cout << width << " " << width + width_offset << " " << height << " " << height + height_offset << std::endl;
    }
}

void glfw_error_callback(int error, const char* description)
{
    std::stringstream error_message;
    error_message << "Error " << error << ": " << description << std::endl;
    throw std::runtime_error(error_message.str());
}

void glfw_resize_callback(__unused GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfw_mouse_callback(GLFWwindow* window,
                         int button,
                         __unused int action,
                         __unused int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        toggle_cell_type(window);
        if (action == GLFW_RELEASE)
            left_click_down = false;
        else if (action == GLFW_PRESS)
            left_click_down = true;
    }
    
    if (!left_click_down)
    {
        prev_xpos = xpos;
        prev_ypos = ypos;
    }
}

bool within_bounds(int xpos, int ypos, int x, int y, int dx, int dy)
{
    return xpos <= dx && x <= xpos && ypos <= dy && y <= ypos;
}

void toggle_cell_type(GLFWwindow* window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    for (size_t i = 0; i < ui_region.size(); i++)
    {
        Region region = ui_region[i];

        if (within_bounds((int)xpos, (int)ypos, region.x, region.y, region.dx, region.dy))
        {
            cell_type = region.cell_type;
            // std::cout << "in bounds for " << region.cell_type << std::endl;
        }
    }
}

void check_mouse_down(GLFWwindow* window, Grid* grid, int grid_height)
{
    if (left_click_down && !display_ui())
    {
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
        glfwGetWindowSize(window, &window_width, &window_height);
        int x, y;
        x = prev_xpos / window_width * 100;
        y = grid_height - prev_ypos / window_height * 100;
        glm::ivec2 idx = glm::vec2(x, y);
        Cell cell = grid->get_cell(idx);

        cell.cell_type = cell_type;
        cell.set_color();
        grid->set_cell(idx, cell);
    }
}

// TODO: We really need to refactor this whole thing lol
void check_clear_pressed(Grid* grid) {
    if (clear_grid) {
        // Ignore clear actions when focused on the UI
        if (display_ui()) {
            clear_grid = false;
        }
        else {
            grid->clear();
        }
    }
}

void key_callback(__unused GLFWwindow* window, int key, __unused int scancode, int action, __unused int mods)
{
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        cell_type = EMPTY;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        cell_type = SAND;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        cell_type = WATER;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        cell_type = WOOD;
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        cell_type = UI;
    }
    else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        toggle_ui = !toggle_ui;
    }
    else if (key == GLFW_KEY_R) {
        if (action == GLFW_PRESS) {
            clear_grid = true;
        }
        else if (action == GLFW_RELEASE) {
            clear_grid = false;
        }
    }
}

bool display_ui()
{
    return toggle_ui;
}

void toggle_cell(Grid* grid)
{
    grid->toggled_cell = cell_type;
}
