/** \file tree.h
 * \author Andrej Leban
 * \date 7/2019
 *
 * An option pricing tree class.
 */


#ifndef TREE_H
#define TREE_H

#include <utility>
#include <vector>

#include <common/trees.h>

#include "parameters.h"
#include "treeproduct.h"

namespace der
{

//! \brief Handles the discretization of the pricing.
//! The prices are supplied by a separate product class.
//! Stores the entire tree in a static array.
class binomialTree
{
public:
    //! \brief Constructor.
    //! \param p_nSteps - number of steps until \p p_expiryTime.
    //! \param p_S0 - Spot @ time 0.
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_sigma - The (constant) volatility.
    //! \param p_expiryTime - The option's expiry time.
    binomialTree(size_t p_nSteps, double p_S0, Parameters p_r, Parameters p_d, double p_sigma, double p_expiryTime);

    //! \brief Performs the pricing on the tree. The product evaluated is a read-only parameter.
    //! \param p_product
    //! \return the \p p_product's price
    double price(const TreeProduct & p_product);

private:
    //! \brief m_tree
    //! The tree structure, the first element of the pair holds the evolution of the spot, the second is the placeholder for the
    //! option value at that node and gets overwritten if \a price is called multiple times with multiple products on the same
    //! tree instance.
    cm::recombinantBTree<std::pair<double, double>> m_tree;
    using treeType = decltype(m_tree);

    std::vector<double> m_discountFactors;

    double m_S0;
    // we support variable interest and dividend rates, but not volatility! - that would severely complicate the tree.
    Parameters m_r;
    Parameters m_d;
    double m_sigma;
    double m_expiryTime;

    double m_deltaT;
};

//! \brief Handles the discretization of the pricing.
//! The prices are supplied by a separate product class.
//! Stores the entire tree in a static array.
class trinomialTree
{
public:
    //! \brief Constructor. Trinomial stochastic process trees form a family, parameterized by \p p_p0 -
    //! probability of no move on time step. The size \f$a\f$ and probability \f$p\f$ of a move in either direction are
    //! therefore uniquely determined: \f$p = \frac{1 - p_0}{2};\; 2 a^2 p = 1\f$.
    //! \param p_nSteps - number of steps until \p p_expiryTime.
    //! \param p_p0 - Probability of no move.
    //! \param p_S0 - Spot @ time 0.
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_sigma - The (constant) volatility.
    //! \param p_expiryTime - The option's expiry time.
    trinomialTree(size_t p_nSteps, double p_p0, double p_S0, Parameters p_r, Parameters p_d, double p_sigma, double p_expiryTime);

    //! \brief Performs the pricing on the tree. The product evaluated is a read-only parameter.
    //! \param p_product
    //! \return the \p p_product's price
    double price(const TreeProduct & p_product);

private:
    //! \brief m_tree
    //! The tree structure, the first element of the pair holds the evolution of the spot, the second is the placeholder for the
    //! option value at that node and gets overwritten if \a price is called multiple times with multiple products on the same
    //! tree instance.
    cm::recombinantTTree<std::pair<double, double>> m_tree;
    using treeType = decltype(m_tree);

    std::vector<double> m_discountFactors;


    //! \name Tree parametrization.
    //!@{
    double m_p0;
    constexpr double p() const { return 0.5 * (1. - m_p0); }
    double a() const { return std::sqrt(1 / (2 * p())); }
    //!@}


    double m_S0;
    // we support variable interest and dividend rates, but not volatility! - that would severely complicate the tree.
    Parameters m_r;
    Parameters m_d;
    double m_sigma;
    double m_expiryTime;

    double m_deltaT;
};

} // namespace der

#endif // TREE_H
