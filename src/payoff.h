/** \file payoff.h
 * \author Andrej Leban
 * \date 11/2018
 *
 * Abstract payoff interface.
 */

#ifndef PAYOFF2_H
#define PAYOFF2_H

#include <memory>

// NOTE: these are registered in payoffregistration.cpp.

namespace der
{

//! \brief Abstract payoff class.
class Payoff
{
public:
    Payoff() = default;
    Payoff(const Payoff &) = default;
    Payoff & operator=(const Payoff &) = default;
    Payoff(Payoff &&) = default;
    Payoff & operator=(Payoff &&) = default;
    virtual ~Payoff();

    virtual std::unique_ptr<Payoff> clone() const = 0;
    //! \brief Calculates the payoff.
    virtual double operator()(double p_spot) const = 0;
};

//! \brief Implementation for calls.
class PayoffCall : public Payoff
{
public:
    PayoffCall(double p_strike);

    std::unique_ptr<Payoff> clone() const override;
    double operator()(double p_spot) const override;

private:
    double m_strike = 0;
};

//! \brief Implementation for puts.
class PayoffPut : public Payoff
{
public:
    PayoffPut(double p_strike);

    std::unique_ptr<Payoff> clone() const override;
    double operator()(double p_spot) const override;

private:
    double m_strike = 0;
};

//! \brief Implementation for double digital options.
class PayoffDoubleDigital : public Payoff
{
public:
    //! \brief PayoffDoubleDigital
    //! \param lowerLevel - the lower knockout boundary
    //! \param upperLevel - the upper knockout boundary
    PayoffDoubleDigital(double lowerLevel, double upperLevel);

    std::unique_ptr<Payoff> clone() const override;
    double operator()(double spot) const override;

private:
    double m_lowerLevel;
    double m_upperLevel;
};

//! \brief Implementation for a simple Forward.
class PayoffForward : public Payoff
{

public:
    PayoffForward(double p_strike);
    virtual std::unique_ptr<Payoff> clone() const override;

    virtual double operator()(double p_spot) const override;

private:
    double m_strike = 0;
};

} // namespace der

#endif // PAYOFF2_H
