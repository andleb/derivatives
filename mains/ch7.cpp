/** \file ch7.cpp
 * \author Andrej Leban
 * \date 4/2019
 *
 * Ch. 7: An exotics engine and the template pattern.
 */

#include <iostream>
#include <sstream>

// common libraries
#include "common/io.h"
#include <common/numeric.h>

#include "../src/exoticengine.h"
#include "../src/pathdependent.h"
#include "../src/payoff.h"

using namespace der;

int main()
{
    double S0, K, T, sigma, r, d;
    size_t nScen;
    size_t nDates;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.2;
    r = 0.02;
    d = 0.01;
    nScen = 1E6;
    nDates = 10;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r, number of scenarios, and the number of averaging dates:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen >> nDates;
#endif

    PayoffCall payoff{K};

    // not interested in time 0, which we'll discard
    std::vector<double> dates = cm::linspace(T / nDates, T, nDates, true);

    AsianOptionArith option(dates, T, payoff);

    ParametersConstant sigmaP{sigma};
    ParametersConstant rP{r};
    ParametersConstant dP{d};

    ConvergenceTable gatherer{std::make_unique<StatisticsMean>()};
    ConvergenceTable gathererat{std::make_unique<StatisticsMean>()};
    ConvergenceTable gathereratm{std::make_unique<StatisticsMean>()};

    MersenneTwister<1> generator{};
    AntiThetic<MersenneTwister<1>, 1> generatorat {};
    AntiTheticMemo<MersenneTwister<1>, 1> generatoratm {};

    ExoticBSEngine<decltype(generator)> engine(option, rP, dP, sigmaP, S0);
    ExoticBSEngine<decltype(generatorat)> engineat(option, rP, dP, sigmaP, S0);
    ExoticBSEngine<decltype(generatoratm)> engineatm(option, rP, dP, sigmaP, S0);

    engine.doSimulation(gatherer, nScen);
    engineat.doSimulation(gathererat, nScen);
    engineatm.doSimulation(gathereratm, nScen);

    auto results = gatherer.resultsSoFar();
    auto resultsat = gathererat.resultsSoFar();
    auto resultsatm = gathereratm.resultsSoFar();

    std::cout << "Arithmetic Asian call pricing with number of paths: " << gatherer.simsSoFar() << "\n";
    std::cout << "the results are: " << results << "\n\n";
    std::cout << "Anti-thetic results are: " << resultsat << "\n\n";
    std::cout << "Anti-thetic w memoization results are: " << resultsatm << "\n\n";

    return 0;
}
