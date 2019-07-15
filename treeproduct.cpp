/** \file treeproduct.h
 * \author Andrej Leban
 * \date 7/2019
 * Ch. 8 Trees
 */

#include "treeproduct.h"

namespace der
{

TreeProduct::TreeProduct(double p_expiryTime, const Payoff2 & p_payoff)
    : m_expiryTime(p_expiryTime)
    , m_pPayoff(p_payoff.clone())
{}

TreeProduct::TreeProduct(double p_expiryTime, std::unique_ptr<Payoff2> p_payoff)
    : m_expiryTime(p_expiryTime)
      , m_pPayoff(std::move(p_payoff))
{}

TreeProduct::TreeProduct(const TreeProduct & p_other)
    : m_expiryTime(p_other.m_expiryTime)
    , m_pPayoff(p_other.m_pPayoff->clone())
{}

TreeProduct & TreeProduct::operator=(const TreeProduct & p_other)
{
    if(this != &p_other)
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

} // namespace der
