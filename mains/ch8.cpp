/** \file ch8.cpp
 * \author Andrej Leban
 * \date 7/2019
 *
 * Ch. 8: Trees
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
    sigma = 0.2;
    r = 0.02;
    d = 0.01;
    nSteps = 10;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r, dividend rate, and the number of steps\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> d >> nSteps;
#endif

    Parameters rP{ParametersConstant(r)};
    Parameters dP{ParametersConstant(d)};

    // the pricing tree
    Tree tree(nSteps, S0, rP, dP, sigma, T);

    // the payoff functions
    PayoffCall payoff(K);
    PayoffForward payoffF(K);

    // the products
    TreeEuropean europeanOption(T, payoff);
    TreeAmerican americanOption(T, payoff);
    // European exercise rights + forward payoff = forward
    TreeEuropean forward(T, payoffF);

    std::cout << "Price of the Forward is: " << tree.price(forward) << "\n";
    std::cout << "Price of the European Option is: " << tree.price(europeanOption) << "\n";
    std::cout << "Price of the American Option is: " << tree.price(americanOption) << "\n";

    return 0;
}
