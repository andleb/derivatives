/** \file payofffactory.h
 * \author Andrej Leban
 * \date 3/2020
 *
 * A Factory Method (Gang of 4 terminology) class for der::Payoffs.
 */


#ifndef DER_PAYOFFFACTORY_H
#define DER_PAYOFFFACTORY_H

#include <map>
#include <memory>
#include <string>


namespace der
{

class Payoff;

//! \brief A Factory Method class for Payoffs.
class PayoffFactory
{
public:
    // virtual constructor, takes a strike
    using PayoffFactoryMethod = std::unique_ptr<Payoff> (*)(double);

    //! \brief The singleton instance.
    static PayoffFactory & instance();

    //! \brief Registers a factory method \p p_factory for Payoff class \p p_name.
    //! \param p_name
    //! \param p_factory
    // could also be templated, but we wish to enforce type safety.
    void registerPayoff(std::string p_name, PayoffFactoryMethod p_factory);
    //! \brief Creates the Payoff class stored under \p p_name.
    //! \param p_name
    //! \param p_strike
    //! \return A std::unique_ptr to the payoff class requested,
    //!  or a nullptr if \p p_name not registered.
    std::unique_ptr<der::Payoff> createPayoff(std::string p_name, double p_strike);

protected:
    PayoffFactory() = default;

private:
    std::map<std::string, PayoffFactoryMethod> m_registeredFactories {};
};

} // namespace der

#endif // DER_PAYOFFFACTORY_H
