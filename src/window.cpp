#include "window.h"

#include <iostream>
#include <stdexcept>

#include "callbacks.h"

void init_glfw()
{
    // Register error callback
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize glfw
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwSetTime(0);

    // Set GLFW parameters
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    // TODO: Make this resizable again (requires mouse->grid coordinate system work with resizable coordinates)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

GLFWwindow* create_window(int width, int height, const char* title)
{
    // Create a GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(window);

    // Load the OpenGL bindings for the window context.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        throw std::runtime_error("Failed to initialize OpenGL context.");
    }
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

    // Clear screen once
    glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set size of buffer & viewport
    // NOTE: These may be different from window size (e.g. retina displays).
    int buffer_width, buffer_height;
    glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
    glViewport(0, 0, buffer_width, buffer_height);
    glfwSwapInterval(1);
    glfwSwapBuffers(window);

    // Register for window resizing event
    glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

    return window;
}