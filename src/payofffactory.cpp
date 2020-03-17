/** \file payofffactory.cpp
 * \author Andrej Leban
 * \date 3/2020
 */

#include <iostream>
#include <memory>

#include "payoff.h"
#include "payofffactory.h"

namespace der
{

PayoffFactory & PayoffFactory::instance()
{
    static PayoffFactory thisInstance{};
    return thisInstance;
}

void PayoffFactory::registerPayoff(std::string p_name, PayoffFactory::PayoffFactoryMethod p_factory)
{
    m_registeredFactories[p_name] = p_factory;
}

std::unique_ptr<Payoff> PayoffFactory::createPayoff(std::string p_name, double p_strike)
{
    try
    {
        return m_registeredFactories.at(p_name)(p_strike);
    }
    catch (const std::out_of_range &)
    {
        std::cerr << "PayoffFactory::createPayoff: No function registered as " << p_name << "\n";
        return nullptr;
    }
}

} // namespace der
