#include "grid.h"
#include "gl.h"

#include <stdexcept>
#include <iostream>

#include "math.h"

/*
    RESOURCES
    - Texture Units https://stackoverflow.com/questions/57621863/opengl-some-clarification-on-using-multiple-textures-or-texture-units
    - Active vs Bind https://stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture

*/

Grid::Grid(std::size_t width, std::size_t height) 
    : width(width), height(height), dirty(true), paused(false)
{
    // Initialize cell grid
    this->grid = new Cell[height * width];

    // Initialize texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // Warp texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Use nearest neighbor (pixel-perfect) sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Don't allow mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

Grid::~Grid() {
    glDeleteTextures(1, &this->texture);
}

void Grid::set_cell(glm::uvec2 position, Cell value) {
    grid[(position.y * width) + position.x] = value;
    this->dirty = true;
}

Cell* Grid::get_cell(glm::uvec2 position) {
    if (
        position.x < 0 || position.x >= width || 
        position.y < 0 || position.y >= height
    ) {
        return nullptr;
    }

    return &grid[(position.y * width) + position.x];
}

void Grid::clear() {
    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            grid[(y * width) + x] = Cell();
        }
    }
}

GLuint Grid::get_texture() const {
    return this->texture;
}

void Grid::generate_texture() {
    // TODO: May be better to allocate one at the start and keep on modifying it
    unsigned char* data = new unsigned char[(this->height * this->width) * 3];
    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            Cell* cell = get_cell(glm::uvec2(x, y));
            data[((y * this->width) + x) * 3] = cell->color.r;
            data[((y * this->width) + x) * 3 + 1] = cell->color.g;
            data[((y * this->width) + x) * 3 + 2] = cell->color.b;
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clear the working pixel buffer
    delete[] data;
}

void Grid::update(double delta_time) {
    // TODO: Figure out why texture is lost when I put this->dirty == false -> return

    if (!this->paused) {// && this->dirty) {
        this->dirty = false; // Will be set to true by set_cell if ever called in simulate_grid

        // Update the position of the cells
        simulate_grid(delta_time);
    }
    // Generate a texture from the new cell positions
    generate_texture();
}

void Grid::simulate_grid(double delta_time)
{
    // TODO: Use delta_time
    cell_dirty = new bool[this->height * this->width]();

    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            glm::uvec2 pos = glm::uvec2(x, y);
            Cell* cell = this->get_cell(pos);
            // if this cell has been updated already, skip
            if(cell_dirty[(y * this->width) + x] == true)
                continue;

            switch (cell->behavior) {
                case CellBehavior::LIQUID:
                    this->simulate_liquid(pos, delta_time);
                    break;
                case CellBehavior::MOVABLE_SOLID:
                    this->simulate_solid(pos, delta_time);
                    break;
                case CellBehavior::IMMOVABLE_SOLID:
                    break;
                case CellBehavior::GAS:
                    this->simulate_gas(pos, delta_time);
                    break;
                case CellBehavior::PLASMA:
                    this->simulate_plasma(pos, delta_time);
                    break;
                case CellBehavior::BEE:
                    this->simulate_bee(pos, delta_time);
                    break;
                case CellBehavior::NONE:
                    break;
            }
        }
    }
}

void Grid::swap_cells(glm::uvec2 first, glm::uvec2 second) {
    // don't swap the dirty cell
    if (cell_dirty[(first.y * this->width) + first.x] == true || 
        cell_dirty[(second.y * this->width) + second.x] == true) {
            return;
    }

    Cell temp = *get_cell(second);
    set_cell(second, *get_cell(first));
    set_cell(first, temp);

    if (!get_cell(first)->is_empty())
        cell_dirty[(first.y * this->width) + first.x] = true;
    if (!get_cell(second)->is_empty())
        cell_dirty[(second.y * this->width) + second.x] = true;
}

void Grid::set_wet_cell(glm::uvec2 position) {
    Cell* cell = get_cell(position);

    if (cell->name == "sand") {
        set_cell(position, Cell::Wet_Sand());
    } else if (cell->name == "grass") {
        set_cell(position, Cell::Wet_Grass());
    } else if (cell->name == "wood") {
        set_cell(position, Cell::Wet_Wood());
    }
} 

