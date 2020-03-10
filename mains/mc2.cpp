/** \file mc2.cpp
 * \author Andrej Leban
 * \date 11/2018
 */

#include <iostream>
#include <sstream>

#include "../src/derivatives.h"
#include "../src/payoff1.h"
#include "../src/simspot.h"


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

    double sum = 0.0;
    const der::simSpot spot {S0, T, sigma, r};
    const der::payoff1 payoff {K, der::OptionsType::call};

    for( int i = 0; i < nScen; ++i )
    {
        sum += payoff(spot());
    }

    std::cout << "the price is: " << std::exp(-r*T) * (sum / nScen) << "\n";


    return 0;
}
