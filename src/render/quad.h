#ifndef QUAD_H
#define QUAD_H

#include "shader.h"
#include <cstddef>
#include <src/gl.h>

class Quad
{

private:
    GLuint vbo, ebo, vao;

    /** Shader to render the quad with. */
    Shader shader;

    /** Window the quad is rendering on. */
    GLFWwindow* window;

    /** The texture unit to grab the texture from. */
    GLenum texture_unit;
    /** The texture to render on the quad. */
    GLuint texture;

    /**
     * Initialize the quad in OpenGL. Called in constructor.
     */
    void init();

public:
    /**
     * Create a new Quad and its corresponding objects in OpenGL.
     * @param window The window this quad should render in.
     * @param texture_unit The unit to grab the texture from.
     * @param texture The texture to render.
     */
    Quad(
        GLFWwindow* window,
        GLenum texture_unit,
        GLuint texture
    );

    ~Quad();

    void render();
};

#endif