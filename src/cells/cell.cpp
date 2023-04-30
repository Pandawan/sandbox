#include "cell.h"

Cell::Cell()
    : name("void"),
      color(0),
      behavior(CellBehavior::NONE),
      mass(0),
      acceleration(0),
      velocity(0)
{
}

Cell::Cell(
    std::string name,
    glm::u8vec3 color,
    CellBehavior behavior,
    double mass
) : name(name),
    color(color),
    behavior(behavior),
    mass(mass),
    acceleration(0),
    velocity(0)
{
}

Cell::Cell(const Cell& obj) 
    : color(obj.color),
      behavior(obj.behavior),
      mass(obj.mass),
      acceleration(obj.acceleration),
      velocity(obj.velocity)
{
}

Cell::~Cell()
{
}

bool Cell::is_empty() {
    return this->behavior == CellBehavior::NONE;
}

Cell Cell::Empty() {
    return Cell();
}

Cell Cell::Sand() {
    std::string name = "sand";
    glm::u8vec3 color(194, 178, 128);
    CellBehavior behavior = CellBehavior::MOVABLE_SOLID;
    double mass = 1520;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Water() {
    std::string name = "water";
    glm::u8vec3 color(116, 204, 244);
    CellBehavior behavior = CellBehavior::LIQUID;
    double mass = 1000;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Stone() {
    std::string name = "stone";
    glm::u8vec3 color(85, 85, 85);
    CellBehavior behavior = CellBehavior::IMMOVABLE_SOLID;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Wood() {
    std::string name = "wood";
    glm::u8vec3 color(101, 56, 24);
    CellBehavior behavior = CellBehavior::IMMOVABLE_SOLID;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}
