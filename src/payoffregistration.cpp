/** \file payoffregistration.cpp
 * \author Andrej Leban
 * \date 3/2020
 *
 * A list of pre-defined classes we wish to register with der::payoffFactory at startup.
 */

#include "payoff.h"
#include "payoffregisterer.h"

// Neat trick with global vars in anonymous namespace - instantiated, but unreachable.

namespace
{
using namespace der;
payoffRegisterer<PayoffCall> call{"call"};
payoffRegisterer<PayoffPut> put{"put"};
payoffRegisterer<PayoffForward> forward{"forward"};

} // anonyomous namespace
