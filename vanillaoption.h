/** \file vanillaoption.h
 *  \author Andrej Leban
 *  \date 2/2018
 */

#ifndef VANILLAOPTION_H
#define VANILLAOPTION_H

#include "payoff2.h"

namespace der {

class VanillaOption
{
public:
    VanillaOption(const Payoff2 & payoff, double expiry)
        : m_payoff(payoff)
        , m_expiry(expiry)
    {}

    double expiry() const { return m_expiry; }
    double optionPayoff(double spot) const;

private:
    const Payoff2 & m_payoff;
    double m_expiry;
};

} // namespace der

#endif // VANILLAOPTION_H
