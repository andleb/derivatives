/** \file final.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * Final project: Multithreaded trinomial tree pricing.
 */

#include <iostream>
#include <sstream>
#include <stddef.h>

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

    Parameters rP{ParametersConstant(r)};
    Parameters dP{ParametersConstant(d)};

    // the pricing tree
    trinomialTree tree(nSteps, 0.5, S0, rP, dP, sigma, T);

    // the payoff functions
    PayoffCall payoffC(K);
    PayoffPut payoffP(K);
    PayoffForward payoffF(K);

    // the products
    TreeEuropean europeanCall(T, payoffC);
    TreeEuropean europeanPut(T, payoffP);
    TreeAmerican americanCall(T, payoffC);
    // European exercise rights + forward payoff = forward
    TreeEuropean forward(T, payoffF);

    double pF = tree.price(forward);
    double pEC = tree.price(europeanCall);
    double pEP = tree.price(europeanPut);

    std::cout << "Pricing with " << nSteps << " time steps.\n";
    std::cout << "Price of the Forward is: " << pF << "\n";
    std::cout << "Price of the European Call Option is: " << pEC << "\n";
    std::cout << "Price of the European Put Option is: " << pEP << "\n";
    std::cout << "Put-call parity preserved: " << std::boolalpha << (std::abs((pEC - pEP) - pF) < 1e-3)
              << " (diff = " << std::abs((pEC - pEP) - pF) << ")"
              << "\n";
    std::cout << "Price of the American Option is: " << tree.price(americanCall) << "\n";

    return 0;
}
