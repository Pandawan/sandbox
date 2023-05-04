#ifndef WORLD_H
#define WORLD_H

#include <cstddef>

#include "gl.h"
#include "grid.h"
#include "render/quad.h"

class World {
private:
    /** Window this world is attached to. */
    __unused GLFWwindow* window;

    /** Grid to store world on. */
    Grid grid;
    /** Quad to render world with. */
    Quad quad;

public:
    World(GLFWwindow* window, std::size_t width, std::size_t height);

    Grid* get_grid();

    void update(double delta_time);
    void fixed_update(double delta_time);
    void render();
};

#endif