/** \file treeproduct.h
 * \author Andrej Leban
 * \date 7/2019
 *
 * Classes representing options to be priced with trees.
 */

#ifndef TREEPRODUCT_H
#define TREEPRODUCT_H

#include <vector>

#include "payoff.h"

namespace der
{

//! \brief Abstract interface to Tree-priced option classes.
//! Knows nothing about interest rates, discretization etc.
class TreeProduct
{
public:
    TreeProduct(double p_expiryTime, const Payoff & p_payoff);
    TreeProduct(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff);

    TreeProduct(const TreeProduct & p_other);
    TreeProduct(TreeProduct && p_other) = default;
    TreeProduct & operator=(const TreeProduct & p_other);
    TreeProduct & operator=(TreeProduct && p_other) = default;
    virtual ~TreeProduct();

    virtual std::unique_ptr<TreeProduct> clone() const = 0;

    //! \brief expiryTime.
    double expiryTime() const;
    //! \brief The final payoff of the product @ \a expiryTime.
    //! \param p_spot
    //! \return
    double payoff(double p_spot) const;
    //! \brief The value of the product @ \p p_t, possibly dependent on (discounted!) \p p_futureValue.
    //! \param p_spot
    //! \param p_t
    //! \param p_futureValue
    //! \return
    virtual double value(double p_spot, double p_t, double p_futureValue) const = 0;

protected:
    double m_expiryTime{0};
    // only to be inherited
    std::unique_ptr<Payoff> m_pPayoff{nullptr};
};

//! \brief An European tree-priced option.
class TreeEuropean : public TreeProduct
{
public:
    TreeEuropean(double p_expiryTime, const Payoff & p_payoff);
    TreeEuropean(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff);

    virtual std::unique_ptr<TreeProduct> clone() const override;

    //! \brief value
    //! \param p_spot
    //! \param p_t
    //! \param p_futureValue
    //! \return
    double value(double p_spot, double p_t, double p_futureValue) const override;
};

//! \brief An American tree-priced option.
class TreeAmerican : public TreeProduct
{
public:
    TreeAmerican(double p_expiryTime, const Payoff & p_payoff);
    TreeAmerican(double p_expiryTime, std::unique_ptr<Payoff> p_pPayoff);

    virtual std::unique_ptr<TreeProduct> clone() const override;

    //! \brief value
    //! \param p_spot
    //! \param p_t
    //! \param p_futureValue
    //! \return
    double value(double p_spot, double p_t, double p_futureValue) const override;
};

} // namespace der

#endif // TREEPRODUCT_H
