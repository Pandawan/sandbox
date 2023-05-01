#ifndef MATH_H
#define MATH_H

#include <glm/vec2.hpp>

namespace dir {
    const glm::uvec2 down = glm::uvec2(0, -1);
    const glm::uvec2 up = glm::uvec2(0, 1);
    const glm::uvec2 left = glm::uvec2(-1, 0);
    const glm::uvec2 right = glm::uvec2(1, 0);  
}

bool coinflip();

/** 
 * Gets a random value between a lower and upper limit
 * @returns a random value between a lower
 */
double get_random_value(double lower_lim, double upper_lim);


#endif