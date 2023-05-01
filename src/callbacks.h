#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "gl.h"

/*
 * We have this class to store the different types of cells dynamically
 */

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

/**
 * This callback function is the entry point for any logic tied to the mouse
 * input commands. 
 * @param button see https://www.glfw.org/docs/3.0/group__buttons.html for types.
 * @param action is either GLFW_PRESS or GLFW_RELEASE.
 * @param mod is bit field describing which modifier keys were held down.
 *   See https://www.glfw.org/docs/3.0/group__mods.html more information
 */
void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mod);

/*
 * Callback when a keyboard input occurs.
 */
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
