/** \file mc2.cpp
 * \author Andrej Leban
 * \date 11/2018
 */

#include <iostream>

#include "derivatives.h"
#include "payoff1.h"

std::random_device rDev {};
// The generator is seeded from the implementation-defined device
std::mt19937_64 rng{ rDev() };

std::normal_distribution<double> Ndist {0, 1};


struct simSpot
{
    simSpot(double p_S0, double p_t, double p_sigma, double p_r) : m_t(p_t), m_sigma(p_sigma),
        m_precalc(p_S0 * std::exp( (p_r - 0.5*p_sigma*p_sigma)*p_t))
    {}

    double operator() () const
    {
        return m_precalc * std::exp( m_sigma * std::sqrt(m_t) * N() );
    }

    static double N ()
    { return Ndist(rng); }

    double m_t; double m_sigma;
    double m_precalc;
};


int main( int /*argc*/, char */*argv*/[] )
{
    double S0, K, T, sigma, r;
    int nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma  = 0.5;
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

    double sum = 0.0;
    const simSpot spot {S0, T, sigma, r};
    const der::payoff1 payoff {K, der::OptionsType::call};

    for( int i = 0; i < nScen; ++i )
    {
        sum += payoff(spot());
    }

    std::cout << "the price is: " << std::exp(-r*T) * (sum / nScen) << "\n";


    return 0;
}
