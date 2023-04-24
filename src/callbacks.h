#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "gl.h"
#include "grid.h"
#include "render/quad.h"

struct Region
{
    int x;
    int y;
    int dx;
    int dy;
    CellType cell_type;
};

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
 * @button see https://www.glfw.org/docs/3.0/group__buttons.html for types.
 * @action is either GLFW_PRESS or GLFW_RELEASE.
 * @mod is bit field describing which modifier keys were held down.
 *   See https://www.glfw.org/docs/3.0/group__mods.html more information
 */
void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mod);

/**
 * This function is used to check the position of the mouse cursor during a drag
 * action.
 */
void check_mouse_down(GLFWwindow* window, Grid *grid, int grid_height);

/*
 * Callback function for keyboard input.
 */
void key_callback(GLFWwindow* window, int key, __unused int scancode, __unused int action, __unused int mods);


/**
 * Tethers the type of Cell to Grid.
 */
void toggle_cell(Grid* grid);

/**
 * Triggers the UI display 
 */
bool display_ui();

/*
 * Signals to the Quad to render the UI
 */
void display_ui(__unused Quad* quad);

/**
 * This function will assign the logical placements for the UI buttons
 */
void ui_partition(int grid_width, int grid_height);

/**
 * Allows switching CellType by iterating through a the ui_regions
 * and getting the corresponding cell_type if there's a match.
 * Should only be called within regions where UI is enabled. 
 */
void toggle_cell_type(GLFWwindow *window);

/**
 * A helper function used to determine if a mous click is within a rectangular region
 */
bool within_bounds(int xpos, int ypos, int x, int y, int dx, int dy);

#endif
