#include "statistics.h"

namespace der {

StatisticsBase::~StatisticsBase() = default;

std::unique_ptr<StatisticsBase> StatisticsMean::clone() const
{
    return std::make_unique<StatisticsMean>(m_runningSum, m_nPathsDone);
}

std::vector<std::vector<double> > StatisticsMean::resultsSoFar() const
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
    return std::make_unique<ConvergenceTable> (*this);
}

std::vector<std::vector<double> > ConvergenceTable::resultsSoFar() const
{
    auto ret = m_results;

    // This means the "cache" hasn't been updated ...
    if(m_nPathsDone * 2 != m_count)
    {
        auto tmp{m_pGatherer->resultsSoFar()};
        for(auto & row : tmp)
        {
            row.push_back(m_nPathsDone);
            ret.push_back(row);
        }
    }

    return ret;
}

size_t ConvergenceTable::simsSoFar() const
{
    return m_nPathsDone;
}

void ConvergenceTable::dumpOneResult(double val)
{
    m_pGatherer->dumpOneResult(val);
    m_nPathsDone++;

    if(m_nPathsDone == m_count)
    {
        m_count *= 2;

        auto tmp{m_pGatherer->resultsSoFar()};
        for(auto & row : tmp)
        {
            row.push_back(m_nPathsDone);
            m_results.push_back(row);
        }
    }
}

} // namespace der
