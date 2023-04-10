#include "test_pixel.h"
#include <iostream>

TestPixel::TestPixel(double xpos, double ypos) {
    this->xpos = xpos;
    this->ypos = ypos;
    this->color = Color::RED;
}

TestPixel::~TestPixel() {

}

void TestPixel::update() {
}