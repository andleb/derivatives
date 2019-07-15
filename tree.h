/** \file tree.h
 * \author Andrej Leban
 * \date 7/2019
 * Ch. 8 Trees
 */


#ifndef TREE_H
#define TREE_H

#include <utility>
#include <vector>

#include <common/btree.h>

#include "treeproduct.h"
#include "parameters.h"

namespace der {

class Tree
{
public:
    Tree(double p_S0, const Parameters & p_r, const Parameters & p_d, double p_sigma, size_t p_nSteps, double p_expiryTime);

    //! \brief price
    //! Performs the pricing on the tree. The product evaluated is a read only parameter
    //! \param p_product
    //! \return
    double price(const TreeProduct & p_product);

private:
    cm::bTree<std::pair<double, double>> m_tree;

    std::vector<double> m_discountFactors;

    double m_S0;
    // we support variable interest and dividend rates
    Parameters m_r;
    Parameters m_d;
    double m_sigma;
    double m_expiryTime;

    double m_deltaT;
};

} // namespace der

#endif // TREE_H
