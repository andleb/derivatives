/** \file ch6.cpp
 * \author Andrej Leban
 * \date 3/2019
 *
 * Ch. 6: A random numbers class.
 */

#include <iostream>
#include <sstream>

#ifdef DUMPRESULTS
#include <filesystem>
#include <fstream>
#endif

#include "common/io.h"

#include "../src/random.h"
#include "../src/simspot.h"
#include "../src/statistics.h"
#include "../src/vanillaoption.h"

using namespace der;

// generator is passed by value so there is no state persistence
// nullptr_t is used as a compile-time "None" marker
template <typename Generator = std::nullptr_t>
auto doMonteCarlo(const VanillaOption & option, const Parameters & sigma, const Parameters & r, double S0, size_t nScen,
                  StatisticsBase & gatherer, Generator p_generator = nullptr)
{
    // declare, but don't initialize
    simSpotParamsMultiple<Generator> spot;

    if constexpr (!std::is_same<Generator, std::nullptr_t>::value)
    {
        spot = simSpotParamsMultiple<Generator>{S0, option.expiry(), sigma, r, p_generator};
    }
    else
    {
        spot = simSpotParamsMultiple<Generator>{S0, option.expiry(), sigma, r};
    }

    // number of scenarios is taken into account here
    std::vector<double> simSpots = spot.simSpotMultiple(nScen);
    double discount = std::exp(-r.integral(0, option.expiry()));

    for (auto & spotInst : simSpots)
    {
        // payoff in the current path
        double thisPayoff = option.optionPayoff(spotInst);
        // add it to the sum
        gatherer.dumpOneResult(discount * thisPayoff);
    }

    return gatherer.resultsSoFar();
}

#ifdef DUMPRESULTS
void dumpResults(std::vector<std::vector<double>> p_results, std::string p_name)
{
    std::filesystem::path dataDir{"../data"};
    if (!std::filesystem::exists(dataDir))
    {
        std::filesystem::create_directory(dataDir);
    }

    std::ofstream f{"../data/" + p_name};
    for (auto it = p_results.begin(); it != p_results.end() - 1; ++it)
    {
        f << (*it)[0] << " " << (*it)[1] << "\n";
    }

    f << (*(p_results.end() - 1))[0] << " " << (*(p_results.end() - 1))[1];
}
#endif

int main(int, char * [])
{

    double S0, K, T, sigma, r;
    size_t nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    nScen = 1e7;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r and number of scenarios:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen;
    std::cin >> S0 >> K >> T >> sigma >> r >> nScen;
#endif

    VanillaOption option{PayoffCall{K}, T};
    StatisticsMean gathererInner{};

    // Park-Miller generator
    ConvergenceTable gatherer1{std::make_unique<StatisticsMean>(gathererInner)};
    RandomParkMiller<1> generator1{1};

    std::vector<std::vector<double>> results1 =
        doMonteCarlo(option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer1, generator1);

    std::cout << "Park-Miller: number of paths were: " << gatherer1.simsSoFar() << "\n";
    std::cout << "the results are: " << results1 << "\n\n";

    // Anti-Thetic generator
    ConvergenceTable gatherer2{std::make_unique<StatisticsMean>(gathererInner)};
    AntiThetic<RandomParkMiller<1>, 1> generator2{42};

    std::vector<std::vector<double>> results2 =
        doMonteCarlo(option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer2, generator2);

    std::cout << "Anti-thetic Park-Miller: number of paths were: " << gatherer2.simsSoFar() << "\n";
    std::cout << "the results are: " << results2 << "\n\n";

    // Built-in Mersenne twister 64bit
    // Default generator for the random class, i.e. when no generator is provided
    ConvergenceTable gatherer3{std::make_unique<StatisticsMean>(gathererInner)};

    std::vector<std::vector<double>> results3 =
        doMonteCarlo<std::nullptr_t>(option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer3);

    std::cout << "Mersenne twister: number of paths were: " << gatherer3.simsSoFar() << "\n";
    std::cout << "the results are: " << results3 << "\n\n";

    // Anti-Thetic Mersenne-twister
    ConvergenceTable gatherer4{std::make_unique<StatisticsMean>(gathererInner)};
    AntiThetic<MersenneTwister<1>, 1> generator4{};
    std::vector<std::vector<double>> results4 =
        doMonteCarlo(option, ParametersConstant{sigma}, ParametersConstant{r}, S0, nScen, gatherer4, generator4);

    std::cout << "Anti-thetic Mersenne twister: number of paths were: " << gatherer4.simsSoFar() << "\n";
    std::cout << "the results are: " << results4 << "\n";

#ifdef DUMPRESULTS
    dumpResults(results1, "PM");
    dumpResults(results2, "PM-AT");
    dumpResults(results3, "MT");
    dumpResults(results4, "MT-AT");
#endif

    return 0;
}
