/** \file tree.cpp
 * \author Andrej Leban
 * \date 7/2019
 * Ch. 8 Trees
 */

#include <algorithm>
#include <cmath>

#include "tree.h"

namespace der
{

Tree::Tree(double p_S0, const Parameters & p_r, const Parameters & p_d, double p_sigma, size_t p_nSteps, double p_expiryTime)
            // including the initial step
    : m_tree(p_nSteps + 1)
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
   double time, logS;

   auto current = m_tree.root();

   for(size_t l = 0; l <= p_nSteps; ++l)
   {
       time = l * m_deltaT;
       logS = logS0 + m_r.integral(0, time) - m_d.integral(0, time) - 0.5 * m_sigma * m_sigma * time;



   }




   // precalculate the discount factors
   for(size_t l = 0; l < p_nSteps; ++l)
   {
       m_discountFactors[l] = std::exp(m_r.integral(l * m_deltaT, (l + 1) * m_deltaT));
   }
}

double Tree::price(const TreeProduct & p_product)
{

}

} // namespace der
