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

    GLFWwindow* window = create_window(600, 600, "Sandbox");
    
    /* Tether callback functions here. */
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)glfw_mouse_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);

    Grid grid = Grid(100, 100);
    Quad quad = Quad(window, GL_TEXTURE0, grid.get_texture());

    double last_frame = 0;

    while (!glfwWindowShouldClose(window))
    {
        double current_frame = glfwGetTime();
        double delta_time = current_frame - last_frame;
        last_frame = current_frame;

//        std::cout << "Delta Time: " << delta_time << std::endl;

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update grid simulation
        grid.update(delta_time);
        // Render quad with updated texture
        quad.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
        toggle_cell(&grid);
        check_mouse_down(window, &grid, 600, 600);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
