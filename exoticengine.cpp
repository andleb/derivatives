/** \file exoticengine.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include "exoticengine.h"

namespace der
{

ExoticEngine::ExoticEngine(const PathDependent & p_product, Parameters p_r)
    : m_pProduct(std::make_unique<PathDependent>(p_product))
    , m_r(std::move(p_r))
    , m_discounts(m_pProduct->possibleCashFlowTimes())
    , m_cashflows(m_pProduct->maxNumberOfCashFlows())
{
    // fill out the discounts using the in-place discrete cashflow times
    std::for_each(m_discounts.begin(), m_discounts.end(), [this](auto & elem) {
        elem = std::exp(-m_r.integral(0.0, elem));
    });
}

double ExoticEngine::doOnePath(const std::vector<double> & p_spots) const
{
    size_t numflows;

    // spots are passed into the products
    // the price of one path is the weighted(by the discounts) sum of amounts
    //TODO: does this take advantage of the move?
    m_cashflows = m_pProduct->cashFlows(p_spots, std::move(m_cashflows));

    //TODO: better way to handle this timeIndex stuff
    double val = 0.0;

    for (const auto & cashflow : m_cashflows)
    {
        val += cashflow.amount * m_discounts[cashflow.timeIndex];
    }

    return val;
}

void ExoticEngine::doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const
{
    std::vector<double> spots(m_pProduct->lookAtTimes().size());

    // should be done in doOnePath->Product::cashFlows otherwise
    // Done in constructor, product should guarantee this is static!
    //    m_cashflows.resize(m_product.maxNumberOfCashFlows());

    double value;

    for (size_t i = 0; i < p_numberOfPaths; ++i)
    {
        spots = path(std::move(spots));
        value = doOnePath(spots);
        p_gatherer.dumpOneResult(value);
    }
}


} // namespace der
