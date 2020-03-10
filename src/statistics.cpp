/** \file statistics.cpp
 * \author Andrej Leban
 * \date 2/2019
 */

#include <algorithm>

#include "statistics.h"

namespace der
{

StatisticsBase::~StatisticsBase() = default;

std::unique_ptr<StatisticsBase> StatisticsMean::clone() const
{
    return std::make_unique<StatisticsMean>(m_runningSum, m_nPathsDone);
}

std::vector<std::vector<double>> StatisticsMean::resultsSoFar() const
{
    return {{m_runningSum / m_nPathsDone}};
}

size_t StatisticsMean::simsSoFar() const
{
    return m_nPathsDone;
}

void StatisticsMean::dumpOneResult(double val)
{
    m_runningSum += val;
    ++m_nPathsDone;
}

ConvergenceTable::~ConvergenceTable() = default;

std::unique_ptr<StatisticsBase> ConvergenceTable::clone() const
{
    return std::make_unique<ConvergenceTable>(*this);
}

std::vector<std::vector<double>> ConvergenceTable::resultsSoFar() const
{
    auto ret = m_results;

    // This means the "cache" hasn't been updated in dumpOneResult, so add the last result
    if (m_nPathsDoneConvergence * 2 != m_count)
    {
        auto tmp{m_pGatherer->resultsSoFar()};
        for (auto & row : tmp)
        {
            row.push_back(m_nPathsDoneConvergence);
            // I wish to reverse the results, Npaths : value
            std::reverse(row.begin(), row.end());
            ret.push_back(row);
        }
    }

    return ret;
}

size_t ConvergenceTable::simsSoFar() const
{
    return m_nPathsDoneConvergence;
}

void ConvergenceTable::dumpOneResult(double val)
{
    m_pGatherer->dumpOneResult(val);
    m_nPathsDoneConvergence++;

    if (m_nPathsDoneConvergence == m_count)
    {
        m_count *= 2;

        auto tmp{m_pGatherer->resultsSoFar()};
        for (auto & row : tmp)
        {
            row.push_back(m_nPathsDoneConvergence);
            // I wish to reverse the results, Npaths : value
            std::reverse(row.begin(), row.end());
            m_results.push_back(row);
        }
    }
}

} // namespace der
