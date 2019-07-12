/** \file exoticengine.h
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#ifndef EXOTICENGINE_H
#define EXOTICENGINE_H

#include <algorithm>
#include <cmath>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

#include "parameters.h"
#include "pathdependent.h"
#include "random.h"
#include "statistics.h"

namespace der
{

//TODO: remove default ctor, dtors after development

//! \brief The ExoticEngine class
//! the central hub for calculation of the exotic derivative's price
class ExoticEngine
{
public:
    ExoticEngine() = default;
    ExoticEngine(const PathDependent & p_product, Parameters p_r);
    ExoticEngine(const ExoticEngine &);
    ExoticEngine(ExoticEngine &&) noexcept = default;
    ExoticEngine & operator=(const ExoticEngine &);
    ExoticEngine & operator=(ExoticEngine &&) noexcept = default;
    virtual ~ExoticEngine() = default;

    //! \brief path
    //! \param p_spots
    //! \return
    //! This is a pure virtual whose implementation will signify a choice of model & stochastic process
    //! Returns an array of spot values
    virtual std::vector<double> path(std::vector<double> && p_spots) const = 0;

    double doOnePath(const std::vector<double> & p_spots) const;
    void doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const;

protected:
    std::unique_ptr<PathDependent> m_pProduct {nullptr};
    Parameters m_r;
    std::vector<double> m_discounts;

    // scratchpad
    mutable std::vector<CashFlow> m_cashflows;
};

template <typename Generator>
class ExoticBSEngine : public ExoticEngine
{
public:
    ExoticBSEngine() = default;
    ExoticBSEngine(const PathDependent & p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0);
    //    ExoticBSEngine(const ExoticBSEngine &) = default;
    //    ExoticBSEngine(ExoticBSEngine &&) noexcept = default;
    //    ExoticBSEngine & operator=(const ExoticBSEngine &) = default;
    //    ExoticBSEngine & operator=(ExoticBSEngine &&) noexcept = default;
    ~ExoticBSEngine() override = default;

    std::vector<double> path(std::vector<double> && p_spots) const override;

protected:
    Generator m_generator;

    const Parameters m_d{};
    const Parameters m_vol{};
    const double m_logS0{0.0};

private:
    mutable std::vector<double> m_times;

    mutable std::vector<double> m_drifts;
    mutable std::vector<double> m_stds;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ExoticBSEngine

template <typename Generator>
ExoticBSEngine<Generator>::ExoticBSEngine(const PathDependent & p_product, Parameters p_r, Parameters p_d,
                                                   Parameters p_vol, double p_S0)
    : ExoticEngine(p_product, p_r)
    , m_d(std::move(p_d))
    , m_vol(std::move(p_vol))
    , m_logS0(std::log(p_S0))
    , m_times(m_pProduct->possibleCashFlowTimes())
    , m_drifts(m_times.size())
    , m_stds(m_times.size())
{
    // pre-calculate the drifts and the standard deviations
    m_stds[0] = std::sqrt(m_vol.integralSquare(0, m_times[0]));
    // TODO: why do I need to be explicit about accessing the base class members in the body?
    //        m_drifts[0] = m_r.Integral(0.0, m_times[0]);
    m_drifts[0] = m_r.integral(0.0, m_times[0]) - m_d.integral(0.0, m_times[0])
                  - 0.5 * m_stds[0] * m_stds[0];

    for (size_t i = 1; i < m_drifts.size(); ++i)
    {
        m_stds[i] = std::sqrt(m_vol.integralSquare(m_times[i-1], m_times[i]));
         m_drifts[0] = m_r.integral(m_times[i-1], m_times[i]) - m_d.integral(m_times[i-1], m_times[i])
                      - 0.5 * m_stds[i] * m_stds[i];
    }
}

template <typename Generator>
std::vector<double> ExoticBSEngine<Generator>::path(std::vector<double> && p_spots) const
{
    p_spots = m_generator.gaussians(std::move(p_spots));

    double logS = m_logS0;

    // evolve the path, overwrites the gaussian spots in-place with the BS spots
    for (size_t i = 0; i < m_times.size(); ++i)
    {
        logS += m_drifts[i];
        logS += m_stds[i] * p_spots[i];

        p_spots[i] = std::exp(logS);
    }

    return std::move(p_spots);
}

} // namespace der

#endif // EXOTICENGINE_H
