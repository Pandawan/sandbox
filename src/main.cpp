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
int GRID_WIDTH = 100;
int GRID_HEIGHT = 100;

int main(__unused int argc, __unused char* argv[])
{
    init_glfw();

    GLFWwindow* window = create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Sandbox");
    
    /* Tether callback functions here. */
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)glfw_mouse_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);

    Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
    Grid ui_grid = Grid(GRID_WIDTH, GRID_HEIGHT);
    Quad quad = Quad(window, GL_TEXTURE0, grid.get_texture());
    Quad ui_quad = Quad(window, GL_TEXTURE1, ui_grid.get_texture());
    ui_grid.paused = true;

    // We do this to configure the UI components.
    ui_partition(WINDOW_WIDTH, WINDOW_HEIGHT);
    int x = 0;
    int dx = GRID_WIDTH / 5;
    int dy = GRID_HEIGHT / 5;
    int y = GRID_HEIGHT - dy;
    Cell cell_types[] = { Cell::Empty(), Cell::Sand(), Cell::Water(), Cell::Stone(), Cell::Wood() };
    // TODO: WHY IS THAT HERE LOL
    // First, initialize the number of UI elements
    for (int i = 0; i != 5; ++i)
    {
        Cell cell_type = cell_types[i];
        // Forgive me, oh heavenly kernel above for this triple nested loop
        for (int j = 0; j < dy; ++j)
        {
            for (int k = 0; k < dx; ++k)
            {
                glm::uvec2 idx = glm::uvec2(x + k, j + y);
                ui_grid.set_cell(idx, cell_type);
            }
        }

        if (i % 5 == 0 && i != 0)
        {
            x = 0;
            y -= dy;
        }
        else 
        {
            x += dx;
        }
    }
    
    double last_frame = 0;
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
        check_mouse_down(window, &grid, GRID_HEIGHT);
        check_clear_pressed(&grid);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
