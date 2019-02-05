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
    payoff2(const payoff2 &) = default;
    payoff2 & operator=(const payoff2 &) = default;
    payoff2(payoff2 &&) = default;
    payoff2 & operator=(payoff2 &&) = default;
    virtual ~payoff2();

    virtual double operator() (double p_spot) const = 0;
};

class payoff2call : public payoff2
{
public:
    payoff2call(double p_strike) : m_strike(p_strike) {}

    double operator() (double p_spot) const override;

private:
    double m_strike = 0;
};

class payoffDoubleDigital : public payoff2
{
    payoffDoubleDigital(double LowerLevel, double UpperLevel);

    double operator()(double Spot) const override;

private:
    double LowerLevel;
    double UpperLevel;
};



} // namespace der


#endif // PAYOFF2_H
