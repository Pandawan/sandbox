#include <iostream>
#include <stdexcept>
#include <string>
#include <ctime>

#include "gl.h"
#include "callbacks.h"
#include "window.h"
#include "game.h"

int WINDOW_WIDTH = 600;
int WINDOW_HEIGHT = 600;
int GRID_WIDTH = 100;
int GRID_HEIGHT = 100;

int main(__unused int argc, __unused char* argv[])
{
    srand(time(0));
    init_glfw();

    GLFWwindow* window = create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Sandbox");
    
    // Tether callback functions
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)glfw_mouse_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun)glfw_key_callback);

    // Setup game
    Game game = Game(window, GRID_WIDTH, GRID_HEIGHT);
    
    // Engine loop
    double last_frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        double current_frame = glfwGetTime();
        double delta_time = current_frame - last_frame;
        last_frame = current_frame;

        game.update(delta_time);

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
