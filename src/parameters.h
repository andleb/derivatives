/** \file parameters.h
 *  \author Andrej Leban
 *  \date 2/2019
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <memory>

namespace der
{

class ParametersInner
{

public:
    ParametersInner() = default;
    virtual ~ParametersInner();
    ParametersInner(const ParametersInner &) = default;
    ParametersInner(ParametersInner &&) = default;
    ParametersInner & operator=(const ParametersInner &) = default;
    ParametersInner & operator=(ParametersInner &&) = default;

    virtual std::unique_ptr<ParametersInner> clone() const = 0;
    virtual double integral(double time1, double time2) const = 0;
    virtual double integralSquare(double time1, double time2) const = 0;
};

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

    double integral(double time1, double time2) const;
    double integralSquare(double time1, double time2) const;
    double mean(double time1, double time2) const;
    double RMS(double time1, double time2) const;

private:
    std::unique_ptr<ParametersInner> m_pImpl {nullptr};
};

class ParametersConstant : public ParametersInner
{

public:
    ParametersConstant(double constant)
        : m_constant(constant)
    {}

    std::unique_ptr<ParametersInner> clone() const override;
    double integral(double time1, double time2) const override;
    double integralSquare(double time1, double time2) const override;

private:
    double m_constant {0.0};
};

} // namespace der

#endif // PARAMETERS_H
