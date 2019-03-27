/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <algorithm>
#include <random>
#include <type_traits>

#include "parameters.h"


namespace der
{

template<typename T>
T normalDist()
{
    static std::random_device rDev;
    // The generator is seeded from the implementation-defined device - this should be /dev/random
    static std::mt19937_64 rng {rDev()};
    static std::normal_distribution<T> Ndist;

    return Ndist(rng);
}

double normalDist()
{
    static std::random_device rDev;
    // The generator is seeded from the implementation-defined device - this should be /dev/random
    static std::mt19937_64 rng {rDev()};
    static std::normal_distribution<double> Ndist;

    return Ndist(rng);
}

//! \brief The simSpot struct
//! Simulates the value of a spot price at time p_t for some volatility & interest rate
struct simSpot
{
    simSpot(double p_S0, double p_t, double p_sigma, double p_r)
        : m_t(p_t)
        , m_sigma(p_sigma)
        , m_precalc(p_S0 * std::exp((p_r - 0.5 * p_sigma * p_sigma) * p_t))
    {}

    double operator()() const
    {
        return m_precalc * std::exp(m_sigma * std::sqrt(m_t) * normalDist());
    }

    double m_t;
    double m_sigma;
    double m_precalc;
};

//! \brief The simSpotParams struct
//! Allows for variable volatilities & interest rates
struct simSpotParams
{
    simSpotParams(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r)
        : m_t(p_t)
        , m_sigma(p_sigma)
        , m_precalc(p_S0 * std::exp(p_r.integral(0, p_t) - 0.5 * p_sigma.integralSquare(0, p_t)))
    {}

    double operator()() const
    {
        // need root variance, sqrt(t) already factored in
        return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * normalDist());
    }


    double m_t;
    Parameters m_sigma;
    double m_precalc;
};

template<typename Generator = std::nullptr_t>
struct simSpotParamsMultiple : public simSpotParams
{
    simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r, size_t p_seed)
        : simSpotParams(p_S0, p_t, p_sigma, p_r)
        , m_generator(p_seed)
    {}

    simSpotParamsMultiple(double p_S0, double p_t, const Parameters & p_sigma, const Parameters & p_r, Generator & p_generator)
        : simSpotParams(p_S0, p_t, p_sigma, p_r)
        , m_generator(p_generator)
    {}

    double operator()() const
    {
        // make use of custom generator
        if constexpr(std::is_same<Generator, std::nullptr_t>::value)
        {
            return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * normalDist());
        }

        return m_precalc * std::exp(sqrt(m_sigma.integralSquare(0, m_t)) * m_generator.gaussians(std::vector<double>(1)));
    }

    std::vector<double> simSpotMultiple(size_t p_nValues)
    {
        std::vector<double> ret(p_nValues);

        if constexpr(std::is_same<Generator, std::nullptr_t>::value)
        {
            std::for_each(ret.begin(), ret.end(), [this](auto & el) { el = *this(); });
            return ret;
        }

        ret = m_generator.gaussians(std::move(ret));
        for(auto & el: ret)
        {
            el *= std::sqrt(m_sigma.integralSquare(0, m_t));
            el = m_precalc * std::exp(el);
        }

        return ret;
    }

    mutable Generator m_generator;
};

} // namespace der
#endif // DERIVATIVES_H
