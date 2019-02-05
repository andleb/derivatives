/** \file payoff2.cpp
 * \author Andrej Leban
 * \date 2/2019
 */

#include "payoff2.h"

namespace der {

double payoff2call::operator()(double p_spot) const
{
    return std::max<double>(p_spot - m_strike, 0);
}

payoff2::~payoff2() {}


payoffDoubleDigital::payoffDoubleDigital(double LowerLevel, double UpperLevel)
{

}



double payoffDoubleDigital::operator()(double Spot)
{

}


} //namespace der
