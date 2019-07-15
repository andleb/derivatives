/** \file treeproduct.h
 * \author Andrej Leban
 * \date 7/2019
 * Ch. 8 Trees
 */

#ifndef TREEPRODUCT_H
#define TREEPRODUCT_H

#include <vector>

#include "payoff2.h"

namespace der
{

class TreeProduct
{
public:
    TreeProduct(double p_expiryTime, const Payoff2 & p_payoff);
    TreeProduct(double p_expiryTime, std::unique_ptr<Payoff2> p_pPayoff);

    TreeProduct(const TreeProduct & p_other);
    TreeProduct(TreeProduct && p_other) = default;
    TreeProduct & operator=(const TreeProduct & p_other);
    TreeProduct & operator=(TreeProduct && p_other) = default;
    virtual ~TreeProduct();

    double expiryTime() const;
    double payoff(double p_spot) const;
    virtual double value(double p_spot, double p_t, double p_futureValue) const = 0;

protected:
    double m_expiryTime{0};
    std::unique_ptr<Payoff2> m_pPayoff{nullptr};
};

class TreeAmerican : public TreeProduct
{
public:
    TreeAmerican(double p_expiryTime, const Payoff2 & p_payoff);
    TreeAmerican(double p_expiryTime, std::unique_ptr<Payoff2> p_pPayoff);

    double value(double p_spot, double p_t, double p_futureValue) const override;
};

class TreeEuropean : public TreeProduct
{
public:
    TreeEuropean(double p_expiryTime, const Payoff2 & p_payoff);
    TreeEuropean(double p_expiryTime, std::unique_ptr<Payoff2> p_pPayoff);

    double value(double p_spot, double p_t, double p_futureValue) const override;
};

} // namespace der

#endif // TREEPRODUCT_H
