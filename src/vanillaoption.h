/** \file vanillaoption.h
 *  \author Andrej Leban
 *  \date 2/2018
 */

#ifndef VANILLAOPTION_H
#define VANILLAOPTION_H

#include "payoff.h"

namespace der
{

class VanillaOption
{
public:
    VanillaOption(std::unique_ptr<Payoff> pPayoff, double expiry)
        : m_pPayoff(std::move(pPayoff))
        , m_expiry(expiry)
    {}
    ~VanillaOption() = default;
    VanillaOption(const VanillaOption & p_othr);
    VanillaOption(VanillaOption && p_othr) noexcept;
    VanillaOption & operator=(const VanillaOption & p_othr);
    VanillaOption & operator=(VanillaOption && p_othr) noexcept;

    double expiry() const { return m_expiry; }
    double optionPayoff(double spot) const;

private:
    std::unique_ptr<Payoff> m_pPayoff;
    double m_expiry;
};

} // namespace der

#endif // VANILLAOPTION_H
