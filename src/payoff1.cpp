/** \file payoff1.cpp
 * \author Andrej Leban
 * \date 11/2018
 */

#include "payoff1.h"

#include <algorithm>
#include <stdexcept>

der::payoff1::payoff1(double p_strike, OptionsType p_type) : m_strike(p_strike), m_type(p_type) {}

double der::payoff1::operator() (double p_spot) const
{
    switch( m_type )
    {
    case OptionsType::call:
        return std::max<double>(p_spot - m_strike, 0);
    case OptionsType::put:
        return std::max<double>(m_strike - p_spot, 0);
    case OptionsType::digital:
        return p_spot > m_strike;
//    default:
//        throw std::runtime_error{"Unrecognized option type"};
    }
}
