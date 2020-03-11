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

//! \brief The interface ABC for stats gatherers.
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

    //! \brief Returns gathered results.
    //! \return a matrix - allows for greatest flexibility in the sub-classes.
    virtual std::vector<std::vector<double>> resultsSoFar() const = 0;
    //! \brief The number of simulations done.
    virtual size_t simsSoFar() const = 0;
    //! \brief The input method.
    virtual void dumpOneResult(double val) = 0;
};

class StatisticsMean : public StatisticsBase
{
public:
    StatisticsMean() = default;
    // used in clone
    StatisticsMean(double runningSum, size_t paths);

    std::unique_ptr<StatisticsBase> clone() const override;

    //! \brief Returns gathered results.
    //! The inner vector is 1-element in this case.
    std::vector<std::vector<double>> resultsSoFar() const override;
    //! \brief The number of simulations done.
    size_t simsSoFar() const override;
    //! \brief The input method.
    void dumpOneResult(double val) override;

private:
    double m_runningSum{0.0};
    size_t m_nPathsDone{0};
};

//! \brief The ConvergenceTable class.
//! Uses an aggregated statistics gatherer.
//! Stores the Monte-Carlo convergence results in \f$2^N\f$ intervals, either on input
//! or when getting the results.
class ConvergenceTable : public StatisticsBase
{
public:
    ConvergenceTable() = default;
    ConvergenceTable(std::unique_ptr<StatisticsBase> p_pGatherer);
    ConvergenceTable(const ConvergenceTable &) = default;
    ConvergenceTable(ConvergenceTable &&) = default;
    ConvergenceTable & operator=(const ConvergenceTable &) = default;
    ConvergenceTable & operator=(ConvergenceTable &&) = default;
    ~ConvergenceTable() override;

    std::unique_ptr<StatisticsBase> clone() const override;

    //! \brief Returns gathered results.
    //! \return The matrix returned is comprised of 2 columns, \f$N_{sims}$\f : results.
    std::vector<std::vector<double>> resultsSoFar() const override;
    //! \brief The number of simulations done.
    size_t simsSoFar() const override;
    //! \brief The input method.
    void dumpOneResult(double val) override;

private:
    std::shared_ptr<StatisticsBase> m_pGatherer{};

    //! \brief Milestones for convergence, i.e. \f$2^N\f$
    size_t m_count{2};
    //! \brief The actual paths done
    size_t m_nPathsDone{0};

    std::vector<std::vector<double>> m_results{};
};

} // namespace der

#endif // STATISTICS_H
