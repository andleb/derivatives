/** \file random.h
 *  \author Andrej Leban
 *  \date 3/2019
 *
 *
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <vector>

#ifdef DUMPINTERMEDIATE
#include <filesystem>
#include <fstream>
#endif

#include "derivatives.h"

// Decided on static polymorphism here instead of dynamic,
// since we usually know the desired generator at compile time

namespace der
{

//! \brief The interface class for random generators.
//! Never used on its own.
//! \p DIM is left aside for possible future implementation of multivariate generators.
template <typename Derived, size_t DIM>
class RandomBase
{
public:
    RandomBase() = default;

    //! @name Distribution functions
    ///@{

    //! \brief Uniform numbers.
    //! \param p_variates should be pre-allocated to the desired size.
    std::vector<double> uniforms(std::vector<double> && p_variates) const;
    //! \brief Gaussian numbers.
    //! \param p_variates should be pre-allocated to the desired size.
    std::vector<double> gaussians(std::vector<double> && p_variates) const;
    ///@}

    //! @name Additional interface
    ///@{

    //! \brief Skip (waste) next \p p_nPaths values.
    void skip(size_t p_nPaths);
    //! \brief Set the seed.
    void setSeed(size_t p_seed);
    //! \brief Reset the generator state.
    void reset();
    ///@}
};

//! \brief The minimal standard linear congruent generator due to Park & Miller.
template <size_t DIM>
class RandomParkMiller : public RandomBase<RandomParkMiller<DIM>, DIM>
{
public:
    RandomParkMiller(long p_seed = 1);

    std::vector<double> uniforms(std::vector<double> && p_variates) const;

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed);
    void reset();

    //! @name Additional interface
    ///@{

    //! \brief The maximum possible number.
    static constexpr long max();
    //! \brief The minimum possible number.
    static constexpr long min();
    //! \brief Generates a single uniformly distributed random number.
    long randInt() const;
    ///@}

protected:
    //! \brief The coefficients for the Park-Miller LCG.
    static constexpr struct Coeff
    {
        static constexpr long a = 16807;
        static constexpr long m = 2147483647;
        static constexpr long q = 127773;
        static constexpr long r = 2836;
    } m_coeffs{}; // GCC bug requires static constexpr's to be initialized, Clang doesn't

private:
    long m_initSeed;
    double m_reciprocal;

    mutable long m_seed;
};

//! \brief Wrapper around the std's Mersenne twister RNG.
template <size_t DIM>
class MersenneTwister : public RandomBase<MersenneTwister<DIM>, DIM>
{
public:
    MersenneTwister(long p_seed = -1);

    std::vector<double> uniforms(std::vector<double> && p_variates) const;
    std::vector<double> gaussians(std::vector<double> && p_variates) const;

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed);
    void reset();

    //! @name Additional interface
    ///@{

    //! \brief The maximum possible number.
    constexpr auto max() const;
    //! \brief The minimum possible number.
    constexpr auto min() const;
    //! \brief Generates a single uniformly distributed random number.
    long randInt() const;
    ///@}

private:
    // no const on these since generation mutates state
    // can be static since it's just an interface to /dev/random by default
    static std::random_device m_rDev;
    // not static since we want different instances, e.g. with different seeds
    mutable std::mt19937_64 m_rng;
    static std::normal_distribution<double> m_normalDist;
    static std::uniform_int_distribution<size_t> m_uniformDist;

    double m_reciprocal = 1. / (1. + max());
};

//! \brief Implements Anti-Thetic sampling using on top of \p Generator.
template <typename Generator, size_t DIM>
class AntiThetic : public RandomBase<AntiThetic<Generator, DIM>, DIM>
{
public:
    AntiThetic() = default;
    explicit AntiThetic(long p_seed);

    std::vector<double> uniforms(std::vector<double> && p_variates) const;

    void skip(size_t p_nPaths);
    void setSeed(size_t p_seed);
    void reset();

protected:
    Generator m_generator;

    mutable std::vector<double> m_last;
    mutable bool useAT{false};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// RandomBase

template <typename Derived, size_t DIM>
std::vector<double> RandomBase<Derived, DIM>::uniforms(std::vector<double> && p_variates) const
{
    // the implementation provides the uniforms
    return static_cast<const Derived *>(this)->uniforms(std::move(p_variates));
}

template <typename Derived, size_t DIM>
std::vector<double> RandomBase<Derived, DIM>::gaussians(std::vector<double> && p_variates) const
{
    // NOTE: here we provide a default implementation, can of course still be overloaded in the derived class
    auto && ret = uniforms(std::move(p_variates));
    std::for_each(ret.begin(), ret.end(), [](auto & el) { el = inverseCumulativeGaussian(el); });

#ifdef DUMPINTERMEDIATE
    std::filesystem::path dataDir{"../data"};
    if (!std::filesystem::exists(dataDir))
    {
        std::filesystem::create_directory(dataDir);
    }
    std::ofstream f{"../data/gaussians"};
    for (auto it = ret.begin(); it != ret.end() - 1; ++it)
    {
        f << *it << ", ";
    }

    f << *(ret.end() - 1);
#endif

    return std::move(ret);
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


// RandomParkMiller

template <size_t DIM>
RandomParkMiller<DIM>::RandomParkMiller(long p_seed) : m_initSeed(p_seed), m_seed(p_seed)
{
    if (p_seed == 0)
    {
        m_seed = 1;
    }

    // + 1 so we obtain the uniforms on the closed interval (0,  1)
    m_reciprocal = 1. / (1. + max());
}

template <size_t DIM>
std::vector<double> RandomParkMiller<DIM>::uniforms(std::vector<double> && p_variates) const
{
    std::for_each(p_variates.begin(), p_variates.end(), [this](auto & el) { el = randInt() * m_reciprocal; });

#ifdef DUMPINTERMEDIATE
    std::filesystem::path dataDir{"../data"};
    if (!std::filesystem::exists(dataDir))
    {
        std::filesystem::create_directory(dataDir);
    }
    std::ofstream f{"../data/uniforms"};
    for (auto it = p_variates.begin(); it != p_variates.end() - 1; ++it)
    {
        f << *it << ", ";
    }

    f << *(p_variates.end() - 1);
#endif

    return std::move(p_variates);
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
void RandomParkMiller<DIM>::setSeed(size_t p_seed)
{
    m_seed = p_seed != 0 ? p_seed : 1;
}

template <size_t DIM>
void RandomParkMiller<DIM>::reset()
{
    m_seed = m_initSeed != 0 ? m_initSeed : 1;
}

template <size_t DIM>
constexpr long RandomParkMiller<DIM>::max()
{
    return m_coeffs.m - 1;
}

template <size_t DIM>
constexpr long RandomParkMiller<DIM>::min()
{
    return 1;
}

template <size_t DIM>
long RandomParkMiller<DIM>::randInt() const
{
    long k = m_seed / m_coeffs.q;

    m_seed = m_coeffs.a * (m_seed - k * m_coeffs.q) - k * m_coeffs.r;

    if (m_seed < 0)
    {
        m_seed += m_coeffs.m;
    }

    return m_seed;
}

// Mersenne Twister

// static inits
template <size_t DIM>
std::random_device MersenneTwister<DIM>::m_rDev;

template <size_t DIM>
std::uniform_int_distribution<size_t> MersenneTwister<DIM>::m_uniformDist;
template <size_t DIM>
std::normal_distribution<double> MersenneTwister<DIM>::m_normalDist;

template <size_t DIM>
MersenneTwister<DIM>::MersenneTwister(long p_seed) : m_rng((p_seed == -1) ? m_rDev() : p_seed)

{}

template <size_t DIM>
std::vector<double> MersenneTwister<DIM>::uniforms(std::vector<double> && p_variates) const
{
    std::for_each(p_variates.begin(), p_variates.end(), [this](auto & el) { el = m_uniformDist(m_rng) * m_reciprocal; });
    return std::move(p_variates);
}

template <size_t DIM>
std::vector<double> MersenneTwister<DIM>::gaussians(std::vector<double> && p_variates) const
{
    std::for_each(p_variates.begin(), p_variates.end(), [this](auto & el) { el = m_normalDist(m_rng); });
    return std::move(p_variates);
}

template <size_t DIM>
void MersenneTwister<DIM>::skip(size_t p_nPaths)
{
    m_rng.discard(p_nPaths);
}

template <size_t DIM>
void MersenneTwister<DIM>::setSeed(size_t p_seed)
{
    m_rng.seed(p_seed);
}

template <size_t DIM>
void MersenneTwister<DIM>::reset()
{
    m_rng.seed();
}

template <size_t DIM>
constexpr auto MersenneTwister<DIM>::max() const
{
    return m_rng.max();
}

template <size_t DIM>
constexpr auto MersenneTwister<DIM>::min() const
{
    m_rng.min();
}

template <size_t DIM>
long MersenneTwister<DIM>::randInt() const
{
    return m_uniformDist(m_rng);
}


// AntiThetic

template <typename Generator, size_t DIM>
AntiThetic<Generator, DIM>::AntiThetic(long p_seed) : m_generator(p_seed)
{}

template <typename Generator, size_t DIM>
std::vector<double> AntiThetic<Generator, DIM>::uniforms(std::vector<double> && p_variates) const
{
    // in case cache can be used
    if (useAT && m_last.size() <= p_variates.size())
    {
        useAT = false;

        // this resizes m_last to 0, cheaper to copy than to resize @ every step
        // return std::move(m_last);
        decltype(m_last) ret(p_variates.size());
        std::copy(m_last.begin(), m_last.begin() + p_variates.size(), ret.begin());
        return ret;
    }

    useAT = true;
    p_variates = AntiThetic<Generator, DIM>::m_generator.uniforms(std::move(p_variates));

    if (m_last.size() != p_variates.size())
    {
        m_last.resize(p_variates.size());
    }

    std::transform(p_variates.begin(), p_variates.end(), m_last.begin(), [](const auto & el) { return 1.0 - el; });

    return std::move(p_variates);
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::skip(size_t p_nPaths)
{
    m_generator.skip(p_nPaths / 2);
}


template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::setSeed(size_t p_seed)
{
    return m_generator.setSeed(p_seed);
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::reset()
{
    m_generator.reset();
    m_last.clear();
    useAT = false;
}

} // namespace der

#endif // RANDOM_H
