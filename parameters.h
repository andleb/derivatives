/** \file parameters.h
 *  \author Andrej Leban
 *  \date 2/2018
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
    virtual double integral() const = 0;
    virtual double integralSquare() const = 0;
};

class Parameters
{

public:
    Parameters(const ParametersInner & parameter);
    ~Parameters();
    Parameters(const Parameters & other);
    Parameters(Parameters && other) noexcept;
    Parameters & operator=(const Parameters & other);
    Parameters & operator=(Parameters && other) noexcept;

private:
    std::unique_ptr<ParametersInner> m_pImpl;
};

} // namespace der

#endif // PARAMETERS_H
