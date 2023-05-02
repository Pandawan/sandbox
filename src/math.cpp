#include <cstdlib>
#include <random>

bool coinflip() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    return distrib(gen) == 0; 
}

double get_random_value(double lower_lim, double upper_lim)
{
    return lower_lim + static_cast<double> (rand() / static_cast<double> (RAND_MAX / (upper_lim - lower_lim)));
}
