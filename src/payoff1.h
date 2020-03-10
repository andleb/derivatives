/** \file payoff1.h
 * \author Andrej Leban
 * \date 11/2018
 *
 * A simple encapsulation of a payoff function
 */

#ifndef PAYOFF1_H
#define PAYOFF1_H

namespace der
{

//! \brief The OptionsType enum
enum class OptionsType
{
    call,
    put,
    digital
};

//! \brief The payoff1 class
class payoff1
{
public:
    payoff1(double p_strike, OptionsType p_type);

    //! \brief Evaluates the payoff for the \p p_spot
    //! \param p_spot
    //! \return
    double operator()(double p_spot) const;

private:
    const double m_strike;
    OptionsType m_type;
};

} // namespace der
#endif // PAYOFF1_H
