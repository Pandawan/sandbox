# Sandbox

## Development

### Prerequisites

You will need `cmake` v3.14 (or above), `make`, and a C++ compiler.

### Setup

We use cmake to setup the project environment for the appropriate platform.

```sh
# Create a build folder at the root of the repo
mkdir build
cd build

# Setup the cmake files
cmake ..
```

Once you have the build folder setup, run `make` to build the project.

You can then run the program `sandbox` inside the build folder.

### Adding cpp files

When adding new C++ files that you want included in the project,
you need to add them to the CMakeLists.txt file found inside the `src` folder.

Just add the file to the `target_sources` list on a new line like so:

```cmake
target_sources(${PROJECT_NAME} PRIVATE 
    main.cpp
    my_new_file.cpp # added here
)
```

## Libraries

- [GLFW](https://www.glfw.org/) for OpenGL rendering
- [GLAD 2](https://github.com/Dav1dde/glad) for cross-platform OpenGL binding
