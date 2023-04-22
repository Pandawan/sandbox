#include "callbacks.h"
#include "cells/cell.h"

#include <sstream>
#include <stdexcept>

// Tech Debt: The state tracking should be done in a class for neatness sake


/*
 * We have these variables to keep track of the state of our mouse
 * since GLFW does not explicitly support holding down a mouse button.
 */
bool left_click_down = false;
double prev_xpos = 0;
double prev_ypos = 0;
CellType cell_type;


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

void check_mouse_down(GLFWwindow* window, Grid* grid, size_t width, size_t height)
{
    if (left_click_down)
    {
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
        int x, y;
        x = prev_xpos / width * 100;
        y = 100 - prev_ypos / height * 100;
        glm::ivec2 idx = glm::vec2(x, y);
        Cell cell = grid->get_cell(idx);

        if (cell.cell_type == EMPTY) {
            cell.cell_type = cell_type;
            cell.set_color();
            grid->set_cell(idx, cell);
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
}

void toggle_cell(Grid* grid)
{
    grid->toggled_cell = cell_type;
}
