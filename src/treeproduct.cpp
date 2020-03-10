/** \file treeproduct.h
 * \author Andrej Leban
 * \date 7/2019
 * Ch. 8 Trees
 */

#include "treeproduct.h"

namespace der
{

TreeProduct::TreeProduct(double p_expiryTime, const Payoff & p_payoff)
    : m_expiryTime(p_expiryTime)
    , m_pPayoff(p_payoff.clone())
{}

TreeProduct::TreeProduct(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff)
    : m_expiryTime(p_expiryTime)
    , m_pPayoff(std::move(p_pPayoff))
{}

TreeProduct::TreeProduct(const TreeProduct & p_other)
    : m_expiryTime(p_other.m_expiryTime)
    , m_pPayoff(p_other.m_pPayoff->clone())
{}

TreeProduct & TreeProduct::operator=(const TreeProduct & p_other)
{
    if (this != &p_other)
    {
        m_expiryTime = p_other.m_expiryTime;
        m_pPayoff = p_other.m_pPayoff->clone();
    }

    return *this;
}

double TreeProduct::expiryTime() const
{
    return m_expiryTime;
}

double TreeProduct::payoff(double p_spot) const
{
    return (*m_pPayoff)(p_spot);
}

TreeProduct::~TreeProduct() = default;

// TreeAmerican

TreeAmerican::TreeAmerican(double p_expiryTime, const Payoff & p_payoff)
    : TreeProduct(p_expiryTime, p_payoff)
{}

TreeAmerican::TreeAmerican(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff)
    : TreeProduct(p_expiryTime, std::move(p_pPayoff))
{}

double TreeAmerican::value(double p_spot, double /*p_t*/, double p_futureValue) const
{
    // the strategy doesn't depend on the time in question
    return std::max(p_spot, p_futureValue);
}

// TreeEuropean

TreeEuropean::TreeEuropean(double p_expiryTime, const Payoff & p_payoff)
    : TreeProduct(p_expiryTime, p_payoff)
{}

TreeEuropean::TreeEuropean(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff)
    : TreeProduct(p_expiryTime, std::move(p_pPayoff))
{}

double TreeEuropean::value(double /*p_spot*/, double /*p_t*/, double p_futureValue) const
{
    // the European option has no exercise strategy, R-N pricing prescribes the value to be the discounted
    // future value
    return p_futureValue;
}

} // namespace der
