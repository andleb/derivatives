/** \file exoticengine.cpp
 * \author Andrej Leban
 * \date 4/2019
 */

#include "exoticengine.h"

namespace der
{

ExoticEngine::~ExoticEngine() = default;

ExoticEngine::ExoticEngine(const PathDependent & p_product, Parameters p_r)
    : m_pProduct(p_product.clone())
    , m_r(std::move(p_r)) // the times get mutated in place to discounts
    , m_discounts(m_pProduct->possibleCashFlowTimes())
    , m_cashflows(m_pProduct->maxNumberOfCashFlows())
{
    precalculate();
}

ExoticEngine::ExoticEngine(std::unique_ptr<PathDependent> p_product, Parameters p_r)
    : m_pProduct(std::move(p_product))
    , m_r(std::move(p_r))
    , m_discounts(m_pProduct->possibleCashFlowTimes())
    , m_cashflows(m_pProduct->maxNumberOfCashFlows())
{
    precalculate();
}

void ExoticEngine::precalculate()
{
    // fill out the discounts using the in-place discrete cashflow times
    std::for_each(m_discounts.begin(), m_discounts.end(), [this](auto & elem) { elem = std::exp(-m_r.integral(0.0, elem)); });
}

ExoticEngine::ExoticEngine(const ExoticEngine & p_other)
    : m_pProduct(p_other.m_pProduct->clone()), m_r(p_other.m_r), m_discounts(p_other.m_discounts), m_cashflows(p_other.m_cashflows)
{}

ExoticEngine & ExoticEngine::operator=(const ExoticEngine & p_other)
{
    if (this != &p_other)
    {
        m_pProduct = p_other.m_pProduct->clone();
        m_r = p_other.m_r;
        m_discounts = p_other.m_discounts;
        m_cashflows = p_other.m_cashflows;
    }

    return *this;
}

double ExoticEngine::doOnePath(const std::vector<double> & p_spots) const
{
    // spots are passed into the products
    // the price of one path is the weighted(by the discounts) sum of amounts

    m_cashflows = m_pProduct->cashFlows(p_spots, std::move(m_cashflows));

    double val = 0.0;

    // using TimeIndex instead of scalar product cash-flows * discounts
    // since discounts are pre-calculated and sized to MAX number of cash-flows
    // - the actual can be lower
    for (const auto & cashflow : m_cashflows)
    {
        val += cashflow.amount * m_discounts[cashflow.timeIndex];
    }

    return val;
}

void ExoticEngine::doSimulation(StatisticsBase & p_gatherer, size_t p_numberOfPaths) const
{
    // spots is moved around and reused at each path
    std::vector<double> spots(m_pProduct->lookAtTimes().size());

    double value;

    for (size_t i = 0; i < p_numberOfPaths; ++i)
    {
        spots = path(std::move(spots));
        value = doOnePath(spots);
        // it is the statistic gatherer's responsibility to sum the paths' results up
        p_gatherer.dumpOneResult(value);
    }
}

} // namespace der
