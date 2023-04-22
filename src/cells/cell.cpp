#include "cell.h"

Cell::Cell()
{
    cell_type = EMPTY;
    velocity = glm::dvec2(0, 0);
}

Cell::~Cell()
{

}

void Cell::configure_rgba()
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
        case (UI):
            RGBA.r = 0.33;
            RGBA.g = 0.33;
            RGBA.b = 0.33;
            RGBA.a = 0;
            break;
        default:
            break;
    }
}