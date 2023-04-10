/**
 * A dummy class that's going to be used to test rendering.
 */
#ifndef TEST_PIXEL_H
#define TEST_PIXEL_H

#include "pixel.h"

class TestPixel : public Pixel {
    public:
        void update();
        TestPixel(double xpos, double ypos);
        ~TestPixel();
};

#endif