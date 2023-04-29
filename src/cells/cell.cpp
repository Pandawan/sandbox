#include "cell.h"

Cell::Cell()
{
    cell_type = EMPTY;
    cell_state = none;
    mass = 0;
    velocity = glm::dvec2(0, 0);
}

Cell::Cell(const Cell &obj)
{
    this->mass = obj.mass;
    this->velocity = obj.velocity;
    this->color = obj.color; 
    this->cell_type = obj.cell_type;
    this->cell_state = obj.cell_state;
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
            mass = 0;
            break;
        case (SAND):
            set_rgba(194, 178, 128, 0.5);
            cell_state = movable_solid;
            mass = 1520;
            break;
        case (WATER):
            set_rgba(116,204, 244, 0);
            cell_state = liquid;
            mass = 1000;

            break;
        case (WOOD):
            set_rgba(101, 56, 24, 0);
            cell_state = immovable_solid;
            mass = 0.7;
            break;
        case (UI):
            set_rgba(85.33, 85.33, 85.33, 0);
            break;
        default:
            break;
    }
}

void Cell::set_rgba(const float &r, const float &g, const float &b, const float &a) {
    color.r = r / 256;
    color.g = g / 256;
    color.b = b / 256;
    color.a = a / 256; 
}