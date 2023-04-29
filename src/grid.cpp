#include "grid.h"
#include "gl.h"
#include <iostream>

/*
    RESOURCES
    - Texture Units https://stackoverflow.com/questions/57621863/opengl-some-clarification-on-using-multiple-textures-or-texture-units
    - Active vs Bind https://stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture

*/

void init_texture(GLuint* texture)
{
    glGenTextures(1, &*texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
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

Grid::Grid(std::size_t width, std::size_t height) {
    this->width = width;
    this->height = height;

    // Initialize cell grid
    this->grid = new Cell[height * width];
    // TODO: Initialize with EMPTY cells

    // Initialize texture
    init_texture(&this->texture);
}

Grid::~Grid() {
    glDeleteTextures(1, &this->texture);
}

void Grid::set_cell(glm::ivec2 position, Cell value) {
    grid[(position.y * width) + position.x] = value;
}

Cell& Grid::get_cell(glm::ivec2 position) {
    return grid[(position.y * width) + position.x];
}

void Grid::clear() {
    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            grid[(y * width) + x] = Cell();
        }
    }
}

GLuint Grid::get_texture() {
    return this->texture;
}

void Grid::generate_texture() {
    // TODO: May be better to allocate one at the start and keep on modifying it
    float* data = new float[(this->height * this->width) * 3];
    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            Cell cell = get_cell(glm::ivec2(x, y));
            data[((y * this->width) + x) * 3] = cell.color.r;
            data[((y * this->width) + x) * 3 + 1] = cell.color.g;
            data[((y * this->width) + x) * 3 + 2] = cell.color.b;
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clear the working pixel buffer
    delete[] data;
}

// TODO: I would have thought this would be a separate component that makes use
// of the grid so that the grid can be used for the UI without special-casing
void Grid::update_grid(__unused double delta_time)
{
    double elapsed_time = 0.0;
    elapsed_time += delta_time;
    //std::cout << delta_time << std::endl;
    //std::cout << delta_time << std::endl;

    if (elapsed_time >= (1 / 30)) {
        for (std::size_t y = 0; y < this->height; ++y) {
            for (std::size_t x = 0; x < this->width; ++x) {
                glm::ivec2 idx = glm::ivec2(x, y);
                Cell cell = get_cell(idx);
                switch (cell.cell_type) {
                    case (SAND):
                        move_logic_solid(x, y, delta_time);
                        break;
                    case (WATER):
                        move_logic_liquid(x, y, delta_time);
                        break;
                    default:
                        break;
                }
            }
        }
    }   
}

bool Grid::move(const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y) {

    if (new_x < 0  || new_x > this->width - 1 || new_y < 0 || new_y > this->height - 1) 
        return false;

    Cell cell = get_cell(glm::ivec2(old_x, old_y));
    Cell neighbor = get_cell(glm::ivec2(new_x, new_y));

    if (neighbor.cell_type == EMPTY) {
        neighbor.cell_type = cell.cell_type;
        neighbor.set_color();

        cell.cell_type = EMPTY;
        cell.set_color();

        set_cell(glm::ivec2(new_x, new_y), neighbor);
        set_cell(glm::ivec2(old_x, old_y), cell);
        return true;
    }
    return false;
}

bool Grid::move_left(const size_t &x, const size_t &y) {
    return move(x, y, x - 1, y);
}

bool Grid::move_right(const size_t &x, const size_t &y) {
    return move(x, y, x + 1, y);
}

bool Grid::move_down(const size_t &x, const size_t &y) {
    return move(x, y, x, y - 1);
}

bool Grid::move_up(const size_t &x, const size_t &y) {
    return move(x, y, x, y + 1);
}

bool Grid::move_down_left(const size_t &x, const size_t &y) {
    return move(x, y, x - 1, y - 1);
}

bool Grid::move_down_right(const size_t &x, const size_t &y) {
    return move(x, y, x + 1, y - 1);
}

bool Grid::move_up_left(const size_t &x, const size_t &y) {
    return move(x, y, x - 1, y + 1);
}

bool Grid::move_up_right(const size_t &x, const size_t &y) {
    return move(x, y, x + 1, y + 1);
}

