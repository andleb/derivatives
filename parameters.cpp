/** \file parameters.cpp
 *  \author Andrej Leban
 *  \date 2/2018
 */
#include "parameters.h"

namespace der
{

ParametersInner::~ParametersInner() = default;

Parameters::Parameters(const ParametersInner & parameter)
    : m_pImpl(parameter.clone())
{}

Parameters::~Parameters() = default;

Parameters::Parameters(const Parameters & other)
    : m_pImpl(other.m_pImpl->clone())
{}

Parameters::Parameters(Parameters && other) noexcept = default;

Parameters & Parameters::operator=(const Parameters & other)
{
    if (this != &other)
    {
        m_pImpl = other.m_pImpl->clone();
    }

    return *this;
}

double Parameters::integral(double time1, double time2) const
{
    return m_pImpl->integral(time1, time2);
}

double Parameters::integralSquare(double time1, double time2) const
{
    return m_pImpl->integralSquare(time1, time2);
}

double Parameters::mean(double time1, double time2) const
{
    return m_pImpl->integral(time1, time2) / (time2 - time1);
}

double Parameters::RMS(double time1, double time2) const
{
    return m_pImpl->integralSquare(time1, time2) / (time2 - time1);
}

Parameters & Parameters::operator=(Parameters && other) noexcept = default;

std::unique_ptr<ParametersInner> ParametersConstant::clone() const
{
    return std::make_unique<ParametersConstant>(m_constant);
}

double ParametersConstant::integral(double time1, double time2) const
{
    return m_constant * (time2 - time1);
}

double ParametersConstant::integralSquare(double time1, double time2) const
{
    return m_constant * m_constant * (time2 - time1);
}

} // namespace der
