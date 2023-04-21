/**
 * This is the base class that all other subclasses will inherit from. 
 */
#ifndef PIXEL_H
#define PIXEL_H

#include <vector>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

enum CellType 
{
    EMPTY,
    SAND,
    WATER
};

class Pixel
{
    public:
        Pixel() 
        {
            cell_type = EMPTY;
            velocity = glm::dvec2(0, 0);
        };
        ~Pixel() 
        {
        };
        bool occupied = false;
        glm::dvec2 velocity;
        glm::dvec4 RGBA;
        CellType cell_type;

        /* Given this pixel's cell type, we'll set the corresponding RGBA values*/
        void configure_rgba() 
        {
            switch(cell_type)
            {
                case (EMPTY):
                    RGBA.r = 0;
                    RGBA.g = 0;
                    RGBA.b = 0;
                    RGBA.a = 0;
                    break;
                case (SAND):
                    RGBA.r = 0.76;
                    RGBA.g = 0.68;
                    RGBA.b = 0.50;
                    RGBA.a = 0;
                    break;
                case (WATER):
                    RGBA.r = 0.51;
                    RGBA.g = 0.84;
                    RGBA.b = 0.93;
                    RGBA.a = 0; // For now, water is opaque
                    break;
                default:
                    break;
            }
        }
};

#endif