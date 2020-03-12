/** \file pathdependent.cpp
 * \author Andrej Leban
 * \date 4/2019
 */

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "payoff.h"

#include "pathdependent.h"

namespace der
{

// PathDependent

PathDependent::PathDependent(std::vector<double> p_lookAtTimes) : m_lookAtTimes(std::move(p_lookAtTimes)) {}

PathDependent::~PathDependent() = default;

std::vector<double> PathDependent::lookAtTimes() const { return m_lookAtTimes; }

// AsianOption

AsianOption::AsianOption(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff & p_payoff)
    : PathDependent(p_lookAtTimes), m_delivery(p_delivery), m_pPayoff(p_payoff.clone())
{}

AsianOption::AsianOption(const AsianOption & p_other)
    : PathDependent(p_other), m_delivery(p_other.m_delivery), m_pPayoff(p_other.m_pPayoff->clone())
{}

AsianOption & AsianOption::operator=(const AsianOption & p_other)
{
    if (this != &p_other)
    {
        // call without returning
        PathDependent::operator=(p_other);
        this->m_delivery = p_other.m_delivery;
        this->m_pPayoff = p_other.m_pPayoff->clone();
    }
    return *this;
}

size_t AsianOption::maxNumberOfCashFlows() const { return 1; }

std::vector<double> AsianOption::possibleCashFlowTimes() const { return {m_delivery}; }

// AsianOptionArith

AsianOptionArith::AsianOptionArith(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff & p_payoff)
    : AsianOption(p_lookAtTimes, p_delivery, p_payoff)
{}

std::unique_ptr<PathDependent> AsianOptionArith::clone() const { return std::make_unique<AsianOptionArith>(*this); }

std::vector<CashFlow> AsianOptionArith::cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const
{
    p_flows.resize(1);
    double sum = std::accumulate(p_spots.begin(), p_spots.end(), 0.0);

    p_flows[0].timeIndex = 0;
    // the payoff is a function of the arithmetic average
    p_flows[0].amount = (*m_pPayoff)(sum / m_lookAtTimes.size());
    return std::move(p_flows);
}

// AsianOptionGeom

AsianOptionGeom::AsianOptionGeom(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff & p_payoff)
    : AsianOption(p_lookAtTimes, p_delivery, p_payoff)
{}

std::unique_ptr<PathDependent> AsianOptionGeom::clone() const { return std::make_unique<AsianOptionGeom>(*this); }

std::vector<CashFlow> AsianOptionGeom::cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const
{
    p_flows.resize(1);
    double tot =
        std::accumulate(p_spots.begin(), p_spots.end(), 1.0, [](auto & runningProd, auto curr) { return runningProd *= curr; });

    p_flows[0].timeIndex = 0;
    // the payoff is a function of the geometric average
    p_flows[0].amount = (*m_pPayoff)(std::pow(tot, 1.0 / m_lookAtTimes.size()));
    return std::move(p_flows);
}

} // namespace der
