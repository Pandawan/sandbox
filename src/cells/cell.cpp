#include "cell.h"

Cell::Cell()
{
    cell_type = EMPTY;
    velocity = glm::dvec2(0, 0);
}

Cell::~Cell()
{

}

void Cell::set_color() // TODO: WTF IS THIS?
{
    switch(cell_type)
    {
        case (EMPTY):
            set_rgba(0, 0, 0, 0);
            cell_state = none;
            break;
        case (SAND):
            set_rgba(194, 178, 128, 0.5);
            cell_state = movable_solid;
            break;
        case (WATER):
            set_rgba(116,204, 244, 0);
            cell_state = liquid;
            break;
        case (WOOD):
            set_rgba(101, 56, 24, 0);
            cell_state = immovable_solid;
            break;
        case (UI):
            set_rgba(85.33, 85.33, 85.33, 0);
            break;
        default:
            break;
    }
}

void Cell::set_rgba(const float &r, const float &g, const float &b, const float &a) {
    RGBA.r = r / 256;
    RGBA.g = g / 256;
    RGBA.b = b / 256;
    RGBA.a = a / 256; 
}