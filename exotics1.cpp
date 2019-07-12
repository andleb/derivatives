/** \file exotics1.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include "exoticengine.h"
#include "pathdependent.h"
#include "payoff2.h"
#include "random.h"

using namespace der;

int main(int, char * [])
{
    double S0, K, T, sigma, r, d;
    size_t nScen;
    size_t nDates;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    nScen = 10000000;
    nDates = 10;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r, number of scenarios and the number of averaging dates:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen >> nDates;
#endif

    ExoticBSEngine<RandomParkMiller<1>> engine{};
    Payoff2call payoff{K};

    std::vector<double> dates(nDates);
    for (size_t i = 0; i < nDates; ++i)
    {
        dates[i] = (i + 1.0) * T / nDates;
    }
    return 0;
}
