/** \file payoffbridge.cpp
 *  \author Andrej Leban
 *  \date 2/2018
 */

#include "payoffbridge.h"

namespace der {

PayoffBridge &PayoffBridge::operator=(const PayoffBridge & p_othr)
{
    if (this != &p_othr)
    {
        this->m_pPayoff = p_othr.m_pPayoff->clone();
    }

    return *this;
}

PayoffBridge &PayoffBridge::operator=(PayoffBridge && p_othr) noexcept
{
    if (this != &p_othr)
    {
        this->m_pPayoff = std::move(p_othr.m_pPayoff);
    }

    return *this;
}

} // namespace der
