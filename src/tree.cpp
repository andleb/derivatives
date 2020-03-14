/** \file tree.cpp
 * \author Andrej Leban
 * \date 7/2019
 */

#include <algorithm>
#include <cmath>

#include "tree.h"

namespace der
{

Tree::Tree(size_t p_nSteps, double p_S0, const Parameters & p_r, const Parameters & p_d, double p_sigma, double p_expiryTime)
    // including the initial step
    : m_tree(p_nSteps + 1)
    // no discounting the last level, hence no + 1
    , m_discountFactors(p_nSteps)
    , m_S0(p_S0)
    , m_r(p_r)
    , m_d(p_d)
    , m_sigma(p_sigma)
    , m_expiryTime(p_expiryTime)
    , m_deltaT(p_expiryTime / p_nSteps)
{
    // populate the tree
    double logS0 = std::log(m_S0);
    double time = 0.0;
    // the spot
    double logS = logS0;

    // index to the first element in the second half of the level
    size_t cumulative;


    double sqrtDeltaT = std::sqrt(m_deltaT);

    // root node
    m_tree[0].first = std::exp(logS);


    // pre-calculate the spots
    for (long level = 1; level <= static_cast<long>(m_tree.numLevels()); ++level)
    {
        cumulative = m_tree.left_boundary(static_cast<size_t>(level));

        time = level * m_deltaT;
        logS = logS0 + m_r.integral(0, time) - m_d.integral(0, time) - 0.5 * m_sigma * m_sigma * time;

        // fill the tree abreast
        size_t k = 0;
        // j must jump from -1 to +1 for each move
        for (long j = -level; j <= level; j = j + 2, ++k)
        {
            m_tree[cumulative + k].first = std::exp(logS + j * m_sigma * sqrtDeltaT);
        }

    }

    // pre-calculate the discount factors
    // TODO: check this
    for (size_t level = 0; level < p_nSteps; ++level)
    {
        m_discountFactors[level] = std::exp(-1.0 * m_r.integral(level * m_deltaT, (level + 1) * m_deltaT));
    }
}

double Tree::price(const TreeProduct & p_product)
{
    if (std::abs(p_product.expiryTime() - m_expiryTime) > 1e-3)
    {
        throw std::runtime_error("Cannot re-use this tree instance, expiry time changed.");
    }

    // fill-out the last level
    for (size_t i = m_tree.left_boundary(m_tree.numLevels()); i <= m_tree.right_boundary(m_tree.numLevels()); ++i)
    {
        m_tree[i].second = p_product.payoff(m_tree[i].first);
    }

    // re-trace back to root
    size_t i;
    for (long level = static_cast<long>(m_tree.numLevels()) - 1; level >= 0; level--)
    {
        double t = level * m_deltaT;
        double discFutValue;

        // evaluate abreast
        for (i = m_tree.left_boundary(static_cast<size_t>(level)); i <= m_tree.right_boundary(static_cast<size_t>(level)); ++i)
        {
            // discounted expectation of the value (not spot!) at next step
            // FIXME:
            discFutValue = m_discountFactors[static_cast<size_t>(level)] * 0.5 *
                           (m_tree[m_tree.goDownLeft(i)].second + m_tree[m_tree.goDownRight(i)].second);
            // the value at this node is product-dependent
            m_tree[i].second = p_product.value(m_tree[i].first, t, discFutValue);
        }
    }

    // the price of a derivative is its current value
    return m_tree[0].second;
}

} // namespace der
