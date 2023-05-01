#include "callbacks.h"

#include <sstream>
#include <stdexcept>
#include <vector>

#include "game.h"

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

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    void* ptr = glfwGetWindowUserPointer(window);
    Game* game = static_cast<Game*>(ptr);
    game->on_mouse(button, action, mods);
}       

void glfw_key_callback(
    GLFWwindow* window, 
    int key, 
    int scancode, 
    int action, 
    int mods
) {
    void* ptr = glfwGetWindowUserPointer(window);
    Game* game = static_cast<Game*>(ptr);
    game->on_key(key, scancode, action, mods);
}