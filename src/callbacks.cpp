#include "callbacks.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

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
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
    }
}
