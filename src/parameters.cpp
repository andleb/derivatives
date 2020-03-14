/** \file parameters.cpp
 *  \author Andrej Leban
 *  \date 2/2019
 */

#include "parameters.h"

#include <cmath>

namespace der
{

// ParametersInner

ParametersInner::~ParametersInner() = default;

// Parameters

Parameters::Parameters(const ParametersInner & parameter) : m_pImpl(parameter.clone()) {}

Parameters::~Parameters() = default;

Parameters::Parameters(const Parameters & other) : m_pImpl(other.m_pImpl->clone()) {}

Parameters::Parameters(Parameters && other) noexcept = default;

Parameters & Parameters::operator=(const Parameters & other)
{
    if (this != &other)
    {
        m_pImpl = other.m_pImpl->clone();
    }

    return *this;
}

double Parameters::integral(double time1, double time2) const { return m_pImpl->integral(time1, time2); }

double Parameters::integralSquare(double time1, double time2) const { return m_pImpl->integralSquare(time1, time2); }

double Parameters::mean(double time1, double time2) const { return m_pImpl->integral(time1, time2) / (time2 - time1); }

double Parameters::RMS(double time1, double time2) const
{
    return std::sqrt(m_pImpl->integralSquare(time1, time2) / (time2 - time1));
}

Parameters & Parameters::operator=(Parameters && other) noexcept = default;

// ParametersConstant

ParametersConstant::ParametersConstant(double constant) : m_constant(constant) {}

ParametersConstant::~ParametersConstant() { delete m_strRepr; }

ParametersConstant::ParametersConstant(std::string p_str) : ParametersConstant(p_str.data()) {}

ParametersConstant::ParametersConstant(const char * p_str) : m_constant(std::atof(p_str)) {}

der::ParametersConstant::operator double() { return m_constant; }

der::ParametersConstant::operator double() const { return const_cast<ParametersConstant *>(this)->operator double(); }

der::ParametersConstant::operator const char *()
{
    m_strRepr = new char[16];
    std::snprintf(m_strRepr, 16, "%g", m_constant);
    return m_strRepr;
}

der::ParametersConstant::operator const char *() const { return const_cast<ParametersConstant *>(this)->operator const char *(); }

der::ParametersConstant::operator std::string() { return std::to_string(m_constant); }

der::ParametersConstant::operator std::string() const { return const_cast<ParametersConstant *>(this)->operator std::string(); }

std::unique_ptr<ParametersInner> ParametersConstant::clone() const { return std::make_unique<ParametersConstant>(m_constant); }

double ParametersConstant::integral(double time1, double time2) const { return m_constant * (time2 - time1); }

double ParametersConstant::integralSquare(double time1, double time2) const { return m_constant * m_constant * (time2 - time1); }

} // namespace der
