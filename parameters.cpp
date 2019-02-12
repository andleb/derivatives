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

Parameters & Parameters::operator=(Parameters && other) noexcept = default;

} // namespace der
