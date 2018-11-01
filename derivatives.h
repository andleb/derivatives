/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <random>

extern std::random_device rDev;
// The generator is seeded from the implementation-defined device - this should be /dev/random
extern std::mt19937_64 rng;
//need to put the generator as extern also to ensure correct init, since it depends ond the above
extern std::normal_distribution<double> Ndist;



#endif // DERIVATIVES_H
