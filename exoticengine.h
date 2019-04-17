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
#include "random.h"
#include "statistics.h"

namespace der
{

class PathDependent;

//! \brief The ExoticEngine class
//! the central hub for calculation of the exotic derivative's price
template <typename Product>
class ExoticEngine
{
public:
    ExoticEngine() = default;
    ExoticEngine(Product p_product, Parameters p_r);
    ExoticEngine(const ExoticEngine &) = default;
    ExoticEngine(ExoticEngine &&) noexcept = default;
    ExoticEngine & operator=(const ExoticEngine &) = default;
    ExoticEngine & operator=(ExoticEngine &&) noexcept = default;
    virtual ~ExoticEngine() = default;

    //! \brief path
    //! \param p_spots
    //! \return
    //! This is a pure virtual whose implementation will signify a choice of model & stochastic process
    //! Returns an array of spot values
    virtual std::vector<double> path(std::vector<double> && p_spots) const = 0;

    double doOnePath(const std::vector<double> & p_spots) const;
    double doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const;

protected:
    std::unique_ptr<Product> m_pProduct;
    Parameters m_r;
    std::vector<double> m_discounts;

    // scratchpad
    mutable std::vector<double> m_cashflows;
};


//TODO: remove default ctor, dtors after development
template <typename Product, typename Generator>
    class ExoticBSEngine : public ExoticEngine<Product>
{
public:
    ExoticBSEngine() = default;
    ExoticBSEngine(const Product & p_product, Parameters p_r, Parameters p_d, Parameters p_vol, double p_S0);
//    ExoticBSEngine(const ExoticBSEngine &) = default;
//    ExoticBSEngine(ExoticBSEngine &&) noexcept = default;
//    ExoticBSEngine & operator=(const ExoticBSEngine &) = default;
//    ExoticBSEngine & operator=(ExoticBSEngine &&) noexcept = default;
    ~ExoticBSEngine() override = default;

    std::vector<double> path(std::vector<double> && p_spots) const override;

protected:
    Generator m_generator;

    const Parameters m_d {};
    const Parameters m_vol {};
    const double m_logS0 {0.0};

private:
    mutable std::vector<double> m_times;

    mutable std::vector<double> m_drifts;
    mutable std::vector<double> m_stds;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Product>
ExoticEngine<Product>::ExoticEngine(Product p_product, Parameters p_r)
    : m_pProduct(std::make_unique(std::move(p_product)))
    , m_r(std::move(p_r))
    , m_discounts(m_pProduct->possibleCashFlowTimes())
    , m_cashflows(m_pProduct->maxNumberOfCashFlows())
{
    // fill out the discounts using the in-place discrete cashflow times
    std::for_each(m_discounts.begin(), m_discounts.end(), [this](auto & elem) {
        elem = std::exp(-m_r.integral(0.0, elem));
    });
}

template <typename Product>
double ExoticEngine<Product>::doOnePath(const std::vector<double> & p_spots) const
{
    size_t numflows;

    // spots are passed into the products
    // the price of one path is the weighted(by the discounts) sum of amounts
    //TODO: does this take advantage of the move?
    std::tie(m_cashflows, numflows) = m_pProduct->cashFlows(p_spots, std::move(m_cashflows));

    //TODO: better way to handle this timeIndex stuff
    double val = 0.0;

    for (size_t i = 0; i < numflows; ++i)
    {
        val += m_cashflows[i].amount * m_discounts[m_cashflows[i].timeIndex];
    }

    return val;
}

template <typename Product>
double ExoticEngine<Product>::doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const
{
    std::vector<double> spots{m_pProduct->lookAtTimes().size()};

    // should be done in doOnePath->Product::cashFlows otherwise
    // Done in constructor, product should guarantee this is static!
    //    m_cashflows.resize(m_product.maxNumberOfCashFlows());

    double value;

    for (auto i = 0; i < p_numberOfPaths; ++i)
    {
        spots = path(std::move(spots));
        value = doOnePath(spots);
        p_gatherer.dumpOneResult(value);
    }
}


//ExoticBSEngine

template<typename Product, typename Generator>
ExoticBSEngine<Product, Generator>::ExoticBSEngine( const Product & p_product, Parameters p_r, Parameters p_d,
                                                   Parameters p_vol, double p_S0)
    : ExoticEngine<Product>(p_product, p_r)
    , m_d(std::move(p_d))
    , m_vol(std::move(p_vol))
    , m_logS0(std::log(p_S0))
    , m_times(std::move(ExoticEngine<Product>::m_pProduct->possibleCashFlowTimes()))
    , m_drifts(m_times.size())
    , m_stds(m_times.size())
    {
        // pre-calculate the drifts and the standard deviations

        // TODO: why do I need to be explicit about accessing the base class members in the body?
        m_drifts[0] = ExoticEngine<Product>::m_r.Integral(0.0, m_times[0]);
//        m_drifts[0] = m_r.Integral(0.0, m_times[0]);
        m_stds[0] = std::sqrt(m_vol.integralSquare(0, m_times[0]));

    }

template <typename Product, typename Generator>
std::vector<double> ExoticBSEngine<Product, Generator>::path(std::vector<double> && p_spots) const
{
    p_spots = m_generator.gaussians(std::move(p_spots));

    double logS = m_logS0;

    // evolve the path, overwrites the gaussian spots in-place with the BS spots
    for(auto i=0; i < m_times.size(); ++i)
    {
        logS += m_drifts[i];
        logS += m_stds[i] * p_spots[i];

        p_spots[i] = std::exp(logS);
    }

    return std::move(p_spots);
}

} // namespace der

#endif // EXOTICENGINE_H
