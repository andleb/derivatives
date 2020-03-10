/** \file mc7.cpp
 * \author Andrej Leban
 * \date 3/2019
 * Ch 6 A random numbers class
 */

#include <iostream>
#include <sstream>

#include "common/io.h"

#include "../src/derivatives.h"

#include "../src/random.h"
#include "../src/simspot.h"
#include "../src/statistics.h"
#include "../src/vanillaoption2.h"

using namespace der;

// generator is passed by value so there is no state persistence
template<typename Generator>
auto doMonteCarlo(const VanillaOption2 & option, const Parameters & sigma, const Parameters & r, double S0, size_t nScen,
                  StatisticsBase & gatherer, std::unique_ptr<Generator> p_pGenerator = nullptr)
{
    simSpotParamsMultiple<Generator> spot;
    if constexpr(! std::is_same<Generator, std::nullptr_t>::value)
    {
        spot = simSpotParamsMultiple<Generator> {S0, option.expiry(), sigma, r, *p_pGenerator};
    }
    else
    {
        spot = simSpotParamsMultiple<Generator> {S0, option.expiry(), sigma, r};
    }

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
    nScen = 10000000;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r and number of scenarios:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen;
    std::cin >> S0 >> K >> T >> sigma >> r >> nScen;
#endif

    VanillaOption2 option{PayoffCall{K}, T};
    StatisticsMean gathererInner{};

    ConvergenceTable gatherer1{std::make_unique<StatisticsMean>(gathererInner)};
    RandomParkMiller<1> generator1{1};

    ConvergenceTable gatherer2{std::make_unique<StatisticsMean>(gathererInner)};
    AntiThetic<RandomParkMiller<1>, 1> generator2{1};

    ConvergenceTable gatherer3{std::make_unique<StatisticsMean>(gathererInner)};

//    const simSpotParamsMultiple<AntiThetic<RandomParkMiller<1>, 1> > spot{S0, option.expiry(),
//                                                                         ParametersConstant{sigma}, ParametersConstant{r}, 1};

    std::vector<std::vector<double>> results1 = doMonteCarlo
        (option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer1,
         std::make_unique<decltype(generator1)>(generator1));

    std::cout << "Park-Miller: number of paths were: " << gatherer1.simsSoFar() << "\n";
    std::cout << "the results are: " << results1 << "\n\n";

    std::vector<std::vector<double>> results2 = doMonteCarlo
        (option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer2,
         std::make_unique<decltype(generator2)>(generator2));
    std::cout << "Anti-thetic Park-Miller: number of paths were: " << gatherer2.simsSoFar() << "\n";
    std::cout << "the results are: " << results2 << "\n\n";

    std::vector<std::vector<double>> results3 = doMonteCarlo<std::nullptr_t>
        (option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer3);
    std::cout << "Mersenne twister: number of paths were: " << gatherer3.simsSoFar() << "\n";
    std::cout << "the results are: " << results3 << "\n";

    return 0;
}
