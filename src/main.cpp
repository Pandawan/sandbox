#include <iostream>

#include <glad/gl.h>
#include <glfwpp/glfwpp.h>

// Size of buffer/window
#define WIDTH 800
#define HEIGHT 600

int main(__unused int argc, __unused char* argv[])
{
    glfw::GlfwLibrary library = glfw::init();

    glfw::WindowHints hints;
    hints.clientApi = glfw::ClientApi::OpenGl;
    hints.contextVersionMajor = 3;
    hints.contextVersionMinor = 3;
    hints.openglProfile = glfw::OpenGlProfile::Core;
    hints.openglForwardCompat = true;
    hints.apply();
    
    glfw::Window window(WIDTH, HEIGHT, "GLFWPP basic example");

    glfw::makeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    while(!window.shouldClose())
    {
        double time = glfw::getTime();
        glClearColor((sin(time) + 1.0) / 2.0, (cos(time) + 1.0) / 2.0, (-sin(time) + 1.0) / 2.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfw::pollEvents();
        window.swapBuffers();
    }

    return 0;
}