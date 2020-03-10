/** \file simspot.cpp
 * \author Andrej Leban
 * \date 3/2020
 */

#include "simspot.h"

namespace der
{

simSpot::simSpot(double p_S0, double p_t, double p_sigma, double p_r)
    : m_t(p_t)
    , m_sigma(p_sigma)
    , m_precalc(p_S0 * std::exp((p_r - 0.5 * p_sigma * p_sigma) * p_t))
{}

double simSpot::operator()() const
{
    return m_precalc * std::exp(m_sigma * std::sqrt(m_t) * normalDist<double>());
}

simSpotParams::simSpotParams(double p_S0, double p_t, const der::Parameters & p_sigma, const der::Parameters & p_r)
    : m_t(p_t)
    , m_sigma(p_sigma)
    , m_precalc(p_S0 * std::exp(p_r.integral(0, p_t) - 0.5 * p_sigma.integralSquare(0, p_t)))
{}

double simSpotParams::operator()() const
{
    // need root variance, sqrt(t) already factored in
    return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * normalDist<double>());
}

} // namespace der
