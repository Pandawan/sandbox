/**
 * This is the base class that all other subclasses will inherit from. 
 */
#ifndef PIXEL_H
#define PIXEL_H

enum class Color {RED, GREEN, BLUE};

class Pixel
{
    public:
        virtual void update() = 0;
        virtual ~Pixel() {};
        double xpos, ypos;
        Color color;
};

#endif