bool Grid::simulate_bee(glm::uvec2 position, __unused double delta_time) {
    glm::uvec2 position_down = position + dir::down;
    glm::uvec2 position_down_left = position_down + dir::left;
    glm::uvec2 position_down_right = position_down + dir::right;
    glm::uvec2 position_left = position + dir::left;
    glm::uvec2 position_right = position + dir::right;
    glm::uvec2 position_up = position + dir::up;
    glm::uvec2 position_up_left = position_up + dir::left;
    glm::uvec2 position_up_right = position_up + dir::right;

    Cell* neighbor_down = get_cell(position_down);
    Cell* neighbor_down_left = get_cell(position_down_left);
    Cell* neighbor_down_right = get_cell(position_down_right);
    Cell* neighbor_left = get_cell(position_left);
    Cell* neighbor_right = get_cell(position_right);
    Cell* neighbor_up = get_cell(position_up);
    Cell* neighbor_up_left = get_cell(position_up_left);
    Cell* neighbor_up_right = get_cell(position_up_right);

    std::vector<Cell*>neighbors = {
        neighbor_down, 
        neighbor_down_left,
        neighbor_down_right,
        neighbor_left,
        neighbor_right,
        neighbor_up,
        neighbor_up_left,
        neighbor_up_right
    };

    std::vector<glm::uvec2>positions = {
        position_down,
        position_down_left,
        position_down_right,
        position_left,
        position_right,
        position_up,
        position_up_left,
        position_up_right
    };

    // Choose random cell/position
    int idx =  rand() % positions.size();
    bool move_coinflip = get_random_value(0, 1) < 0.2 ? true : false;

    if (neighbors[idx] != nullptr && neighbors[idx]->behavior == CellBehavior::NONE && move_coinflip) {
        swap_cells(positions[idx], position);
        return true;
    }
    return false;
}   

bool Grid::simulate_solid(glm::uvec2 position, __unused double delta_time) {
    Cell* cell = get_cell(position);
    assert(cell->behavior == CellBehavior::MOVABLE_SOLID);

    glm::uvec2 position_down = position + dir::down;
    Cell* neighbor_down = get_cell(position_down);

    // Out of bounds
    if (neighbor_down == nullptr) return false;
    
    //cell.velocity.y += sqrt(2 * GRAVITY * cur_y * this->height);
    //unsigned int amount_down = static_cast<unsigned int>(round(cell.velocity.y * delta_time));

    
    //unsigned int delta_position = static_cast<unsigned int>(round(cell.velocity.y * delta_time + 0.5 * cell.acceleration.y * delta_time * delta_time));
    
    // const double OFFSET = 0.47;
    // cell.acceleration.y = GRAVITY * cell.mass;
    // double delta_position = round(cell.velocity.y * delta_time + 0.5 * cell.acceleration.y * delta_time * delta_time + OFFSET);
    
    // cell.velocity.y  += cell.acceleration.y * delta_time;

    //std::cout << "Sand Veloctiy: " << cell.velocity.y << std::endl;
    //std::cout << "Sand delta_s: "<< delta_position << std::endl;

    // TODO: Make it so liquids & gases don't pop all the way to the top of a column
    // This likely means having additional checks where the liquid/gas doesn't just go up but rather goes left or right if possible?

    // Empty, liquid, or gaseous below
    if (
        neighbor_down->behavior == CellBehavior::NONE ||
        neighbor_down->behavior == CellBehavior::LIQUID || 
        neighbor_down->behavior == CellBehavior::GAS
    ) {
        // Just swap them
        swap_cells(position, position_down);
        return true;
    }
    // Solid below
    else if (
        neighbor_down->behavior == CellBehavior::IMMOVABLE_SOLID || 
        neighbor_down->behavior == CellBehavior::MOVABLE_SOLID
    ) {
        // Try with down left
        glm::uvec2 pos_down_left = position + dir::down + dir::left;
        Cell* neighbor_down_left = get_cell(pos_down_left);
        // TODO: Should this allow sand dropping into liquid from the side? If so, should probably add an abstraction like "try_move" or something?
        if (neighbor_down_left != nullptr && 
            (neighbor_down_left->behavior == CellBehavior::NONE ||
            neighbor_down_left->behavior == CellBehavior::LIQUID || 
            neighbor_down_left->behavior == CellBehavior::GAS)) {
            swap_cells(position, pos_down_left);
            return true;
        }
        else {
            // Try with down right
            glm::uvec2 pos_down_right = position + dir::down + dir::right;
            Cell* neighbor_down_right = get_cell(pos_down_right);
            if (neighbor_down_right != nullptr && 
                (neighbor_down_right->behavior == CellBehavior::NONE ||
                neighbor_down_right->behavior == CellBehavior::LIQUID || 
                neighbor_down_right->behavior == CellBehavior::GAS)) {
                swap_cells(position, pos_down_right);
                return true;
            }
        }
    }

    // If we haven't returned yet, then the cell didn't move.
    return false;
}

