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
#include "statistics.h"

namespace der
{

class PathDependent;

//! \brief The ExoticEngine class
//! the central hub for calculation of the exotic derivative's price
template<typename Product>
class ExoticEngine
{
public:
    ExoticEngine() = default;
    ExoticEngine(const Product & p_product, Parameters p_r);
    ExoticEngine(const ExoticEngine & ) = default;
    ExoticEngine(ExoticEngine &&) noexcept = default;
    ExoticEngine & operator=(const ExoticEngine &) = default;
    ExoticEngine & operator=(ExoticEngine &&) noexcept = default;
    ~ExoticEngine() = default;

    //! \brief path
    //! \param p_spots
    //! \return
    //! This is a pure virtual whhose implementation will signify a choice of model & stochastic process
    virtual std::vector<double> path(std::vector<double> && p_spots) const = 0;
    double doOnePath(const std::vector<double> & p_spots) const;
    double doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const;


private:
    Product m_product;
    Parameters m_r;
    std::vector<double> m_discounts;

    // scratchpatd
    mutable std::vector<double> m_cashflows;

    // TODO: don't pass around reference to a vector of cash flows
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Product>
ExoticEngine<Product>::ExoticEngine(const Product & p_product, Parameters p_r)
    : m_product(p_product)
    , m_r(std::move(p_r))
    , m_discounts(m_product.possibleCashFlowTimes())
    , m_cashflows(m_product.maxNumberOfCashFlows())
{
    // fill out the discounts using the in-place discrete cashflow times
    std::for_each(m_discounts.begin(), m_discounts.end(), [this] (auto & elem)
                  {
                      elem = std::exp(-m_r.integral(0.0, elem));
                  });
}

template<typename Product>
double ExoticEngine<Product>::doOnePath(const std::vector<double> & p_spots) const
{
    // the price of one path is the weighted(by the discounts) sum of amounts
    auto [m_cashflows, numflows] = m_product.cashFlows(p_spots, std::move(m_cashflows));

    //TODO: better way to handle this timeIndex stuff
    double val = 0.0;

    for(size_t i = 0; i < numflows; ++i)
    {
        val += m_cashflows[i].amount * m_discounts[m_cashflows[i].timeIndex];
    }

    return val;
}

template<typename Product>
double ExoticEngine<Product>::doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const
{

}

} // namespace der

#endif // EXOTICENGINE_H
