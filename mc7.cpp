/** \file mc7.cpp
 * \author Andrej Leban
 * \date 3/2019
 * Ch 6 A random numbers class
 */

#include <iostream>
#include <sstream>

#include "common/io.h"

#include "derivatives.h"

#include "random.h"
#include "parameters.h"
#include "statistics.h"
#include "vanillaoption2.h"

using namespace der;

// generator is passed by value so there is no state persistence
template<typename Generator, size_t DIM>
auto doMonteCarlo(const VanillaOption2 & option, const Parameters & sigma, const Parameters & r, double S0, int nScen,
                  StatisticsBase & gatherer, RandomBase<Generator, DIM> p_generator)
{
    const simSpotParams spot{S0, option.expiry(), sigma, r};
    auto discount = std::exp(-r.integral(0, option.expiry()));

    for (int i = 0; i < nScen; ++i)
    {
        // payoff in the current path, spot() simulates a spot value @expiry
        auto thisPayoff = option.optionPayoff(spot());
        // add it to the sum
        gatherer.dumpOneResult(discount * thisPayoff);
    }

    return gatherer.resultsSoFar();
}

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
