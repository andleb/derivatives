#ifndef PAYOFF2_H
#define PAYOFF2_H

/** \file payoff2.h
 * \author Andrej Leban
 * \date 11/2018
 */

#include <algorithm>

namespace der {

class payoff2
{
public:
    payoff2();
    virtual ~payoff2() {};

    virtual double operator() (double p_spot) const = 0;
};

class payoff2call : public payoff2
{
public:
    payoff2call(double p_strike) : m_strike(p_strike) {};
    virtual ~payoff2call() {};

    virtual double operator() (double p_spot) const
    {
        return std::max<double>(p_spot - m_strike, 0);
    };

private:
    double m_strike = 0;
};

} // namespace der


#endif // PAYOFF2_H
