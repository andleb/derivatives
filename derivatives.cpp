/** \file derivatives.cpp
 * \author Andrej Leban
 * \date 3/2019
 */

#include "derivatives.h"

double der::normalDist()
{
    static std::random_device rDev;
    // The generator is seeded from the implementation-defined device - this should be /dev/random
    static std::mt19937_64 rng {rDev()};
    static std::normal_distribution<double> Ndist;

    return Ndist(rng);
}
