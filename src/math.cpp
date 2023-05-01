#include <cstdlib>
#include <random>

bool coinflip() {
        std::random_device                    rand_dev;
        std::mt19937                          generator(rand_dev());
        std::uniform_int_distribution<int>    distr(0, 9);
        return distr(generator) % 2 == 0;
}

double get_random_value(double lower_lim, double upper_lim)
{
    return lower_lim + static_cast<double> (rand() / static_cast<double> (RAND_MAX / (upper_lim - lower_lim)));
}
