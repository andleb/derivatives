# derivatives
Based on [M. Joshi's C++ Design Patterns and Derivatives Pricing](https://www.amazon.com/gp/product/0521721628), making use of updated paradigms wherever possible. 

*C++17* strongly recommended.

## Dependencies & installation
[*cmake*](https://github.com/andleb/cmake) repository for the *CMake* scripts; included as a **submodule**.

[*common*](https://github.com/andleb/common) repository for utility classes & functions.

The project is built via *CMake*. One can, of course, modify the *CMake* configuration at will or use one's own.
The documentation is to be built with *Doxygen* from the provided *doxyfile*.

## Overview
The project roughly follows the second edition of the aforementioned book, which was published in 2008. While the numerics and mathematics haven't changed, *C++* has gone through quite a bit of a revolution in the time since. Due to this fact, the original book includes some now archaic elements, such as a self-implemented smart pointer. All such were replaced with their modern equivalents. Some chapters do not correspond to a concrete problem and have been excluded; in others, the sub-problems have often been consolidated/handled in a more generic way.

The code itself is separated into the driver/playground routines, located in *mains*, and the library, located in *src*.
The *mains* correspond to each relevant chapter. Take a look at the list of includes, corresponding to each executable in *CMakeLists.txt*, to follow along with the development of the library. 


#### Ch. 1 - 5:
These perform *Monte-Carlo* pricing of various derivatives, with an increasing degree of software architecture.

#### Ch. 6:
This chapter implements custom random number generators and integrates them with the architecture developed in preceding chapters.

#### Ch. 7:
Deals with the pricing of exotic, more specifically, path-dependent, options via *Monte-Carlo* using all the preceding infrastructure.

#### Ch. 8:
Switches gears and introduces pricing on *trees* (or *lattices*). For this, I have developed a helper static recombinant tree class that is found in the [*common*](https://github.com/andleb/common) repository (see above). This allows us to price early-exercise options.

#### Ch. 9:
Deals with the concept of *implied volatility* and introduces two simple numerical root-finding routines.

#### Ch. 10, 14:
Introduce the factory pattern & apply it to the existing architecture. The latter improves on the former by making the factory completely generic.

#### Final
As an exercise, I added pricing on trinomial trees. In the future, I intend to add low-level parallelization as an additional feature.


NOTE: I have stayed away from *C*-like input/output parameters and have replaced them with *rvalue* reference inputs that get moved into, modified in place, and returned by value (moved out). I feel that this syntax is clearer and also mirrors the approach popular *Python* numerics packages, such as *Pandas*, are taking with regards to in-place modification. There is also the added benefit of zero additional performance costs, afforded to us by modern *C++*.


## Authors

Andrej Leban

## Acknowledgments
Dr. Mark Joshi (RIP)
