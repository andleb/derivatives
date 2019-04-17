/** \file exotics1.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include "exoticengine.h"
#include "pathdependent.h"
#include "random.h"

using namespace der;

int main( int /*argc*/, char */*argv*/[] )
{

    ExoticBSEngine<PathDependent, RandomParkMiller<1>> engine {};

    return 0;
}
