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

GLuint Grid::get_texture() {
    return this->texture;
}

void Grid::generate_texture() {
    // TODO: May be better to allocate one at the start and keep on modifying it
    float* data = new float[(this->height * this->width) * 3];

    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            Cell cell = get_cell(glm::ivec2(x, y));
            data[((y * this->width) + x) * 3] = cell.RGBA.r;
            data[((y * this->width) + x) * 3 + 1] = cell.RGBA.g;
            data[((y * this->width) + x) * 3 + 2] = cell.RGBA.b;
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clear the working pixel buffer
    delete[] data;
}

// TODO: make the switch case neater cause it's pretty fugly rn
void Grid::update_grid()
{
    for (std::size_t y = 0; y < this->height; ++y) {
        for (std::size_t x = 0; x < this->width; ++x) {
            glm::ivec2 idx = glm::ivec2(x, y);
            Cell cell = get_cell(idx);
            switch (cell.cell_type) {
                case (SAND):
                    move_logic_solid(cell, x, y);
                    break;
                case (WATER):
                    move_logic_liquid(cell, x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

bool Grid::move(Cell cell,const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y) {

    if (new_x < 0  || new_x > this->width - 1 || new_y < 0 || new_y > this->height - 1) 
        return false;

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

bool Grid::move_left(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x - 1, y);
}

bool Grid::move_right(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x + 1, y);
}

bool Grid::move_down(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x, y - 1);
}

bool Grid::move_up(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x, y + 1);
}

bool Grid::move_down_left(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x - 1, y - 1);
}

bool Grid::move_down_right(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x + 1, y - 1);
}

bool Grid::move_up_left(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x - 1, y + 1);
}

bool Grid::move_up_right(Cell cell, const size_t &x, const size_t &y) {
    return move(cell, x, y, x + 1, y + 1);
}

bool Grid::swap_cell(Cell cell,const size_t &old_x, const size_t &old_y, const size_t &new_x, const size_t &new_y) {
    if (new_x < 0  || new_x > this->width - 1 || new_y < 0 || new_y > this->height - 1) 
        return false;

    Cell neighbor = get_cell(glm::ivec2(new_x, new_y));
    
    Cell temp = get_cell(glm::ivec2(new_x, new_y));

    neighbor.cell_type = cell.cell_type;
    neighbor.set_color();
    set_cell(glm::ivec2(new_x, new_y), neighbor);

    cell = temp;
    cell.cell_type = temp.cell_type;
    cell.set_color();
    set_cell(glm::ivec2(old_x, old_y), cell);
    
    return true;
}

bool Grid::move_logic_solid(Cell cell, const size_t &cur_x, const size_t &cur_y) {
    Cell neighbor_below = get_cell(glm::ivec2(cur_x, cur_y - 1));

    if (neighbor_below.cell_type == EMPTY) {
        return move_down(cell, cur_x, cur_y);
    } else if (neighbor_below.cell_state == liquid) {
        return swap_cell(cell, cur_x, cur_y, cur_x, cur_y-1);
    } else if (neighbor_below.cell_state == immovable_solid ||
        neighbor_below.cell_state == movable_solid) {
        if (!move_down_left(cell, cur_x, cur_y))
            return move_down_right(cell, cur_x, cur_y);
    }
    return false;
}

bool Grid::move_logic_liquid(Cell cell, const size_t &cur_x, const size_t &cur_y) {
    return flow_down(cell, cur_x, cur_y);
}

bool Grid::flow_down(Cell cell, const size_t &cur_x, const size_t &cur_y) {

    Cell neighbor_down = get_cell(glm::ivec2(cur_x, cur_y - 1));
    Cell neighbor_left = get_cell(glm::ivec2(cur_x - 1, cur_y));
    Cell neighbor_right = get_cell(glm::ivec2(cur_x + 1, cur_y));
    Cell neighbor_down_left = get_cell(glm::ivec2(cur_x - 1, cur_y - 1));
    Cell neighbor_down_right = get_cell(glm::ivec2(cur_x + 1, cur_y - 1));


    if (neighbor_down.cell_type == EMPTY) {
        return move_down(cell, cur_x, cur_y);
    } else if (neighbor_down_left.cell_type == EMPTY && neighbor_down_right.cell_type != EMPTY) {
        return move_down_left(cell, cur_x, cur_y);
    } else if (neighbor_down_left.cell_type != EMPTY && neighbor_down_right.cell_type == EMPTY) {
        return move_down_right(cell, cur_x, cur_y);
    } else if (neighbor_down_left.cell_type == EMPTY && neighbor_down_right.cell_type == EMPTY) {
        bool left_or_right = rand() % 2;
        return left_or_right ? move_down_left(cell, cur_x, cur_y) : move_down_right(cell, cur_x, cur_y);
    } else if (neighbor_left.cell_type == EMPTY && neighbor_right.cell_type != EMPTY) {
        return move_left(cell, cur_x, cur_y);
    } else if (neighbor_left.cell_type != EMPTY && neighbor_right.cell_type == EMPTY) {
        return move_right(cell, cur_x, cur_y);
    } else if (neighbor_left.cell_type == EMPTY && neighbor_right.cell_type == EMPTY) {
        bool left_or_right = rand() % 2;
        return left_or_right ? move_left(cell, cur_x, cur_y) : move_right(cell, cur_x, cur_y);
    }

    return false;
}

void Grid::update(__unused double delta_time) {
    // We update the position of the Cells
    update_grid();
    generate_texture();
}
