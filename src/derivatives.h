/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 *
 * The common library to the derivatives project
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <random>

namespace der
{

template <typename T>
T normalDist()
{
    static std::random_device rDev;
    // The generator is seeded from the implementation-defined device - this should be /dev/random
    static std::mt19937_64 rng{rDev()};
    static std::normal_distribution<T> Ndist;

    return Ndist(rng);
}

} // namespace der
#endif // DERIVATIVES_H
