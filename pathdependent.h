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

//! \brief The AsianOption class
//! An abstract class encapsulating common attributes to all Asian type options
class AsianOption : public PathDependent
{
public:
    //! \brief AsianOption
    //! \param p_lookAtTimes: the averaging times
    //! \param p_delivery:  the expiry date can be different from the last averaging date
    //! \param p_payoff:    the simple payoff/option this option is composed of, i.e. vanilla call etc.
    AsianOption(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff2 & p_payoff);
    AsianOption(const AsianOption & p_other);
    AsianOption(AsianOption &&) = default;
    AsianOption & operator=(const AsianOption & p_other);
    AsianOption & operator=(AsianOption &&) = default;
    ~AsianOption() override = default;


    size_t maxNumberOfCashFlows() const override;
    std::vector<double> possibleCashFlowTimes() const override;

    // NOTE: these are still abstract!
    //    std::unique_ptr<PathDependent> clone() const = 0;
    //    std::vector<CashFlow> cashFlows(const std::vector<double> &, std::vector<CashFlow> &&) const = 0;

protected:
    double m_delivery;
    std::unique_ptr<Payoff2> m_pPayoff;
};

class AsianOptionArith : public AsianOption
{
public:
    //! \brief AsianOptionArith
    //! \param p_lookAtTimes: the averaging times
    //! \param p_delivery:  the expiry date can be different from the last averaging date
    //! \param p_payoff:    the simple payoff/option this option is composed of, i.e. vanilla call etc.
    AsianOptionArith(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff2 & p_payoff);

    std::unique_ptr<PathDependent> clone() const override;

    std::vector<CashFlow> cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const override;
};

class AsianOptionGeom : public AsianOption
{
public:
    //! \brief AsianOptionGeom
    //! \param p_lookAtTimes: the averaging times
    //! \param p_delivery:  the expiry date can be different from the last averaging date
    //! \param p_payoff:    the simple payoff/option this option is composed of, i.e. vanilla call etc.
    //!
    AsianOptionGeom(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff2 & p_payoff);

    std::unique_ptr<PathDependent> clone() const override;

    std::vector<CashFlow> cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const override;
};

} // namespace der

#endif // PATHDEPENDENT_H
