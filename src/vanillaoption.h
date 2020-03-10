/** \file vanillaoption.h
 *  \author Andrej Leban
 *  \date 2/2018
 *
 *  A vanilla option type with an abstract payoff
 */

#ifndef VANILLAOPTION_H
#define VANILLAOPTION_H

#include "payoff.h"

namespace der
{

//! \brief A vanilla option type with an abstract payoff
class VanillaOption
{
public:
    VanillaOption(std::unique_ptr<Payoff> pPayoff, double expiry);
    VanillaOption(const Payoff & pPayoff, double expiry);
    ~VanillaOption() = default;
    VanillaOption(const VanillaOption & p_othr);
    VanillaOption(VanillaOption && p_othr) noexcept;
    VanillaOption & operator=(const VanillaOption & p_othr);
    VanillaOption & operator=(VanillaOption && p_othr) noexcept;

    //! \brief expiry time
    //! \return
    double expiry() const { return m_expiry; }

    //! \brief evaluates the payoff for the spot value provided
    //! \param spot
    //! \return
    double optionPayoff(double spot) const;

private:
    std::unique_ptr<Payoff> m_pPayoff;
    double m_expiry;
};

} // namespace der

#endif // VANILLAOPTION_H
