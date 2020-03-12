/** \file statistics.cpp
 * \author Andrej Leban
 * \date 2/2019
 */

#include <algorithm>

#include "statistics.h"

namespace der
{

StatisticsBase::~StatisticsBase() = default;

StatisticsMean::StatisticsMean(double runningSum, size_t paths) : m_runningSum(runningSum), m_nPathsDone(paths) {}

std::unique_ptr<StatisticsBase> StatisticsMean::clone() const
{
    return std::make_unique<StatisticsMean>(m_runningSum, m_nPathsDone);
}

std::vector<std::vector<double>> StatisticsMean::resultsSoFar() const { return {{m_runningSum / m_nPathsDone}}; }

size_t StatisticsMean::simsSoFar() const { return m_nPathsDone; }

void StatisticsMean::dumpOneResult(double val)
{
    m_runningSum += val;
    ++m_nPathsDone;
}

ConvergenceTable::~ConvergenceTable() = default;

ConvergenceTable::ConvergenceTable(std::unique_ptr<StatisticsBase> p_pGatherer) : m_pGatherer(std::move(p_pGatherer)) {}

std::unique_ptr<StatisticsBase> ConvergenceTable::clone() const { return std::make_unique<ConvergenceTable>(*this); }

std::vector<std::vector<double>> ConvergenceTable::resultsSoFar() const
{
    auto ret = m_results;

    // The inequality means the "cache" hasn't been updated in dumpOneResult, so add the last result
    if (m_nPathsDone * 2 != m_count)
    {
        ret.push_back({static_cast<double>(m_nPathsDone), m_pGatherer->resultsSoFar().back().back()});
    }

    return ret;
}

size_t ConvergenceTable::simsSoFar() const { return m_nPathsDone; }

void ConvergenceTable::dumpOneResult(double val)
{
    m_pGatherer->dumpOneResult(val);
    m_nPathsDone++;

    if (m_nPathsDone == m_count)
    {
        m_count *= 2;
        m_results.push_back({static_cast<double>(m_nPathsDone), m_pGatherer->resultsSoFar().back().back()});
    }
}

} // namespace der
