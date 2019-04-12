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

    std::vector<double> lookAtTimes() const;

    virtual constexpr size_t maxNumberOfCashFlows() const = 0;
    virtual constexpr std::vector<double> possibleCashFlowTimes() const = 0;
    virtual std::pair<std::vector<double>, size_t>
        cashFlows(const std::vector<double> & p_spots, std::vector<double> && p_flows) const = 0;


private:
    std::vector<double> m_lookAtTimes;
};

} // namespace der

#endif // PATHDEPENDENT_H
