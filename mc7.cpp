/** \file mc7.cpp
 * \author Andrej Leban
 * \date 3/2019
 * Ch 6 A random numbers class
 */

#include <iostream>
#include <sstream>

#include "common/io.h"

#include "derivatives.h"

#include "parameters.h"
#include "random.h"
#include "statistics.h"
#include "vanillaoption2.h"

using namespace der;

// generator is passed by value so there is no state persistence
template<typename Generator>
auto doMonteCarlo(const VanillaOption2 & option, const Parameters & sigma, const Parameters & r, double S0, size_t nScen,
                  StatisticsBase & gatherer, Generator & p_generator)
{
    simSpotParamsMultiple<Generator> spot{S0, option.expiry(), sigma, r, p_generator};

    std::vector<double> simSpots = spot.simSpotMultiple(nScen);
    auto discount = std::exp(-r.integral(0, option.expiry()));

    for (auto & spotInst : simSpots)
    {
        // payoff in the current path, spot() simulates a spot value @expiry
        auto thisPayoff = option.optionPayoff(spotInst);
        // add it to the sum
        gatherer.dumpOneResult(discount * thisPayoff);
    }

    return gatherer.resultsSoFar();
}

int main(int /*argc*/, char * /*argv*/ [])
{

    double S0, K, T, sigma, r;
    size_t nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    nScen = 100000;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r and number of scenarios:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen;
    std::cin >> S0 >> K >> T >> sigma >> r >> nScen;
#endif

    VanillaOption2 option{Payoff2call{K}, T};
    StatisticsMean gathererInner{};
    ConvergenceTable gatherer{std::make_unique<StatisticsMean>(std::move(gathererInner))};

//    AntiThetic<RandomParkMiller<1>, 1> generator{1};
    RandomParkMiller<1> generator{1};

//    const simSpotParamsMultiple<AntiThetic<RandomParkMiller<1>, 1> > spot{S0, option.expiry(),
//                                                                         ParametersConstant{sigma}, ParametersConstant{r}, 1};


    std::vector<std::vector<double>> results = doMonteCarlo
        (option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer, generator);

    std::cout << "the price is: " << results.back().front() << "\n";
    std::cout << "number of paths were: " << gatherer.simsSoFar() << "\n";
    std::cout << "the results are: " << results << "\n";

    return 0;
}
