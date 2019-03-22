/** \file mc7.cpp
 * \author Andrej Leban
 * \date 3/2019
 * Ch 6 A random numbers class
 */

#include <iostream>
#include <sstream>

#include "common/io.h"

#include "derivatives.h"

using namespace der;


int main(int /*argc*/, char * /*argv*/ [])
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

    return 0;
}
