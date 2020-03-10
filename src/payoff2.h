/** \file payoff2.h
 * \author Andrej Leban
 * \date 11/2018
 */

#ifndef PAYOFF2_H
#define PAYOFF2_H

#include <memory>

namespace der
{

class Payoff2
{
public:
    Payoff2() = default;
    Payoff2(const Payoff2 &) = default;
    Payoff2 & operator=(const Payoff2 &) = default;
    Payoff2(Payoff2 &&) = default;
    Payoff2 & operator=(Payoff2 &&) = default;
    virtual ~Payoff2();

    virtual std::unique_ptr<Payoff2> clone() const = 0;

    virtual double operator()(double p_spot) const = 0;
};

class Payoff2call : public Payoff2
{
public:
    Payoff2call(double p_strike)
        : m_strike(p_strike)
    {}

    std::unique_ptr<Payoff2> clone() const override;

    double operator()(double p_spot) const override;

private:
    double m_strike = 0;
};

class Payoff2put : public Payoff2
{
public:
    Payoff2put(double p_strike)
        : m_strike(p_strike)
    {}

    std::unique_ptr<Payoff2> clone() const override;

    double operator()(double p_spot) const override;

private:
    double m_strike = 0;
};

class Payoff2DoubleDigital : public Payoff2
{
    Payoff2DoubleDigital(double lowerLevel, double upperLevel)
        : m_lowerLevel(lowerLevel)
        , m_upperLevel(upperLevel)
    {}

    std::unique_ptr<Payoff2> clone() const override;

    double operator()(double spot) const override;

private:
    double m_lowerLevel;
    double m_upperLevel;
};

} // namespace der

#endif // PAYOFF2_H
