/** \file ch14.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * Ch. 14: Templatizing the factory.
 */

#include <iostream>
#include <sstream>

// common libraries
#include <common/io.h>

#include "../src/templatefactory.h"
#include "../src/payoff.h"

using namespace der;

int main()
{
    Factory<Payoff> & factory = Factory<Payoff>::instance();
    factoryRegisterer<PayoffCall, Payoff, double> call{"call"};
    factoryRegisterer<PayoffDoubleDigital, Payoff, double, double> dd{"dd"};

    std::string name1, name2;
    double strike1, strike2, strike3;

#ifndef NDEBUG
    name1 = "call";
    strike1 = 90.;

    name2 = "dd";
    strike2 = 90.;
    strike3 = 110.;

#else
    std::cout << "enter the product name and strike \n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> name >> strike;
#endif

    // arbitrary number of arguments
    auto payoffcall = factory.create(name1, strike1);
    if (payoffcall != nullptr)
    {
        std::cout << "The payoff of a " << name1 << " for a spot of 100 is: " << (*payoffcall)(100) << "\n";
    }
    auto payoffdd = factory.create(name2, strike2, strike3);
    if (payoffdd != nullptr)
    {
        std::cout << "The payoff of a " << name2 << " for a spot of 100 is: " << (*payoffdd)(100) << "\n";
    }

    return 0;
}
