#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "gl.h"

/**
 * Callback when GLFW encounters an error.
 *
 * @throws Throws back the error as a std::runtime_error.
 */
void glfw_error_callback(int error, const char* description);

/**
 * Callback when the given window has been resized.
 */
void glfw_resize_callback(GLFWwindow* window, int width, int height);

#endif