/** \file derivatives.cpp
 * \author Andrej Leban
 * \date 3/2020
 */

#include <algorithm>
#include <cmath>
#include <iostream>

#include "derivatives.h"


namespace der
{
double gaussianDensity(double p_x)
{
    static constexpr double inv2SqrtPi = 0.39894228040143267794;
    return inv2SqrtPi * std::exp(-1. * p_x * p_x / 2.);
}

double cumulativeGaussian(double p_x) { return 0.5 * (1 + std::erf(p_x / std::sqrt(2))); }

double inverseCumulativeGaussian(double p_y)
{
    // adapted from M. Joshi's source

    static constexpr double a[4] = {2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};

    static constexpr double b[4] = {-8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833};

    static constexpr double c[9] = {0.3374754822726147, 0.9761690190917186, 0.1607979714918209,
                                    0.0276438810333863, 0.0038405729373609, 0.0003951896511919,
                                    0.0000321767881768, 0.0000002888167364, 0.0000003960315187};

    double u = p_y - 0.5;
    double r;

    if (std::abs(u) < 0.42) // Beasley-Springer approximation
    {
        double y = u * u;

        r = u * (((a[3] * y + a[2]) * y + a[1]) * y + a[0]) / ((((b[3] * y + b[2]) * y + b[1]) * y + b[0]) * y + 1.0);
    }
    else // Moro approximation
    {

        r = p_y;

        if (u > 0.0)
        {
            r = 1.0 - p_y;
        }

        r = std::log(-std::log(r));

        r = c[0] + r * (c[1] + r * (c[2] + r * (c[3] + r * (c[4] + r * (c[5] + r * (c[6] + r * (c[7] + r * c[8])))))));

        if (u < 0.0)
        {
            r = -r;
        }
    }

    p_y = r;

    return p_y;
}

// BS formuals

BSFormula::BSFormula(BSFormula::p p_r, BSFormula::p p_d, BSFormula::p p_T, BSFormula::p p_sigma, BSFormula::p p_S0, BSFormula::p p_K)
    : m_r(p_r), m_d(p_d), m_T(p_T), m_sigma(p_sigma), m_S0(p_S0), m_K(p_K)
{}

BSFormula::~BSFormula() = default;

double BSCall::operator()(BSFormula::p p_r, BSFormula::p p_d, BSFormula::p p_T, BSFormula::p p_sigma, BSFormula::p p_S0, BSFormula::p p_K) const
{
    double r = p_r.value_or(m_r.value());
    double d = p_d.value_or(m_d.value());
    double T = p_T.value_or(m_T.value());
    double sigma = p_sigma.value_or(m_sigma.value());
    double S0 = p_S0.value_or(m_S0.value());
    double K = p_K.value_or(m_K.value());

    auto params = {r, d, T, sigma, S0, K};
    if (std::any_of(params.begin(), params.end(), [](const auto & el) { return el == 0; }))
    {
        std::cerr << "BSCall::operator::Warning: some parameters zero. Is this intentional?";
    }

    return BSCallFormula(r, d, T, sigma, S0, K);
}

double BSCall::operator()(double p_sigma) const { return BSCallFormula(*m_r, *m_d, *m_T, p_sigma, *m_S0, *m_K); }

double BSCall::vega(double p_sigma) const
{
    double std = p_sigma * std::sqrt(*m_T);
    static double logSK = std::log(*m_S0 / *m_K);
    double d1 = (logSK + (*m_r - *m_d) * (*m_T) + 0.5 * std * std) / std;
    return std::exp(-1. * (*m_d) * (*m_T)) * (*m_S0) * std::sqrt(*m_T) * cumulativeGaussian(d1);
}

double BSCall::BSCallFormula(const double p_r, const double p_d, const double p_T, const double p_sigma, const double p_S0, const double p_K)
{
    double std = p_sigma * std::sqrt(p_T);
    double logSK = std::log(p_S0 / p_K);
    double d1 = (logSK + (p_r - p_d) * p_T + 0.5 * std * std) / std;
    double d2 = d1 - std;
    return std::exp(-1. * p_d * p_T) * p_S0 * cumulativeGaussian(d1) - std::exp(-1. * p_r * p_T) * cumulativeGaussian(d2);
}


double BSPut::operator()(BSFormula::p p_r, BSFormula::p p_d, BSFormula::p p_T, BSFormula::p p_sigma, BSFormula::p p_S0, BSFormula::p p_K) const
{
    double r = p_r.value_or(m_r.value());
    double d = p_d.value_or(m_d.value());
    double T = p_T.value_or(m_T.value());
    double sigma = p_sigma.value_or(m_sigma.value());
    double S0 = p_S0.value_or(m_S0.value());
    double K = p_K.value_or(m_K.value());

    auto params = {r, d, T, sigma, S0, K};
    if (std::any_of(params.begin(), params.end(), [](const auto & el) { return el == 0; }))
    {
        std::cerr << "BSCall::operator::Warning: some parameters zero. Is this intentional?";
    }

    return BSPutFormula(r, d, T, sigma, S0, K);
}

double BSPut::operator()(double p_sigma) const { return BSPutFormula(*m_r, *m_d, *m_T, p_sigma, *m_S0, *m_K); }

double BSPut::vega(double p_sigma) const
{
    // is the same as for Calls
    // The classes are identical in layout, so this reinterpret_cast is OK.
    return reinterpret_cast<const BSCall *>(this)->vega(p_sigma);
}

double BSPut::BSPutFormula(const double p_r, const double p_d, const double p_T, const double p_sigma, const double p_S0, const double p_K)
{
    double std = p_sigma * std::sqrt(p_T);
    double logSK = std::log(p_S0 / p_K);
    double d1 = (logSK + (p_r - p_d) * p_T + 0.5 * std * std) / std;
    double d2 = d1 - std;
    return -std::exp(-1. * p_d * p_T) * p_S0 * cumulativeGaussian(-d1) + std::exp(-1. * p_r * p_T) * cumulativeGaussian(-d2);
}
} // namespace der
