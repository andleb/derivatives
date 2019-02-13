/** \file mc3.cpp
 * \author Andrej Leban
 * \date 12/2018
 */

#include <iostream>
#include <sstream>

#include "derivatives.h"
#include "payoff2.h"

double doMonteCarlo(const der::Payoff2 & payoff, double T, double sigma, double r, double S0, int nScen)
{
    double sum = 0.0;
    const der::simSpot spot {S0, T, sigma, r};

    for( int i = 0; i < nScen; ++i )
    {
        sum += payoff(spot());
    }

    return std::exp(-r*T) * (sum / nScen);
}

int main( int /*argc*/, char */*argv*/[] )
{
    double S0, K, T, sigma, r;
    int nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma  = 0.5;
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

    std::cout << "the price is: " << doMonteCarlo(der::Payoff2call{K}, T, sigma, r, S0, nScen)  << "\n";

    return 0;
}
