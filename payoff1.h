/** \file payoff1.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef PAYOFF1_H
#define PAYOFF1_H

namespace der {

enum class OptionsType
{
    call,
    put,
    digital
};

class payoff1
{
public:
    payoff1(double p_strike, OptionsType p_type);

    double operator() (double p_spot) const;

private:
    const double m_strike;
    OptionsType m_type;
};

} // namespace der
#endif // PAYOFF1_H
