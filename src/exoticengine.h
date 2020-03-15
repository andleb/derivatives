/** \file exoticengine.h
 * \author Andrej Leban
 * \date 4/2019
 *
 * A generalized option pricing engine.
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

//! \brief A generalized option pricing engine.
//! The process is provided by sub-classing this class and implementing the \p path method.
class ExoticEngine
{
public:
    ExoticEngine(const PathDependent & p_product, Parameters p_r);
    ExoticEngine(std::unique_ptr<PathDependent> p_product, Parameters p_r);

    ExoticEngine(const ExoticEngine &);
    ExoticEngine(ExoticEngine &&) noexcept = default;
    ExoticEngine & operator=(const ExoticEngine &);
    ExoticEngine & operator=(ExoticEngine &&) noexcept = default;
    virtual ~ExoticEngine();

    //! \brief This is a pure virtual whose implementation will signify a choice of stochastic process.
    //! One could implement an exercise strategy here as well.
    //! \param p_spots
    //! \return An array of spot values.
    virtual std::vector<double> path(std::vector<double> && p_spots) const = 0;

    //! \brief Evaluates one path of the simulation, with the spot values provided.
    //! \param p_spots
    //! \return
    double doOnePath(const std::vector<double> & p_spots) const;

    //! \brief Performs the whole simulation, i.e. evaluates all the paths.
    //! \param p_gatherer
    //! \param p_numberOfPaths
    void doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const;

protected:
    std::unique_ptr<PathDependent> m_pProduct{nullptr};

    Parameters m_r;
    //! \brief pre-calculated discounts
    std::vector<double> m_discounts;

    //! \brief used as scratchpad for in-place calculations
    mutable std::vector<CashFlow> m_cashflows;

private:
    //! \brief Pre-calculates the discount factors \p m_discounts given the interest rate \p m_r.
    void precalculate();
};

//! \brief A concrete implementation of an options pricing engine using a Black-Scholes (i.e. log-Wiener) process.
template <typename Generator>
class ExoticBSEngine : public ExoticEngine
{
public:
    ExoticBSEngine(const PathDependent & p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0);
    ExoticBSEngine(std::unique_ptr<PathDependent> p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0);

    ExoticBSEngine(const ExoticBSEngine &) = default;
    ExoticBSEngine(ExoticBSEngine &&) = default;
    ExoticBSEngine & operator=(const ExoticBSEngine &) = default;
    ExoticBSEngine & operator=(ExoticBSEngine &&) noexcept = default;
    ~ExoticBSEngine() override = default;

    //! \brief Implements the Black-Scholes process for the spot with the class' parameters.
    //! \param p_spots
    //! \return Modified \p p_spots in-place.
    std::vector<double> path(std::vector<double> && p_spots) const override;

protected:
    //! \brief The RNG provided.
    Generator m_generator;

    const Parameters m_d{};
    const Parameters m_vol{};
    const double m_logS0{0.0};

private:
    //! \brief Pre-calculates the drifts \p m_drifts and stdev's \p m_stds given the parameters above.
    void precalculate();

    // helpers
    //! \brief Cached relevant spot times to the product's cash-flow function.
    mutable std::vector<double> m_times;

    // pre-calculated
    mutable std::vector<double> m_drifts;
    mutable std::vector<double> m_stds;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ExoticBSEngine

template <typename Generator>
ExoticBSEngine<Generator>::ExoticBSEngine(const PathDependent & p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0)
    : ExoticEngine(p_product, p_r)
    , m_d(std::move(p_d))
    , m_vol(std::move(p_vol))
    , m_logS0(std::log(p_S0))
    , m_times(m_pProduct->lookAtTimes())
    , m_drifts(m_times.size())
    , m_stds(m_times.size())
{
    precalculate();
}

template <typename Generator>
ExoticBSEngine<Generator>::ExoticBSEngine(std::unique_ptr<PathDependent> p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0)
    : ExoticEngine(std::move(p_product), p_r)
    , m_d(std::move(p_d))
    , m_vol(std::move(p_vol))
    , m_logS0(std::log(p_S0))
    , m_times(m_pProduct->lookAtTimes())
    , m_drifts(m_times.size())
    , m_stds(m_times.size())
{
    precalculate();
}

template <typename Generator>
void ExoticBSEngine<Generator>::precalculate()
{
    // pre-calculate the drifts and the standard deviations
    m_stds[0] = std::sqrt(m_vol.integralSquare(0, m_times[0]));
    m_drifts[0] = m_r.integral(0.0, m_times[0]) - m_d.integral(0.0, m_times[0]) - 0.5 * m_stds[0] * m_stds[0];

    for (size_t i = 1; i < m_times.size(); ++i)
    {
        m_stds[i] = std::sqrt(m_vol.integralSquare(m_times[i - 1], m_times[i]));
        m_drifts[i] = m_r.integral(m_times[i - 1], m_times[i]) - m_d.integral(m_times[i - 1], m_times[i]) - 0.5 * m_stds[i] * m_stds[i];
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
