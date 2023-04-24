#include <iostream>
#include <stdexcept>
#include <string>

#include "gl.h"

#include "callbacks.h"
#include "window.h"
#include "grid.h"
#include "render/quad.h"
#include "render/shader.h"

int WINDOW_WIDTH = 600;
int WINDOW_HEIGHT = 600;

int main(__unused int argc, __unused char* argv[])
{
    init_glfw();

    GLFWwindow* window = create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Sandbox");
    
    /* Tether callback functions here. */
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)glfw_mouse_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);

    Grid grid = Grid(100, 100);
    Grid ui_grid = Grid(100, 100);
    Quad quad = Quad(window, GL_TEXTURE0, grid.get_texture());
    Quad ui_quad = Quad(window, GL_TEXTURE1, ui_grid.get_texture());
    double last_frame = 0;
    ui_partition(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        double current_frame = glfwGetTime();
        double delta_time = current_frame - last_frame;
        last_frame = current_frame;

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update grid simulation
        if (!display_ui())
        {
            grid.update(delta_time);
            // Render quad with updated texture
            quad.render();
        }
        else
        {
            ui_grid.update(delta_time);
            ui_quad.render();
        }

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
