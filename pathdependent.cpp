/** \file pathdependent.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include "pathdependent.h"

namespace der {

PathDependent::~PathDependent() = default;

std::vector<double> PathDependent::lookAtTimes() const
{
    return m_lookAtTimes;
}

} // namespace der
