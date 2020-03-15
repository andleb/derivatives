/** \file derivatives.h
 * \author Andrej Leban
 * \date 11/2018
 *
 * The common library to the derivatives project
 */

#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <numeric>
#include <optional>
#include <random>

namespace der
{

// forward declarations
template <typename T>
T normalDistImpl(size_t p_seed = std::numeric_limits<size_t>::max());

//! \name Random numbers
//!@{

//! \brief Returns a normally distributed number of type \p T.
//! \return
template <typename T>
T normalDist()
{
    return normalDistImpl<T>();
}

//! \brief Returns a normally distributed number of type \p T, seeded once with p_seed.
//! \return
template <typename T>
T normalDist(size_t p_seed)
{
    return normalDistImpl<T>(p_seed);
}
//!@}

//! \name Mathematical functions
//!@{

//! //! \brief The PDF for the normalized Gaussian: \f$\frac{1}{\sqrt{2\pi}} e^{-\frac{1}{2}x^2}\f$
//! \return
double gaussianDensity(double p_x);

//! \brief Evaluates \f$\frac{1}{\sqrt{2\pi}} \int_{-\infty}^{x} e^{-\frac{1}{2}t^2} dt\f$
//! \return
double cumulativeGaussian(double p_x);

//! \brief Solves \f$\frac{1}{\sqrt{2\pi}} \int_{-\infty}^{x} e^{-\frac{1}{2}t^2} dt = y\f$ for \f$x\f$.
//! \return
double inverseCumulativeGaussian(double p_y);
//!@}

//! \name The Black-Scholes formulas.
//!@{

//! \brief An abstract Black-Scholes formula class.
class BSFormula
{
public:
    using p = std::optional<double>;

    //! \brief Caching of parameters - simulating named parameters.
    //! Pass default std::optional to not provide a value. C.f. \a operator()
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_T - The expiry time.
    //! \param p_sigma - The volatility.
    //! \param p_S0 - The spot @ time 0.
    //! \param p_K - The strike.
    BSFormula(p p_r, p p_d, p p_T, p p_sigma, p p_S0, p p_K);
    virtual ~BSFormula();

    //! \brief Prices the option. Pass default std::optional to not provide a value.
    //! Cached value will then be used - c.f. \a BSFormula().
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_T - The expiry time.
    //! \param p_sigma - The volatility.
    //! \param p_S0 - The spot @ time 0.
    //! \param p_K - The strike.
    //! \return The value of the option.
    virtual double operator()(p p_r, p p_d, p p_T, p p_sigma, p p_S0, p p_K) const = 0;
    //! \brief Overload for numerically solving the implied volatility equation.
    //! Warning: Assumes all the other parameters are cached!
    //! \param p_sigma
    //! \return The value of the option.
    virtual double operator()(double p_sigma) const = 0;
    //! \brief The vega: \f$\frac{\partial C}{\partial \sigma}\f$ - used in Newton-Raphson root finding algorithm, for example.
    //! \param p_sigma
    //! \return
    virtual double vega(double p_sigma) const = 0;

protected:
    p m_r{};
    p m_d{};
    p m_T{};
    p m_sigma{};
    p m_S0{};
    p m_K{};
};

//! \brief The Black-Scholes formula for calls.
class BSCall : public BSFormula
{
public:
    using BSFormula::BSFormula;
    using BSFormula::p;
    friend class BSPut;

    virtual double operator()(p p_r, p p_d, p p_T, p p_sigma, p p_S0, p p_K) const override;
    virtual double operator()(double p_sigma) const override;
    virtual double vega(double p_sigma) const override;

    // This would be a fantastic candidate for a constexpr function, but alas the std math functions are not constexpr for C
    // compatibility.
    //! \brief A static version of the formula.
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_T - The expiry time.
    //! \param p_sigma - The volatility.
    //! \param p_S0 - The spot @ time 0.
    //! \param p_K - The strike.
    //! \return The value of the option.
    static double
    BSCallFormula(const double p_r, const double p_d, const double p_T, const double p_sigma, const double p_S0, const double p_K);
};

//! \brief The Black-Scholes formula for puts.
class BSPut : public BSFormula
{
public:
    using BSFormula::BSFormula;
    using BSFormula::p;

    virtual double operator()(p p_r, p p_d, p p_T, p p_sigma, p p_S0, p p_K) const override;
    virtual double operator()(double p_sigma) const override;
    virtual double vega(double p_sigma) const override;

    // This would be a fantastic candidate for a constexpr function, but alas the std math functions are not constexpr for C
    // compatibility.
    //! \brief A static version of the formula.
    //! \param p_r - The interest rate.
    //! \param p_d - The dividend rate.
    //! \param p_T - The expiry time.
    //! \param p_sigma - The volatility.
    //! \param p_S0 - The spot @ time 0.
    //! \param p_K - The strike.
    //! \return The value of the option.
    static double
    BSPutFormula(const double p_r, const double p_d, const double p_T, const double p_sigma, const double p_S0, const double p_K);
};

//!@}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T normalDistImpl(size_t p_seed)
{
    thread_local std::random_device rDev;
    if (p_seed == std::numeric_limits<size_t>::max())
    {
        // The generator is seeded from the implementation-defined device - this should be /dev/random
        p_seed = rDev();
    }
    thread_local std::mt19937_64 rng{p_seed};
    thread_local std::normal_distribution<T> Ndist;

    return Ndist(rng);
}

} // namespace der

#endif // DERIVATIVES_H
