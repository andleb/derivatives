/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <random>

#include "parameters.h"

static std::random_device rDev;
// The generator is seeded from the implementation-defined device - this should be /dev/random
static std::mt19937_64 rng;
static std::normal_distribution<double> Ndist;

namespace der
{

struct simSpot
{
    simSpot(double p_S0, double p_t, double p_sigma, double p_r)
        : m_t(p_t)
        , m_sigma(p_sigma)
        , m_precalc(p_S0 * std::exp((p_r - 0.5 * p_sigma * p_sigma) * p_t))
    {}

    double operator()() const
    {
        return m_precalc * std::exp(m_sigma * std::sqrt(m_t) * N());
    }

    static double N() { return Ndist(rng); }

    double m_t;
    double m_sigma;
    double m_precalc;
};


//! \brief The simSpotParams struct
//! Allows for variable volatilities & interest rates
struct simSpotParams
{
    simSpotParams(double p_S0, double p_t, Parameters p_sigma, Parameters p_r)
        : m_t(p_t)
        , m_sigma(p_sigma)
        , m_precalc(p_S0 * std::exp(p_r.integral(0, p_t) - 0.5 * p_sigma.integralSquare(0, p_t)))
    {}

    double operator()() const
    {
                                                // need root variance, sqrt(t) already factored in
        return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * N());
    }

    static double N() { return Ndist(rng); }

    double m_t;
    Parameters m_sigma;
    double m_precalc;
};


} // namespace der
#endif // DERIVATIVES_H
