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

// TODO:    * Should I use classes?

namespace der
{

class Solver
{
public:
    Solver() = default;
};


template <typename Function>
double bisection(double p_y, Function p_function, double p_a, double p_b, double p_tol = 1e-5)
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

} // namespace der

#endif // DER_SOLVER_H
