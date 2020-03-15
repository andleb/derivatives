/** \file solver.h
 * \author Andrej Leban
 * \date 3/2020
 *
 * Custom root-finding algorithms.
 */

#ifndef DER_SOLVER_H
#define DER_SOLVER_H

#include <cmath>
#include <stdexcept>

//NOTE: Remove this coupling if type checking is not desired for Newton's method.
#include <type_traits>
#include "derivatives.h"

namespace der
{

class Solver
{
public:
    Solver() = default;
};


//! \brief Solves \p p_function( \p return) = \p p_y via bisection.
//! \param p_y
//! \param p_function
//! \param p_a - lower guess.
//! \param p_b - upper guess.
//! \param p_tol - desired tolerance
//! \return The solution.
template <typename Function>
double bisection(double p_y, Function p_function, double p_a, double p_b, double p_tol = 1e-5) noexcept(false)
{
    int increasing = (p_function(p_b) >= p_function(p_a)) ? 1 : -1;
    size_t iters = 0;

    double mid, yf, diff;
    bool flag = true;

    do
    {
        mid = (p_a + p_b) / 2.;
        yf = p_function(mid);
        diff = (p_y - yf) * increasing;
        flag = std::abs(diff) > p_tol;

        if (diff > p_tol)
        {
            p_a = mid;
        }
        else if (diff < -p_tol)
        {
            p_b = mid;
        }
        else
        {
            break;
        }

        ++iters;
        if (iters > 1e6)
        {
            throw std::logic_error("Cannot converge, function malformed or doesn't cross the value on interval?");
        }
    } while (flag);

    return mid;
}

//! \brief Solves \p p_functor( \p return) = \p p_y via Newton-Raphson's method.
//! \p p_functor must be derived from the BSFormula class.
//! \param p_y
//! \param p_functor
//! \param p_x0 - initial guess.
//! \param p_tol - desired tolerance.
//! \return The solution.
// SFINAE
template <typename Functor,
    typename std::enable_if<std::is_base_of<BSFormula, Functor>{}, int>::type = 0>
double newtonRaphson(double p_y, const Functor & p_functor, double p_x0, double p_tol = 1e-5) noexcept(false)
{
    double diff = p_y - p_functor(p_x0);
    size_t iters = 0;
    do
    {
        diff /= p_functor.vega(p_x0);
        p_x0 += diff;

        ++iters;
        if (iters > 1e6)
        {
            throw std::logic_error("Cannot converge, function malformed or doesn't cross the value on interval?");
        }

        diff = p_y - p_functor(p_x0);
    } while (std::abs(diff) > p_tol);

    return p_x0;
}

} // namespace der

#endif // DER_SOLVER_H
