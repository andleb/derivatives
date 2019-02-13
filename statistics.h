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
    virtual ~StatisticsBase();

    virtual std::unique_ptr<StatisticsBase> clone() const;

    virtual std::vector<std::vector<double>> resultsSoFar() const = 0;
    virtual void dumpOneResult(double val) = 0;
};

class StatisticsMean : public StatisticsBase
{
public:
    StatisticsMean() = default;
    StatisticsMean(double runningSum, size_t paths)
        : m_runningSum(runningSum)
        , m_nPathsDone(paths)
    {}

    std::unique_ptr<StatisticsBase> clone() const override;

    std::vector<std::vector<double>> resultsSoFar() const override;
    void dumpOneResult(double val) override;

private:
    double m_runningSum{0.0};
    size_t m_nPathsDone{0};
};

} // namespace der

#endif // STATISTICS_H
