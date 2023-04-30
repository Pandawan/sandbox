/**
 * This is the base class that all other subclasses will inherit from.
 */
#ifndef CELL_H
#define CELL_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>

enum CellBehavior
{
    NONE = 0,
    LIQUID,
    MOVABLE_SOLID,
    IMMOVABLE_SOLID,
    GAS
};

const double GRAVITY = 9.8f;

class Cell
{
public:
    /** Create an empty cell. */
    Cell();
    /** Create a cell with given properties. */
    Cell(std::string name, glm::u8vec3 color, CellBehavior behavior, double mass);
    /** Copy a cell from another. */
    Cell(const Cell& obj);
    ~Cell();

    // Cell Properties
    /** Name of the cell type. */
    std::string name;
    /** Color of the cell. */
    glm::u8vec3 color;
    /** Type of behavior to expect from this cell. */
    CellBehavior behavior; // TODO: Do we even need this?
    /** Mass of the cell for physics behavior. */
    double mass;

    // Runtime State
    glm::dvec2 acceleration;
    glm::dvec2 velocity;

    bool is_empty();

    // Presets
    static Cell Empty();
    static Cell Sand();
    static Cell Water();
    static Cell Stone();
    static Cell Wood();
};

#endif