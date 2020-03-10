/** \file Payoff2.cpp
 * \author Andrej Leban
 * \date 2/2019
 */

#include <algorithm>

#include "payoff2.h"

namespace der {



Payoff2::~Payoff2() = default;

//Payoff2call::Payoff2call(double p_strike) : m_strike(p_strike) {}

std::unique_ptr<Payoff2> Payoff2call::clone() const
{
    return std::make_unique<Payoff2call>(*this);
}

double Payoff2call::operator()(double p_spot) const
{
    return std::max<double> (p_spot - m_strike, 0);
}


std::unique_ptr<Payoff2> Payoff2put::clone() const
{
    return std::make_unique<Payoff2put>(*this);
}

double Payoff2put::operator()(double p_spot) const
{
    return std::max<double> (m_strike - p_spot, 0);
}


std::unique_ptr<Payoff2> Payoff2DoubleDigital::clone() const
{
    return std::make_unique<Payoff2DoubleDigital>(*this);
}

double Payoff2DoubleDigital::operator()(double spot) const
{
    return (spot <= m_upperLevel && spot >= m_lowerLevel) ? 1.0 : 0.0;
}


} //namespace der
