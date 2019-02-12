/** \file vanillaoption2.h
 *  \author Andrej Leban
 *  \date 2/2018
 */

#ifndef VANILLAOPTION2_H
#define VANILLAOPTION2_H

#include "payoffbridge.h"

namespace der
{

class VanillaOption2
{
public:
    VanillaOption2(PayoffBridge payoff, double expiry)
        : m_payoff(std::move(payoff))
        , m_expiry(expiry)
    {}

    double expiry() const { return m_expiry; }
    double optionPayoff(double spot) const { return m_payoff(spot); }

private:
    PayoffBridge m_payoff;
    double m_expiry;
};

} // namespace der

#endif // VANILLAOPTION2_H
