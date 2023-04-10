#include "callbacks.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

/*
 * We have these variables to keep track of the state of our mouse
 * since GLFW does not explicitly support holding down a mouse button.
 */
bool left_click_down = false;
double prev_xpos = 0;
double prev_ypos = 0;

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

void glfw_mouse_callback(__unused GLFWwindow* window,
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
