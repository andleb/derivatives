/** \file statistics.h
 * \author Andrej Leban
 * \date 2/2019
 *
 * An abstract stats gatherer
 */

#ifndef STATISTICS_H
#define STATISTICS_H

#include <memory>
#include <vector>

namespace der
{

class StatisticsBase
{
public:
    StatisticsBase() = default;
    StatisticsBase(const StatisticsBase &) = default;
    StatisticsBase(StatisticsBase &&) = default;
    StatisticsBase & operator=(const StatisticsBase &) = default;
    StatisticsBase & operator=(StatisticsBase &&) = default;
    virtual ~StatisticsBase();

    virtual std::unique_ptr<StatisticsBase> clone() const = 0;

    virtual std::vector<std::vector<double>> resultsSoFar() const = 0;
    virtual size_t simsSoFar() const = 0;

    virtual void dumpOneResult(double val) = 0;
};

class StatisticsMean : public StatisticsBase
{
public:
    StatisticsMean() = default;
    // used in clone
    StatisticsMean(double runningSum, size_t paths)
        : m_runningSum(runningSum)
        , m_nPathsDone(paths)
    {}

    std::unique_ptr<StatisticsBase> clone() const override;

    std::vector<std::vector<double>> resultsSoFar() const override;
    size_t simsSoFar() const override;
    void dumpOneResult(double val) override;

private:
    double m_runningSum{0.0};
    size_t m_nPathsDone{0};
};


//! \brief The ConvergenceTable class
//! Uses an aggregated statistics gatherer
class ConvergenceTable : public StatisticsBase
{
public:
    ConvergenceTable(std::unique_ptr<StatisticsBase> p_pGatherer)
        : m_pGatherer(std::move(p_pGatherer))
    {}

    ConvergenceTable() = default;
    ConvergenceTable(const ConvergenceTable &) = default;
    ConvergenceTable(ConvergenceTable &&) = default;
    ConvergenceTable & operator=(const ConvergenceTable &) = default;
    ConvergenceTable & operator=(ConvergenceTable &&) = default;
    ~ConvergenceTable() override;

    std::unique_ptr<StatisticsBase> clone() const override;
    std::vector<std::vector<double>> resultsSoFar() const override;
    size_t simsSoFar() const override;

    void dumpOneResult(double val) override;

private:
    std::shared_ptr<StatisticsBase> m_pGatherer{};

    size_t m_count{2};
    // used for caching
    size_t m_nPathsDoneConvergence{0};

    std::vector<std::vector<double>> m_results{};
};

} // namespace der

#endif // STATISTICS_H
