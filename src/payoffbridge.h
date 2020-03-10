/** \file payoffbridge.h
 *  \author Andrej Leban
 *  \date 2/2018
 */

#ifndef PAYOFFBRIDGE_H
#define PAYOFFBRIDGE_H

#include "payoff2.h"

namespace der
{

class PayoffBridge
{
public:
    //NOTE: we wish to have implicit conversion
    PayoffBridge(const Payoff2 & payoff)
        : m_pPayoff(payoff.clone())
    {}
    ~PayoffBridge() = default;
    PayoffBridge(const PayoffBridge & p_othr)
        : m_pPayoff(p_othr.m_pPayoff->clone())
    {}
    PayoffBridge(PayoffBridge && p_othr) noexcept
        : m_pPayoff(std::move(p_othr.m_pPayoff))
    {}
    PayoffBridge & operator=(const PayoffBridge & p_othr);
    PayoffBridge & operator=(PayoffBridge && p_othr) noexcept;

    double operator() (double p_spot) const { return (*m_pPayoff)(p_spot); }

private:
    std::unique_ptr<Payoff2> m_pPayoff;
};

} // namespace der

#endif // PAYOFFBRIDGE_H
