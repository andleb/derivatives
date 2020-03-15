/** \file payoff.cpp
 * \author Andrej Leban
 * \date 2/2019
 */

#include <algorithm>

#include "payoff.h"

namespace der
{

Payoff::~Payoff() = default;

PayoffCall::PayoffCall(double p_strike) : m_strike(p_strike) {}

std::unique_ptr<Payoff> PayoffCall::clone() const { return std::make_unique<PayoffCall>(*this); }

double PayoffCall::operator()(double p_spot) const { return std::max<double>(p_spot - m_strike, 0.0); }

PayoffPut::PayoffPut(double p_strike) : m_strike(p_strike) {}

std::unique_ptr<Payoff> PayoffPut::clone() const { return std::make_unique<PayoffPut>(*this); }

double PayoffPut::operator()(double p_spot) const { return std::max<double>(m_strike - p_spot, 0.0); }

PayoffDoubleDigital::PayoffDoubleDigital(double lowerLevel, double upperLevel)
    : m_lowerLevel(lowerLevel), m_upperLevel(upperLevel)
{}

std::unique_ptr<Payoff> PayoffDoubleDigital::clone() const { return std::make_unique<PayoffDoubleDigital>(*this); }

double PayoffDoubleDigital::operator()(double spot) const { return (spot <= m_upperLevel && spot >= m_lowerLevel) ? 1.0 : 0.0; }

PayoffForward::PayoffForward(double p_strike) : m_strike(p_strike) {}

std::unique_ptr<Payoff> PayoffForward::clone() const { return std::make_unique<PayoffForward>(*this); }

double PayoffForward::operator()(double p_spot) const { return p_spot - m_strike; }

} // namespace der
