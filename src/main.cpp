#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Size of buffer/window
#define WIDTH 800
#define HEIGHT 600

GLFWwindow *window = nullptr;

void error_callback(int error, const char* description)
{
    std::cerr << "Error " << error << ": " << description << std::endl;
}

void framebuffer_size_callback(__unused GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void createGLContexts()
{
    // Register error callback
    glfwSetErrorCallback(error_callback);

    // Initialize glfw
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwSetTime(0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

    glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set size of buffer & viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glfwSwapBuffers(window);

    // Register for window resizing event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
}

int main(__unused int argc, __unused char* argv[])
{
    createGLContexts();

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