bool Grid::simulate_plasma(glm::uvec2 position, double delta_time)
{
    Cell* cell = get_cell(position);
    cell->lifetime -= delta_time;

    if (cell->lifetime <= 0) 
    {   
        // int biased_coin = round(get_random_value(0, 3));
        // if (biased_coin == 0) {
        //     set_cell(position, Cell::Smoke());
        //     get_cell(position)->lifetime = get_random_value(0.025, 0.05);
        // }
        // else
        //     set_cell(position, Cell::Empty());
        set_cell(position, cell->deathrattle());
        return false;
    }

    glm::uvec2 position_down = position + dir::down;
    glm::uvec2 position_down_left = position_down + dir::left;
    glm::uvec2 position_down_right = position_down + dir::right;
    glm::uvec2 position_left = position + dir::left;
    glm::uvec2 position_right = position + dir::right;
    glm::uvec2 position_up = position + dir::up;
    glm::uvec2 position_up_left = position_up + dir::left;
    glm::uvec2 position_up_right = position_up + dir::right;

    Cell* neighbor_down = get_cell(position_down);
    Cell* neighbor_down_left = get_cell(position_down_left);
    Cell* neighbor_down_right = get_cell(position_down_right);
    Cell* neighbor_left = get_cell(position_left);
    Cell* neighbor_right = get_cell(position_right);
    Cell* neighbor_up = get_cell(position_up);
    Cell* neighbor_up_left = get_cell(position_up_left);
    Cell* neighbor_up_right = get_cell(position_up_right);

    bool success = false;
    // We do a coinflip to randomize cell proliferation
    double coinflip;

    // Check below
    if (neighbor_down != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_down, position_down, cell->lifetime * cell->lifetime); 
    }

    // Check above
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_up != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_up, position_up, cell->lifetime * cell->lifetime);
    }

    // Check left
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_left != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_left, position_left, cell->lifetime * cell->lifetime);
    }

    // Check right
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_right != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_right, position_right, cell->lifetime * cell->lifetime);
    }
    
    // Check bottom left
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_down_left != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_down_left, position_down_left, cell->lifetime * cell->lifetime);
    }

    // Check bottom right
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_down_right != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_down_right, position_down_right, cell->lifetime * cell->lifetime);
    }

    // Check top left
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_up_left != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_up_left, position_up_left, cell->lifetime * cell->lifetime);
    }

    // Check top right
    coinflip = get_random_value(0, 1);
    if (coinflip < cell->spread_chance && neighbor_up_right != nullptr)
    {
        Cell spread = Cell::Fire();
        success = proliferate(&spread, neighbor_up_right, position_up_right, cell->lifetime * cell->lifetime);
    }

    return success;
}

bool Grid::simulate_gas(glm::uvec2 position, double delta_time)
{
    Cell* cell = get_cell(position);
    cell->lifetime -= delta_time;

    if (cell->lifetime <= 0) 
    {
        set_cell(position, Cell::Empty());
        return false;
    }

    glm::uvec2 position_up = position + dir::up;
    //Cell* neighbor_up = get_cell(position_up);

    Cell* neighbor_up = get_cell(position + dir::up);
    Cell* neighbor_left = get_cell(position + dir::left);
    Cell* neighbor_right = get_cell(position + dir::right);
    Cell* neighbor_up_left = get_cell(position + dir::up + dir::left);
    Cell* neighbor_up_right = get_cell(position + dir::up + dir::right);

    // Empty space above.
    if (neighbor_up != nullptr && neighbor_up->behavior == CellBehavior::NONE) 
    {
        swap_cells(position, position_up);
        return true;
    }
    // Above is occupied, try to move to left or right
    // up left only
    else if (
        neighbor_up_left != nullptr && neighbor_up_left->is_empty() 
        && (neighbor_up_right == nullptr || neighbor_up_right->behavior != CellBehavior::NONE)
    ) {
        swap_cells(position, position + dir::up + dir::left);
        return true;
    }
    // up right only
    else if (
        (neighbor_up_left == nullptr || neighbor_up_left->behavior != CellBehavior::NONE)
        && neighbor_up_right != nullptr && neighbor_up_right->is_empty()
    ) {
        swap_cells(position, position + dir::up + dir::right);
        return true;
    }
    // up left OR up right
    else if (
        (neighbor_up_left != nullptr && neighbor_up_left->is_empty())
        && neighbor_up_right != nullptr && neighbor_up_right->is_empty()
    ) {
        bool go_left = coinflip();
        if (go_left) { 
            swap_cells(position, position + dir::up + dir::left);
        } else {
            swap_cells(position, position + dir::up + dir::right);
        }
        return true;
    }
    // Left only
    else if (
        neighbor_left != nullptr && neighbor_left->is_empty() 
        && (neighbor_right == nullptr || neighbor_right->behavior != CellBehavior::NONE)
    ) {
        swap_cells(position, position + dir::left);
        return true;
    }
    // Right only
    else if (
        (neighbor_left == nullptr || neighbor_left->behavior != CellBehavior::NONE)
        && neighbor_right != nullptr && neighbor_right->is_empty()
    ) {
        swap_cells(position, position + dir::right);
        return true;
    }
    // Left OR Right
    else if (
        (neighbor_left != nullptr && neighbor_left->is_empty())
        && neighbor_right != nullptr && neighbor_right->is_empty()
    ) {
        bool go_left = coinflip();
        if (go_left) { 
            swap_cells(position, position + dir::left);
        } else {
            swap_cells(position, position+ dir::right);
        }
        return true;
    }

    return false;
}

