#include "grid.h"
#include "gl.h"

/*
    RESOURCES
    - Texture Units https://stackoverflow.com/questions/57621863/opengl-some-clarification-on-using-multiple-textures-or-texture-units
    - Active vs Bind https://stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture

*/


Grid::Grid(std::size_t width, std::size_t height) {
    this->width = width;
    this->height = height;

    // Initialize cell grid
    this->grid = new Cell[height * width];

    // Initialize texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // Warp texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Use nearest neighbor (pixel-perfect) sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Don't allow mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

}

Grid::~Grid() {
    glDeleteTextures(1, &this->texture);
}

void Grid::set_cell(glm::ivec2 position, Cell value) {
    grid[(position.y * width) + position.x] = value;
}

Cell& Grid::get_cell(glm::ivec2 position) {
    return grid[(position.y * width) + position.x];
}

GLuint Grid::get_texture() {
    return this->texture;
}

void Grid::generate_texture() {
    // TODO: May be better to allocate one at the start and keep on modifying it
    float* data = new float[(this->height * this->width) * 3];

    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            // if (x % 2 == 0 && y % 2 == 0) {
                data[((y * this->width) + x) * 3] = (float)x / (float)this->width;
                data[((y * this->width) + x) * 3 + 1] = (float)y / (float)this->height;
                data[((y * this->width) + x) * 3 + 2] = 0.5f;
            // }
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clear the working pixel buffer
    delete[] data;
}

void Grid::update(__unused double delta_time) {
    generate_texture();
}