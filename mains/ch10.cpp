/** \file ch10.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * Ch. 10: The Factory pattern.
 */

#include <cstddef>
#include <iostream>
#ifdef NDEBUG
#include <sstream>
#endif

#include "../src/payoff.h"
#include "../src/payofffactory.h"

using namespace der;

int main()
{
    PayoffFactory & factory = PayoffFactory::instance();
    std::string name;
    double strike;

#ifndef NDEBUG
    name = "call";
    strike = 90.;
#else
    std::cout << "enter the product name and strike \n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> name >> strike;
#endif

    auto payoff = factory.createPayoff(name, strike);
    if (payoff != nullptr)
    {
        std::cout << "The payoff of a " << name << " for a spot of 100 is: " << (*payoff)(100) << "\n";
    }

    return 0;
}
