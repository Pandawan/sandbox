#include "world.h"

World::World(
    GLFWwindow* window,
    std::size_t width,
    std::size_t height
) : window(window),
    grid(width, height),
    quad(window, GL_TEXTURE0, grid.get_texture())
{
}

Grid* World::get_grid() {
    return &this->grid;
}

void World::update(double delta_time) {
    grid.update(delta_time);
}

void World::fixed_update(double delta_time) {
    grid.fixed_update(delta_time);
}

void World::render() {
    quad.render();
}