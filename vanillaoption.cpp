/** \file vanillaoption.cpp
*   \author Andrej Leban
*   \date 2/2018
*/

#include "vanillaoption.h"

namespace der {

double VanillaOption::optionPayoff(double spot) const
{
    return (*m_pPayoff)(spot);
}


} // namespace der
