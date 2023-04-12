#include <iostream>
#include <stdexcept>
#include <string>

#include "gl.h"

#include "callbacks.h"
#include "window.h"
#include "grid.h"
#include "render/quad.h"
#include "render/shader.h"

int main(__unused int argc, __unused char* argv[])
{
    init_glfw();

    GLFWwindow* window = create_window(800, 600, "Sandbox");
    
    /* Tether callback functions here. */
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)glfw_mouse_callback);

    Grid grid = Grid(100, 100);
    Quad quad = Quad();

    // Generate a texture from the grid
    GLuint texture = grid.generate_texture();
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render that texture onto a quad
        quad.render(GL_TEXTURE0, texture);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
