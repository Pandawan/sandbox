#ifndef WINDOW_H
#define WINDOW_H

#include "gl.h"

/**
 * Initialize the GLFW library.
 */
void init_glfw();

/**
 * Initialize and display a new GLFW window.
 */
GLFWwindow* create_window(int width, int height, const char* title);

#endif