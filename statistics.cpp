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

void StatisticsMean::dumpOneResult(double val)
{
    m_runningSum += val;
    ++m_nPathsDone;
}

} // namespace der
