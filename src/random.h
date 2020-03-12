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

protected:
    //! \brief Transforms a uniformly distributed \p x into a normally distributed one in-place.
    static void invCDFNormal(double & x);
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

private:
    Generator m_generator;
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
    std::for_each(ret.begin(), ret.end(), invCDFNormal);

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

template <typename Derived, size_t DIM>
void RandomBase<Derived, DIM>::invCDFNormal(double & x)
{
    // adapted from M. Joshi's source

    static constexpr double a[4] = {2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};

    static constexpr double b[4] = {-8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833};

    static constexpr double c[9] = {0.3374754822726147, 0.9761690190917186, 0.1607979714918209,
                                    0.0276438810333863, 0.0038405729373609, 0.0003951896511919,
                                    0.0000321767881768, 0.0000002888167364, 0.0000003960315187};

    double u = x - 0.5;
    double r;

    if (std::abs(u) < 0.42) // Beasley-Springer approximation
    {
        double y = u * u;

        r = u * (((a[3] * y + a[2]) * y + a[1]) * y + a[0]) / ((((b[3] * y + b[2]) * y + b[1]) * y + b[0]) * y + 1.0);
    }
    else // Moro approximation
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

    // TODO: remove this after debug:
    if (m_seed > max())
    {
        throw std::runtime_error{"random generator out of bounds"};
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

// Anti-Thetic

template <typename Generator, size_t DIM>
AntiThetic<Generator, DIM>::AntiThetic(long p_seed) : m_generator(p_seed)
{}

template <typename Generator, size_t DIM>
std::vector<double> AntiThetic<Generator, DIM>::uniforms(std::vector<double> && p_variates) const
{
    long half = p_variates.size() / 2;
    auto midpoint = p_variates.begin() + half;

    // recycling p_variates to minimize copying but keep value semantics
    auto tmp = m_generator.uniforms(std::vector<double>{p_variates.begin(), midpoint});
    std::move(tmp.begin(), tmp.end(), p_variates.begin());

    // anti-thetic in the second half
    for (auto it = midpoint; it != p_variates.end(); ++it)
    {
        *it = 1.0 - *(it - half);
    }

    return std::move(p_variates);
}

template <typename Generator, size_t DIM>
void AntiThetic<Generator, DIM>::skip(size_t p_nPaths)
{
    // we only need to skip half the paths
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

} // namespace der

#endif // RANDOM_H
