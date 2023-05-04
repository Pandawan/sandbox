#ifndef GAME_H
#define GAME_H

#include <cstddef>
#include <glm/vec2.hpp>

#include "gl.h"
#include "ui.h"
#include "world.h"

class Game {
private:
    /** Window this ui is attached to. */
    GLFWwindow* window;
    /** Width of ui. */
    std::size_t width;
    /** Height of ui. */
    std::size_t height;

    /** Mouse position in grid coordinates. */
    glm::uvec2 mouse_pos;
    /** Mouse is within bounds. */
    bool mouse_in_view;
    bool mouse_left;
    bool mouse_right;

    void update_mouse_pos();

public:
    World world;
    UI ui;

    Game(GLFWwindow* window, std::size_t width, std::size_t height);

    /**
     * Update performed on every frame. 
     * Use this for input-dependent behavior. 
     */
    void update(double delta_time);
    /** 
     * Update performed on a fixed tick interval. 
     * Use this for simulation/physics behavior.
     */
    void fixed_update(double delta_time);
    void render();

    /** Key event handler. */
    void on_key(int key, int scancode, int action, int mods);
    /** Mouse event handler. */
    void on_mouse(int button, int action, int mods);
};

#endif