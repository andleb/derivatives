/** \file ch5.cpp
 * \author Andrej Leban
 * \date 2/2019
 *
 * Ch. 5: Strategies, decoration, and statistics.
 */

#include <iostream>
#include <sstream>

#include "common/io.h"

#include "../src/simspot.h"
#include "../src/statistics.h"
#include "../src/vanillaoption.h"

using namespace der;

auto doMonteCarlo(const VanillaOption & option, const Parameters & sigma, const Parameters & r, double S0, int nScen,
                  StatisticsBase & gatherer)
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

    VanillaOption option{PayoffCall{K}, T};

    StatisticsMean gathererInner{};
    ConvergenceTable gatherer{std::make_unique<StatisticsMean>(std::move(gathererInner))};

    auto results = doMonteCarlo(option, ParametersConstant{sigma}, ParametersConstant{r},
                                S0, nScen, gatherer);

    std::cout << "the price is: " << results.back().back() << "\n";
    std::cout << "number of paths were: " << gatherer.simsSoFar() << "\n";
    std::cout << "the results are: " << results << "\n";

    return 0;
}
