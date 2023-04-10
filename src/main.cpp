#include <iostream>
#include <stdexcept>
#include <string>

#include "gl.h"

#include "callbacks.h"
#include "window.h"

int main(__unused int argc, __unused char* argv[])
{
    init_glfw();

    GLFWwindow* window = create_window(800, 600, "Sandbox");

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}