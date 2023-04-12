#ifndef QUAD_H
#define QUAD_H

#include <src/gl.h>
#include "shader.h"

class Quad {

private:
    GLuint vbo, ebo, vao;

    /** Shader to render the quad with. */
    Shader shader;

    /** 
     * Initialize the quad in OpenGL. Called in constructor. 
     */
    void init();

public:
    /**
     * Create a new Quad and its corresponding objects in OpenGL.
     */
    Quad();

    ~Quad();

    void render(GLenum texture_unit, GLuint texture);
};

#endif