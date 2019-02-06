/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <random>

static std::random_device rDev;
// The generator is seeded from the implementation-defined device - this should be /dev/random
static std::mt19937_64 rng;
static std::normal_distribution<double> Ndist;

struct simSpot
{
    simSpot(double p_S0, double p_t, double p_sigma, double p_r)
        : m_t(p_t), m_sigma(p_sigma)
        , m_precalc(p_S0 * std::exp( (p_r - 0.5*p_sigma*p_sigma)*p_t))
    {}

    double operator() () const
    {
        return m_precalc * std::exp( m_sigma * std::sqrt(m_t) * N() );
    }

    static double N () { return Ndist(rng); }

    double m_t;
    double m_sigma;
    double m_precalc;
};


#endif // DERIVATIVES_H
