/** \file pathdependent.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include "pathdependent.h"

namespace der
{

PathDependent::~PathDependent() = default;

std::vector<double> PathDependent::lookAtTimes() const
{
    return m_lookAtTimes;
}

std::unique_ptr<PathDependent> AsianOption::clone() const
{
    return std::make_unique<AsianOption>(*this);
}

size_t AsianOption::maxNumberOfCashFlows() const
{
    //TODO:
    return 0;
}

std::vector<double> AsianOption::possibleCashFlowTimes() const
{
    //TODO:
    return {};
}

std::vector<CashFlow> AsianOption::cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const
{
    //TODO:
    return {};
}

} // namespace der
