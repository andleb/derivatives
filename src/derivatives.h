/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 *
 * The common library to the derivatives project
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <numeric>
#include <random>

namespace der
{

template <typename T>
T normalDistImpl(size_t p_seed = std::numeric_limits<size_t>::max())
{
    thread_local std::random_device rDev;
    if (p_seed == std::numeric_limits<size_t>::max())
    {
        // The generator is seeded from the implementation-defined device - this should be /dev/random
        p_seed = rDev();
    }
    thread_local std::mt19937_64 rng{p_seed};
    thread_local std::normal_distribution<T> Ndist;

    return Ndist(rng);
}

//! \brief Returns a normally distributed number of type \p T.
//! \return
template <typename T>
T normalDist()
{
    return normalDistImpl<T>();
}

//! \brief Returns a normally distributed number of type \p T, seeded once with p_seed.
//! \return
template <typename T>
T normalDist(size_t p_seed)
{
    return normalDistImpl<T>(p_seed);
}

} // namespace der

#endif // DERIVATIVES_H