bool Grid::swap_cell(const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y) {
    if (new_x < 0  || new_x > this->width - 1 || new_y < 0 || new_y > this->height - 1) 
        return false;

    Cell cell = get_cell(glm::ivec2(old_x, old_y));
    Cell neighbor = get_cell(glm::ivec2(new_x, new_y)); 
    Cell temp = get_cell(glm::ivec2(new_x, new_y));  // temp = neighbor

    neighbor.cell_type = cell.cell_type;
    neighbor.cell_state = cell.cell_state;

    cell.cell_type = temp.cell_type;
    cell.cell_state = temp.cell_state;

    neighbor.set_color();
    cell.set_color();

    set_cell(glm::ivec2(new_x, new_y), neighbor);
    set_cell(glm::ivec2(old_x, old_y), cell);

    if (neighbor.cell_state == liquid)
        return move_logic_liquid(old_x, old_y, 1/60.0);
    else if (neighbor.cell_state == movable_solid)
        return move_logic_solid(old_x, old_y, 1/60.0);
    else if (neighbor.cell_state == gas)
        //TODO: return move_logic_gas(old_x, old_y);
        return true;
    else 
        return false;
}

bool Grid::move_logic_solid(const size_t &cur_x, const size_t &cur_y, const double &delta_time) {
    Cell cell = get_cell(glm::ivec2(cur_x, cur_y));
    Cell neighbor_below = get_cell(glm::ivec2(cur_x, cur_y - 1));
    
    const size_t ARBITRARY_SPEED_CONSTANT = 10;
    cell.velocity.y = sqrt(2 * GRAVITY * cur_y);
    unsigned int amount_down = static_cast<unsigned int>(round(cell.velocity.y * delta_time * ARBITRARY_SPEED_CONSTANT));

    std::cout << amount_down << std::endl;
    if (neighbor_below.cell_type == EMPTY) {
        for (unsigned int i = 0; i < amount_down; ++i)
            move_down(cur_x, cur_y);
        return true;
    } else if (neighbor_below.cell_state == liquid) {
        return swap_cell(cur_x, cur_y, cur_x, cur_y - 1);
    } else if (neighbor_below.cell_state == immovable_solid ||
        neighbor_below.cell_state == movable_solid) {
        if (!move_down_left(cur_x, cur_y))
            return move_down_right(cur_x, cur_y);
    }
    return false;
}

bool Grid::move_logic_liquid(const size_t &cur_x, const size_t &cur_y, const double &delta_time) {
    // Todo: Add liquid interaction with other state neighbor
    return flow_down(cur_x, cur_y, delta_time);
}

bool Grid::flow_down(const size_t &cur_x, const size_t &cur_y, const double &delta_time) {
    Cell cell = get_cell(glm::ivec2(cur_x, cur_y));
    Cell neighbor_down = get_cell(glm::ivec2(cur_x, cur_y - 1));
    Cell neighbor_left = get_cell(glm::ivec2(cur_x - 1, cur_y));
    Cell neighbor_right = get_cell(glm::ivec2(cur_x + 1, cur_y));
    Cell neighbor_down_left = get_cell(glm::ivec2(cur_x - 1, cur_y - 1));
    Cell neighbor_down_right = get_cell(glm::ivec2(cur_x + 1, cur_y - 1));

    const size_t ARBITRARY_SPEED_CONSTANT = 10;
    cell.velocity.y = sqrt(2 * GRAVITY * cur_y);
    unsigned int amount_down = static_cast<unsigned int>(round(cell.velocity.y * delta_time * ARBITRARY_SPEED_CONSTANT));

    if (neighbor_down.cell_type == EMPTY) {
        for (unsigned int i = 0; i < amount_down; ++i)
            move_down(cur_x, cur_y);
        return true;
    } else if (neighbor_down_left.cell_type == EMPTY && neighbor_down_right.cell_type != EMPTY) {
        return move_down_left(cur_x, cur_y);
    } else if (neighbor_down_left.cell_type != EMPTY && neighbor_down_right.cell_type == EMPTY) {
        return move_down_right(cur_x, cur_y);
    } else if (neighbor_down_left.cell_type == EMPTY && neighbor_down_right.cell_type == EMPTY) {
        bool left_or_right = std::rand() % 2 == 0;
        return left_or_right ? move_down_left(cur_x, cur_y) : move_down_right(cur_x, cur_y);
    } else if (neighbor_left.cell_type == EMPTY && neighbor_right.cell_type != EMPTY) {
        return move_left(cur_x, cur_y);
    } else if (neighbor_left.cell_type != EMPTY && neighbor_right.cell_type == EMPTY) {
        return move_right(cur_x, cur_y);
    } else if (neighbor_left.cell_type == EMPTY && neighbor_right.cell_type == EMPTY) {
        bool left_or_right = std::rand() % 2 == 0;
        return left_or_right ? move_left(cur_x, cur_y) : move_right(cur_x, cur_y);
    }
    return false;
}

void Grid::update(__unused double delta_time) {
    // We update the position of the Cells
    update_grid(delta_time); // TODO: Make this take delta_time into account
    generate_texture();
}

// TODO: If we actually used delta_time then we could just have a special case 
// of delta_time == 0 -> don't update_grid and then we don't need a special 
// function anymore
void Grid::ui_update()
{
    generate_texture();
}