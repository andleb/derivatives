/** \file ch8.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * Ch. 14: Templatizing the factory.
 */

#include <iostream>
#include <sstream>

// common libraries
#include <common/io.h>
#include <common/numeric.h>

#include "../src/parameters.h"
#include "../src/payoff.h"
#include "../src/tree.h"
#include "../src/treeproduct.h"

using namespace der;

int main()
{
    double S0, K, T, sigma, r, d;
    size_t nSteps;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    d = 0.;
    nSteps = 1000;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r, dividend rate, and the number of steps\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> d >> nSteps;

    // for benchmarking
    //  std::cout << "enter the number of steps\n";
    //  std::string inputParams;
    //  std::getline(std::cin, inputParams);
    //  std::istringstream iss{inputParams};
    //  iss >> nSteps;

#endif


//    std::cout << "Price of the American Option is: " << tree.price(americanCall) << "\n";

    return 0;
}
