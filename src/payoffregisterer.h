/** \file payoffregisterer.h
 * \author Andrej Leban
 * \date 3/2020
 *
 * An adapter that makes a Payoff class with a constructor that takes a double usable
 * with the der::PayoffFactory class.
 */

#ifndef DER_PAYOFFREGISTERER_H
#define DER_PAYOFFREGISTERER_H

#include <string>
#include <utility>

#include "payofffactory.h"

namespace der
{

//! \brief An adapter that makes any Payoff class which has a constructor that takes a double
//!  usable with the PayoffFactory class.
template <typename T>
class payoffRegisterer
{
public:
    //! \brief Automatically registers the class with PayoffFactory under \p p_name.
    //! \param p_name
    payoffRegisterer(std::string p_name);
    static std::unique_ptr<Payoff> create(double p_strike);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
payoffRegisterer<T>::payoffRegisterer(std::string p_name)
{
    PayoffFactory::instance().registerPayoff(std::move(p_name), &payoffRegisterer<T>::create);
}

template<typename T>
std::unique_ptr<Payoff> payoffRegisterer<T>::create(double p_strike)
{
    return std::make_unique<T>(p_strike);
}

} // namespace der

#endif // DER_PAYOFFREGISTERER_H
