/** \file random.h
 *  \author Andrej Leban
 *  \date 3/2019
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#ifdef TESTING
    #include <fstream>
#endif
#include <stdexcept>
#include <vector>

// Decided on static polymorphism here instead of dynamic,
// since we know the desired generator at compile time

// interface functions are not const since they mutate some generator states
// TODO:    * perhaps use mutable for the state fields?

namespace der
{

//! \brief The RandomBase class
//! The interface class for random generators
template <typename Derived, size_t DIM>
class RandomBase
{
public:
    RandomBase() = default;

    //! \brief uniforms
    //! \param p_variates should be pre-allocated to the desired size
    std::vector<double> uniforms(std::vector<double> && p_variates);
    //! \brief gaussians
    //! \param p_variates should be pre-allocated to the desired size
    std::vector<double> gaussians(std::vector<double> && p_variates);

    static void invCDFNormal(double & x);

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed);
    void reset();
};

template <typename Generator, size_t DIM>
class AntiThetic : public RandomBase<AntiThetic<Generator, DIM>, DIM>
{
public:
    AntiThetic() = default;
    explicit AntiThetic(long p_seed)
        : m_generator(p_seed)
    {}

    std::vector<double> uniforms(std::vector<double> && p_variates);

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed);
    void reset();

private:
    Generator m_generator;
};

//! \brief The RandomParkMiller class
//! The minimal standard linear congruential generator due to Park & Miller
template <size_t DIM>
class RandomParkMiller : public RandomBase<RandomParkMiller<DIM>, DIM>
{
public:
    RandomParkMiller(long p_seed = 1);

    std::vector<double> uniforms(std::vector<double> && p_variates);

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed) { m_seed = p_seed != 0 ? p_seed : 1; }
    void reset();

    long max() const { return m_coeffs.m - 1; }
    long min() const { return 1; }
    long randInt();

private:
    long m_initSeed;
    mutable long m_seed;
    double m_reciprocal;

    //! The coefficients for the Park-Miller LCG
    static constexpr struct Coeff
    {
        static constexpr long a = 16807;
        static constexpr long m = 2147483647;
        static constexpr long q = 127773;
        static constexpr long r = 2836;
    } m_coeffs;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// RandomBase

template <typename Derived, size_t DIM>
std::vector<double> RandomBase<Derived, DIM>::uniforms(std::vector<double> && p_variates)
{
    //the implementation provides the uniforms
    return static_cast<Derived *>(this)->uniforms(std::move(p_variates));
}

template <typename Derived, size_t DIM>
std::vector<double> RandomBase<Derived, DIM>::gaussians(std::vector<double> && p_variates)
{
    //NOTE: here we provide a default implementation, can of course still be overloaded in the derived class
    auto ret = uniforms(std::move(p_variates));
    std::for_each(ret.begin(), ret.end(), invCDFNormal);
#ifdef TESTING
    std::ofstream f {"../gaussians"};
    for(auto it = ret.begin(); it != ret.end() - 1; ++it)
    {
        f << *it << ", ";
    }

    f << *(ret.end() - 1);
#endif

    return ret;
}

template <typename Derived, size_t DIM>
void RandomBase<Derived, DIM>::invCDFNormal(double & x)
{
    // adapted from M. Joshi's source

    static constexpr double a[4] = {2.50662823884,
                          -18.61500062529,
                          41.39119773534,
                          -25.44106049637};

    static constexpr double b[4] = {-8.47351093090,
                          23.08336743743,
                          -21.06224101826,
                          3.13082909833};

    static constexpr double c[9] = {0.3374754822726147,
                          0.9761690190917186,
                          0.1607979714918209,
                          0.0276438810333863,
                          0.0038405729373609,
                          0.0003951896511919,
                          0.0000321767881768,
                          0.0000002888167364,
                          0.0000003960315187};

    double u = x - 0.5;
    double r;

    if (std::abs(u) < 0.42) // Beasley-Springer
    {
        double y = u * u;

        r = u * (((a[3] * y + a[2]) * y + a[1]) * y + a[0]) / ((((b[3] * y + b[2]) * y + b[1]) * y + b[0]) * y + 1.0);
    }
    else // Moro
    {

        r = x;

        if (u > 0.0)
        {
            r = 1.0 - x;
        }

        r = std::log(-std::log(r));

        r = c[0] + r * (c[1] + r * (c[2] + r * (c[3] + r * (c[4] + r * (c[5] + r * (c[6] + r * (c[7] + r * c[8])))))));

        if (u < 0.0)
        {
            r = -r;
        }
    }

    x = r;
}

template <typename Derived, size_t DIM>
void RandomBase<Derived, DIM>::skip(size_t p_nPaths)
{
    return static_cast<Derived *>(this)->skip(p_nPaths);
}

template <typename Derived, size_t DIM>
void RandomBase<Derived, DIM>::setSeed(size_t p_seed)
{
    return static_cast<Derived *>(this)->setSeed(p_seed);
}

template <typename Derived, size_t DIM>
void RandomBase<Derived, DIM>::reset()
{
    return static_cast<Derived *>(this)->reset();
}

//Anti-Thetic

//TODO: do this with less copies while keeping value semantics
template <typename Generator, size_t DIM>
std::vector<double> AntiThetic<Generator, DIM>::uniforms(std::vector<double> && p_variates)
{
    auto ret = p_variates;
    long half = ret.size() / 2;
    auto midpoint = ret.begin() + half;

    auto tmp = m_generator.uniforms(std::vector<double>{ret.begin(), midpoint});

    std::move(tmp.begin(), tmp.end(), ret.begin());

    // anti-thetic in the second half
    for (auto it = midpoint + 1; it != ret.end(); ++it)
    {
        *it = 1.0 - *(it - half);
    }

    return ret;
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::skip(size_t p_nPaths)
{
    return m_generator.skip(p_nPaths / 2);
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::setSeed(size_t p_seed)
{
    return m_generator.setSeed(p_seed);
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::reset()
{
    return m_generator.reset();
}

// RandomParkMiller

//template<size_t DIM>
//typename RandomParkMiller<DIM>::Coeff const RandomParkMiller<DIM>::m_coeffs;

template <size_t DIM>
RandomParkMiller<DIM>::RandomParkMiller(long p_seed)
    : m_initSeed(p_seed)
    , m_seed(p_seed)
{
    if (p_seed == 0)
    {
        m_seed = 1;

    }

    // + 1 so we obtain the uniforms on the closed interval (0,  1)
    m_reciprocal = 1. / (1. + max());
}

template <size_t DIM>
std::vector<double> RandomParkMiller<DIM>::uniforms(std::vector<double> && p_variates)
{
    //TODO: move here?
    auto ret = p_variates;
    std::for_each(ret.begin(), ret.end(), [this](auto & el) { el = randInt() * m_reciprocal; });
#ifdef TESTING
    std::ofstream f {"../uniforms"};
    for(auto it = ret.begin(); it != ret.end() - 1; ++it)
    {
        f << *it << ", ";
    }

    f << *(ret.end() - 1);
#endif
    return ret;
}

template <size_t DIM>
void RandomParkMiller<DIM>::skip(size_t p_nPaths)
{
    for (size_t i; i < p_nPaths; ++i)
    {
        randInt();
    }
}

template <size_t DIM>
void RandomParkMiller<DIM>::reset()
{
    m_seed = m_initSeed != 0 ? m_initSeed : 1;
}

template <size_t DIM>
long RandomParkMiller<DIM>::randInt()
{
    long k = m_seed / m_coeffs.q;

    m_seed = m_coeffs.a * (m_seed - k * m_coeffs.q) - k * m_coeffs.r;

    if (m_seed < 0)
    {
        m_seed += m_coeffs.m;
    }

    // TODO: remove this after debug:
    if (m_seed > max())
    {
        throw std::runtime_error{"random generator out of bounds"};
    }

    return m_seed;

// from the article
/*
//    long hi = m_seed / m_coeffs.q;
//    long lo = m_seed % m_coeffs.q;

//    long test = m_coeffs.a * lo - m_coeffs.r * hi;

//    if (test > 0)
//    {
//        m_seed = test;
//    }
//    else
//    {
//        m_seed = test + m_coeffs.m;
//    }

//    return m_seed / m_coeffs.m;

*/
}

} // namespace der

#endif // RANDOM_H
