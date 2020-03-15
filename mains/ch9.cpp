/** \file ch9.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * Ch. 9: Solvers, templates, and implied volatilities.
 */

#include <cstddef>
#include <iostream>
#ifdef NDEBUG
#include <sstream>
#endif

#include "../src/derivatives.h"
#include "../src/solver.h"

using namespace der;

int main()
{
    double S0, K, T, sigma, r, d;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    d = 0.;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r, and the dividend rate\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> d;
#endif

    std::cout << "Sanity check: " << BSCall::BSCallFormula(r, d, T, sigma, S0, K) << "\n";
    try
    {
        double impVolB = bisection(88.2363, BSCall(r, d, T, {}, S0, K), 0.3, 0.55);
        std::cout << "Implied volatility by bisection is: " << impVolB << "\n";
    }
    catch (const std::logic_error & e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    try
    {
        double impVolN = newtonRaphson(88.2363, BSCall(r, d, T, {}, S0, K), 0.4);
        std::cout << "Implied volatility by Newton-Raphson is: " << impVolN << "\n";
    }
    catch (const std::logic_error & e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
