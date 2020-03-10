/** \file mc1.cpp
 * \author Andrej Leban
 * \date 11/2018
 *
 * Chapter 1. A simple Monte Carlo model
 */

#include <cmath>
#include <iostream>

#include "../src/derivatives.h"

double payoff(double p_spot, double p_strike)
{
    return std::max<double>((p_spot - p_strike), 0);
}

double simSpot(double p_S0, double p_t, double p_sigma, double p_r)
{
    return p_S0 * std::exp((p_r - 0.5 * p_sigma * p_sigma) * p_t + p_sigma * std::sqrt(p_t) * der::normalDist<double>());
}

int main(int, char * [])
{

    double S0, K, T, sigma, r;
    int nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    nScen = 10000000;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r and number of scenarios:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen;
    std::cin >> S0 >> K >> T >> sigma >> r >> nScen;
#endif

    std::cout << S0 << " " << K << " " << T << " " << sigma << " " << r << " " << nScen << "\n";

    double sum = 0.0;
    for (int i = 0; i < nScen; ++i)
    {
        sum += payoff(simSpot(S0, T, sigma, r), K);
    }

    // NOTE: this is wildly inaccurate due to the numerical inaccuracy of the simSpot call
    std::cout << "the price is: " << std::exp(-r * T) * (sum / nScen) << "\n";

    return 0;
}