bool Grid::simulate_liquid(glm::uvec2 position, __unused double delta_time) {

    Cell* neighbor_up = get_cell(position + dir::up);
    Cell* neighbor_down = get_cell(position + dir::down);
    Cell* neighbor_left = get_cell(position + dir::left);
    Cell* neighbor_right = get_cell(position + dir::right);
    Cell* neighbor_up_left = get_cell(position + dir::up + dir::left);
    Cell* neighbor_up_right = get_cell(position + dir::up + dir::right);
    Cell* neighbor_down_left = get_cell(position + dir::down + dir::left);
    Cell* neighbor_down_right = get_cell(position + dir::down + dir::right);
    
    // TODO: Refactor this, this isn't clean code

    // Get all your neighbors soaking wet
    if (neighbor_up != nullptr)
        set_wet_cell(position + dir::up);
    if (neighbor_up_left != nullptr)
        set_wet_cell(position + dir::up + dir::left);
    if (neighbor_up_right != nullptr)
        set_wet_cell(position + dir::up + dir::right);
    if (neighbor_down != nullptr)
        set_wet_cell(position + dir::down);
    if (neighbor_down_left != nullptr)
        set_wet_cell(position + dir::down + dir::left);
    if (neighbor_down_right != nullptr)
        set_wet_cell(position + dir::down + dir::right);
    if (neighbor_left != nullptr)
        set_wet_cell(position + dir::left);
    if (neighbor_right != nullptr)
        set_wet_cell(position + dir::right);

    // Down
    if (neighbor_down != nullptr && neighbor_down->is_empty()) {
        swap_cells(position, position + dir::down);
        return true;
    }
    // Down left only
    else if (
        neighbor_down_left != nullptr && neighbor_down_left->is_empty() 
        && (neighbor_down_right == nullptr || neighbor_down_right->behavior != CellBehavior::NONE)
    ) {
        swap_cells(position, position + dir::down + dir::left);
        return true;
    }
    // Down right only
    else if (
        (neighbor_down_left == nullptr || neighbor_down_left->behavior != CellBehavior::NONE)
        && neighbor_down_right != nullptr && neighbor_down_right->is_empty()
    ) {
        swap_cells(position, position + dir::down + dir::right);
        return true;
    }
    // Down left OR Down right
    else if (
        (neighbor_down_left != nullptr && neighbor_down_left->is_empty())
        && neighbor_down_right != nullptr && neighbor_down_right->is_empty()
    ) {
        bool go_left = coinflip();
        if (go_left) { 
            swap_cells(position, position + dir::down + dir::left);
        } else {
            swap_cells(position, position + dir::down + dir::right);
        }
        return true;
    }
    // Left only
    else if (
        neighbor_left != nullptr && neighbor_left->is_empty() 
        && (neighbor_right == nullptr || neighbor_right->behavior != CellBehavior::NONE)
    ) {
        swap_cells(position, position + dir::left);
        return true;
    }
    // Right only
    else if (
        (neighbor_left == nullptr || neighbor_left->behavior != CellBehavior::NONE)
        && neighbor_right != nullptr && neighbor_right->is_empty()
    ) {
        swap_cells(position, position + dir::right);
        return true;
    }
    // Left OR Right
    else if (
        (neighbor_left != nullptr && neighbor_left->is_empty())
        && neighbor_right != nullptr && neighbor_right->is_empty()
    ) {
        bool go_left = coinflip();
        if (go_left) { 
            swap_cells(position, position + dir::left);
        } else {
            swap_cells(position, position+ dir::right);
        }
        return true;
    }

    return false;
}

bool Grid::proliferate(Cell* spread, Cell* victim, glm::uvec2 victim_pos, double lifetime) {
    if (victim->behavior == CellBehavior::NONE)
    {   
        set_cell(victim_pos, *spread);
        get_cell(victim_pos)->lifetime = lifetime;
        return true;
    }
    else if (victim->is_combustible)
    {
        set_cell(victim_pos, *spread);
        get_cell(victim_pos)->deathrattle = &Cell::Smoke;
        return true;
    }
    return false;
}