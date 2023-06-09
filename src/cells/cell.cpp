#include "cell.h"

Cell::Cell()
    : name("void"),
      color(0),
      behavior(CellBehavior::NONE),
      mass(0),
      acceleration(0),
      velocity(0),
      velocity_delay(1),
      lifetime(0),
      is_combustible(false),
      is_wet(false),
      deathrattle(&Cell::Empty),
      spread_chance(0)
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
    velocity(0),
    velocity_delay(1),
    lifetime(0),
    is_combustible(false),
    is_wet(false),
    deathrattle(&Cell::Empty),
    spread_chance(0)
{
}

Cell::Cell(const Cell& obj) 
    : 
      name(obj.name),
      color(obj.color),
      behavior(obj.behavior),
      mass(obj.mass),
      acceleration(obj.acceleration),
      velocity(obj.velocity),
      velocity_delay(obj.velocity_delay),
      lifetime(obj.lifetime),
      is_combustible(obj.is_combustible),
      is_wet(obj.is_wet),
      deathrattle(obj.deathrattle),
      spread_chance(obj.spread_chance)
{
}

Cell::~Cell()
{
}

bool Cell::is_empty() const {
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

Cell Cell::Wet_Sand() {
    std::string name = "wet sand";
    glm::u8vec3 color(173, 162, 142);
    CellBehavior behavior = CellBehavior::MOVABLE_SOLID;
    double mass = 1520;

    return Cell(name, color, behavior, mass);
}


Cell Cell::Water() {
    std::string name = "water";
    glm::u8vec3 color(116, 204, 244);
    CellBehavior behavior = CellBehavior::LIQUID;
    double mass = 1000;
    Cell cell = Cell(name, color, behavior, mass);
    cell.velocity_delay = 0.15;

    return cell;
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
    Cell cell = Cell(name, color, behavior, mass);
    cell.is_combustible = true;

    return cell;
}

Cell Cell::Wet_Wood() {
    std::string name = "wet wood";
    glm::u8vec3 color(85, 60, 42);
    CellBehavior behavior = CellBehavior::IMMOVABLE_SOLID;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Fire() {
    std::string name = "fire";
    glm::u8vec3 color(255, 69, 0);
    CellBehavior behavior = CellBehavior::PLASMA;
    double mass = 0; 
    Cell cell = Cell(name, color, behavior, mass);
    cell.lifetime = get_random_value(0.1, 0.2);
    cell.spread_chance = 0.7;

    return cell;
}

Cell Cell::Smoke() {
    std::string name = "smoke";
    glm::u8vec3 color(122, 122, 122);
    CellBehavior behavior = CellBehavior::GAS;
    double mass = 0;
    Cell cell = Cell(name, color, behavior, mass);
    cell.lifetime = get_random_value(1.25, 1.75);
    cell.velocity_delay = 0.85;

    return cell;
}

Cell Cell::Lava() {
    std::string name = "lava";
    glm::u8vec3 color(207, 16, 32);
    CellBehavior behavior = CellBehavior::MOVABLE_SOLID;
    double mass = 0;
    Cell cell = Cell(name, color, behavior, mass);
    cell.deathrattle = &Cell::Obsidian;
    
    return cell;
}

Cell Cell::Grass() {
    std::string name = "grass";
    glm::u8vec3 color(124, 252, 0);
    CellBehavior behavior = CellBehavior::MOVABLE_SOLID;
    double mass = 0;
    Cell cell = Cell(name, color, behavior, mass);
    cell.is_combustible = true;

    return cell;
}

Cell Cell::Wet_Grass() {
    std::string name = "wet grass";
    glm::u8vec3 color(119, 187, 46);
    CellBehavior behavior = CellBehavior::MOVABLE_SOLID;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Bee() {
    std::string name = "BEES";
    glm::u8vec3 color(255, 255, 0);
    CellBehavior behavior = CellBehavior::BEE;
    double mass = 0;
    Cell cell = Cell(name, color, behavior, mass);
    cell.is_combustible = true;

    return cell;
}

Cell Cell::Wet_Bee() {
    std::string name = "Wet BEES";
    glm::u8vec3 color(246, 190, 0);
    CellBehavior behavior = CellBehavior::BEE;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}

Cell Cell::Obsidian() {
    std::string name = "obsidian";
    glm::u8vec3 color(58, 50, 45);
    CellBehavior behavior = CellBehavior::IMMOVABLE_SOLID;
    double mass = 0;

    return Cell(name, color, behavior, mass);
}