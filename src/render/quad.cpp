#include "quad.h"

#include <algorithm>

#include <glm/mat4x4.hpp>

const float vertices[] = {
    // positions          // colors           // texture coords
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
const unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Quad::Quad(
    GLFWwindow* window, 
    GLenum texture_unit, GLuint texture
) : 
    shader("../shaders/default.vs", "../shaders/default.fs"), 
    window(window),
    texture_unit(texture_unit), texture(texture)
{
    this->init();
}

Quad::~Quad()
{
    glfwMakeContextCurrent(this->window);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

void Quad::init()
{
    glfwMakeContextCurrent(this->window);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Quad::render()
{
    glfwMakeContextCurrent(this->window);
    // Bind textures on given texture unit
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Bind quad buffers
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    // Compute rendering size
    int window_width, window_height;
    glfwGetWindowSize(this->window, &window_width, &window_height);
    double window_aspect_ratio = (double)window_width / (double)window_height;

    int texture_width, texture_height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texture_height);
    double texture_aspect_ratio = (double)texture_width / (double)texture_height;

    // Source: https://bumbershootsoft.wordpress.com/2017/11/26/gtk3-aspect-corrected-image-scaling-with-opengl/
    double scale_width, scale_height;
    if (window_aspect_ratio < texture_aspect_ratio) {
        scale_width = 1;
        scale_height = window_aspect_ratio / texture_aspect_ratio;
    } 
    else {
        scale_width = texture_aspect_ratio / window_aspect_ratio;
        scale_height = 1;
    }

    // Bind shader
    shader.use();
    shader.setInt("uTexture", 0);
    glUniform4f(glGetUniformLocation(shader.ID, "uScale"), scale_width, scale_height, 1.0f, 1.0f);

    // Draw the quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}