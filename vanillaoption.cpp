/** \file vanillaoption.cpp
*   \author Andrej Leban
*   \date 2/2018
*/

#include "vanillaoption.h"

namespace der
{

VanillaOption::VanillaOption(const VanillaOption & p_othr)
    : m_pPayoff(p_othr.m_pPayoff->clone())
    , m_expiry(p_othr.m_expiry)
{}

VanillaOption::VanillaOption(VanillaOption && p_othr) noexcept
    : m_pPayoff(std::move(p_othr.m_pPayoff))
    , m_expiry(p_othr.m_expiry)
{}

VanillaOption & VanillaOption::operator=(const VanillaOption & p_othr)
{
    if (this != &p_othr)
    {
        this->m_expiry = p_othr.m_expiry;
        this->m_pPayoff = p_othr.m_pPayoff->clone();
    }

    return *this;
}

VanillaOption &VanillaOption::operator=(VanillaOption && p_othr) noexcept
{
    if (this != &p_othr)
    {
        this->m_expiry = p_othr.m_expiry;
        this->m_pPayoff = std::move(p_othr.m_pPayoff);
    }

    return *this;
}

double VanillaOption::optionPayoff(double spot) const
{
    return (*m_pPayoff)(spot);
}

} // namespace der
