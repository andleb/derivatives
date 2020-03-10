/** \file simspot.h
 * \author Andrej Leban
 * \date 3/2020
 *
 * Implements structures that simulate a spot price using a Black-Scholes process.
 */

#ifndef DER_SIMSPOT_H
#define DER_SIMSPOT_H

#include <algorithm>
#include <cmath>
#include <vector>

#include "derivatives.h"
#include "parameters.h"

namespace der
{

//! \brief The simSpot struct
//! Simulates the value of a spot price at time p_t for some volatility & interest rate
struct simSpot
{
    simSpot(double p_S0, double p_t, double p_sigma, double p_r);

    double operator()() const;

    double m_t;
    double m_sigma;
    double m_precalc;
};

//! \brief The simSpotParams struct
//! Allows for generic volatilities & interest rates
struct simSpotParams
{
    simSpotParams() = default;
    simSpotParams(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r);

    double operator()() const;

    double m_t{0.0};
    Parameters m_sigma;
    double m_precalc{0.0};
};


//! \brief The simSpotParamsMultiple struct
//! Allows a compile-time selection of a RNG, in addition to generic Parameters
template <typename Generator = std::nullptr_t>
struct simSpotParamsMultiple : public simSpotParams
{
    simSpotParamsMultiple() = default;

    simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r);
    simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r, size_t p_seed);
    simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r, Generator & p_generator);

    double operator()() const;

    std::vector<double> simSpotMultiple(size_t p_nValues);

    mutable Generator m_generator;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Generator>
simSpotParamsMultiple<Generator>::simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r)
    : simSpotParams(p_S0, p_t, p_sigma, p_r)
{}

template <typename Generator>
simSpotParamsMultiple<Generator>::simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r,
                                                        size_t p_seed)
    : simSpotParams(p_S0, p_t, p_sigma, p_r)
    , m_generator(p_seed)
{}

template <typename Generator>
simSpotParamsMultiple<Generator>::simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r,
                                                        Generator & p_generator)
    : simSpotParams(p_S0, p_t, p_sigma, p_r)
    , m_generator(p_generator)
{}

template <typename Generator>
double simSpotParamsMultiple<Generator>::operator()() const
{
    {
        // make use of custom generator
        if constexpr (std::is_same<Generator, std::nullptr_t>::value)
        {
            return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * normalDist<double>());
        }
        else
        {
            return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * m_generator.gaussians(std::vector<double>(1)));
        }
    }
}

template <typename Generator>
std::vector<double> simSpotParamsMultiple<Generator>::simSpotMultiple(size_t p_nValues)
{
    std::vector<double> ret(p_nValues);

    if constexpr (std::is_same<Generator, std::nullptr_t>::value)
    {
        std::for_each(ret.begin(), ret.end(), [this](auto & el) { el = (*this)(); });
        return ret;
    }

    else
    {
        ret = m_generator.gaussians(std::move(ret));
        for (auto & el : ret)
        {
            el *= std::sqrt(m_sigma.integralSquare(0, m_t));
            el = m_precalc * std::exp(el);
        }

        return ret;
    }
}

} // namespace der

#endif // DER_SIMSPOT_H
