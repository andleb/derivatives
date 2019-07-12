/** \file pathdependent.h
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#ifndef PATHDEPENDENT_H
#define PATHDEPENDENT_H

#include <utility>
#include <vector>

#include "derivatives.h"

namespace der
{

//Forward declarations
class Payoff2;

// no ctors => an aggregation hence can be aggregate-initialized!

//! \brief The CashFlow struct
//!
//TODO: better way to handle this timeIndex stuff
struct CashFlow
{
    double amount;
    size_t timeIndex;
};

// TODO: remove the virtuals to save on the vtable if you'll be using static polymorphism only

//! \brief The PathDependent class
//!
class PathDependent
{
public:
    explicit PathDependent(const std::vector<double> & p_lookAtTimes);
    PathDependent(const PathDependent &) = default;
    PathDependent(PathDependent &&) = default;
    PathDependent & operator=(const PathDependent &) = default;
    PathDependent & operator=(PathDependent &&) = default;
    virtual ~PathDependent();

    virtual std::unique_ptr<PathDependent> clone() const = 0;

    std::vector<double> lookAtTimes() const;

    virtual constexpr size_t maxNumberOfCashFlows() const = 0;
    virtual constexpr std::vector<double> possibleCashFlowTimes() const = 0;

    //NOTE: return a modified version of the input, prefering this to input/output params
    virtual std::vector<CashFlow> cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const = 0;

protected:
    std::vector<double> m_lookAtTimes;
};


class AsianOptionArith : public PathDependent
{
public:
    //! \brief AsianOptionArith
    //! \param p_lookAtTimes: the averaging times
    //! \param p_delivery:  the expiry date can be different from the last averaging date
    //! \param p_payoff:    the simple payoff/option this option is composed of, i.e. vanilla call etc.
    //!
    AsianOptionArith(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff2 & p_payoff);
    AsianOptionArith(const AsianOptionArith & p_other);
    AsianOptionArith(AsianOptionArith &&) = default;
    AsianOptionArith & operator=(const AsianOptionArith & p_other);
    AsianOptionArith & operator=(AsianOptionArith &&) = default;
    ~AsianOptionArith() override = default;

    std::unique_ptr<PathDependent> clone() const override;

    size_t maxNumberOfCashFlows() const override;
    std::vector<double> possibleCashFlowTimes() const override;
    std::vector<CashFlow> cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const override;

protected:
    double m_delivery;
    std::unique_ptr<Payoff2> m_pPayoff;
};

} // namespace der

#endif // PATHDEPENDENT_H
