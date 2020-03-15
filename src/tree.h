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

#include <common/btree.h>

#include "parameters.h"
#include "treeproduct.h"

namespace der
{

//! \brief Handles the discretization of the pricing.
//! The prices are supplied by a separate product class.
//! Stores the entire tree in a static array.
class Tree
{
public:
    Tree(size_t p_nSteps, double p_S0, Parameters p_r, Parameters p_d, double p_sigma, double p_expiryTime);

    //! \brief Performs the pricing on the tree. The product evaluated is a read-only parameter.
    //! \param p_product
    //! \return the \p p_product's price
    double price(const TreeProduct & p_product);

private:
    //! \brief m_tree
    //! The tree structure, the first element of the pair holds the evolution of the spot, the second is the placeholder for the
    //! option value at that node and gets overwritten of \a price is called multiple times with multiple products on the same
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

} // namespace der

#endif // TREE_H
