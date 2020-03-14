/** \file parameters.h
 *  \author Andrej Leban
 *  \date 2/2019
 *
 * PIMPL class that allows for generic parameter types
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <memory>

namespace der
{


//! \brief The abstract base class for parameter implementations
class ParametersInner
{

public:
    ParametersInner() = default;
    virtual ~ParametersInner();
    ParametersInner(const ParametersInner &) = default;
    ParametersInner(ParametersInner &&) = default;
    ParametersInner & operator=(const ParametersInner &) = default;
    ParametersInner & operator=(ParametersInner &&) = default;

    //! \brief clone
    //! \return
    virtual std::unique_ptr<ParametersInner> clone() const = 0;
    //! \brief a definite integral
    //! \param time1
    //! \param time2
    //! \return
    virtual double integral(double time1, double time2) const = 0;
    //! \brief a definite integral of the square of the function
    //! \param time1
    //! \param time2
    //! \return
    virtual double integralSquare(double time1, double time2) const = 0;
};

//! \brief The PIPML interface class
class Parameters
{

public:
    Parameters() = default;
    Parameters(const ParametersInner & parameter);

    ~Parameters();
    Parameters(const Parameters & other);
    Parameters(Parameters && other) noexcept;

    Parameters & operator=(const Parameters & other);
    Parameters & operator=(Parameters && other) noexcept;

    //! \brief a definite integral
    //! \param time1
    //! \param time2
    //! \return
    double integral(double time1, double time2) const;
    //! \brief a definite integral of the square of the function
    //! \param time1
    //! \param time2
    //! \return
    double integralSquare(double time1, double time2) const;
    //! \brief the mean value on the interval
    //! \param time1
    //! \param time2
    //! \return
    double mean(double time1, double time2) const;
    //! \brief the root-mean-square on the interval
    //! \param time1
    //! \param time2
    //! \return
    double RMS(double time1, double time2) const;

private:
    std::unique_ptr<ParametersInner> m_pImpl{nullptr};
};

//! \brief A constant-type implementation
class ParametersConstant : public ParametersInner
{

public:
    ParametersConstant() = default;
    // no explicit since we want implicit conversion
    ParametersConstant(double constant);

    virtual ~ParametersConstant() override;

    //! \name Conversions
    //!@{

    //! \brief Parsing constructors
    //! \param p_str
    ParametersConstant(std::string p_str);
    ParametersConstant(const char * p_str);

    //! \brief Conversion operators
    operator double();
    operator double() const;

    operator const char *();
    operator const char *() const;

    operator std::string();
    operator std::string() const;
    //!@}

    std::unique_ptr<ParametersInner> clone() const override;
    double integral(double time1, double time2) const override;
    double integralSquare(double time1, double time2) const override;

private:
    double m_constant{0.0};

    //! \brief To enable implicit conversions, e.g. for printing
    char * m_strRepr{nullptr};
};

} // namespace der

#endif // PARAMETERS_H
