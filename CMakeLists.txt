cmake_minimum_required(VERSION 3.5)

project(Derivatives CXX)

# NOTE: these definitions can and should be overriden by the IDE
include(cmake/definitions.cmake)
include(cmake/dirs.cmake)

# NOTE: CMAKE_BUILD_TYPE can and should be overriden by the IDE
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Building in DEBUG mode, enabling testing macros.")
    add_definitions(-DTESTING)
#    add_definitions(-DDUMPINTERMEDIATE)
endif()

# Common headers
set(HEADERS
    src/derivatives.h
   )

# External dependencies
# My roll-your-own library of data structures etc.
include(cmake/libraries/common.cmake)

add_executable(mc1
    ${HEADERS}
    mains/mc1.cpp
    )

add_executable(mc2
    ${HEADERS}
    src/payoff1.h
    src/payoff1.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/mc2.cpp
    )

add_executable(mc3
    ${HEADERS}
    src/payoff2.h
    src/payoff2.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/mc3.cpp
    )

add_executable(mc4
    ${HEADERS}
    src/payoff2.h
    src/payoff2.cpp
    src/vanillaoption.h
    src/vanillaoption.cpp
    src/payoffbridge.h
    src/payoffbridge.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/mc4.cpp
    )

add_executable(mc5
    ${HEADERS}
    src/payoff2.h
    src/payoff2.cpp
    src/vanillaoption2.h
    src/vanillaoption2.cpp
    src/payoffbridge.h
    src/payoffbridge.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/mc5.cpp
    )

add_executable(mc6
    ${HEADERS}
    src/parameters.h
    src/parameters.cpp
    src/statistics.h
    src/statistics.cpp
    src/payoff2.h
    src/payoff2.cpp
    src/vanillaoption2.h
    src/vanillaoption2.cpp
    src/simspot.h
    src/simspot.cpp
    mains/mc6.cpp
    )

add_executable(mc7
    ${HEADERS}
    src/parameters.h
    src/parameters.cpp
    src/payoff2.h
    src/payoff2.cpp
    src/simspot.h
    src/simspot.cpp
    src/statistics.h
    src/statistics.cpp
    src/random.h
    mains/mc7.cpp
    )

add_executable(exotics1
    ${HEADERS}
    src/exoticengine.h
    src/exoticengine.cpp
    src/parameters.h
    src/parameters.cpp
    src/payoff2.h
    src/payoff2.cpp
    src/pathdependent.h
    src/pathdependent.cpp
    src/random.h
    src/statistics.h
    src/statistics.cpp
    mains/exotics1.cpp
    )

add_executable(trees1
    ${HEADERS}
    src/treeproduct.h
    src/treeproduct.cpp
    src/tree.h
    src/tree.cpp
    src/parameters.h
    src/parameters.cpp
    src/payoff2.h
    src/payoff2.cpp
    src/random.h
    src/statistics.h
    src/statistics.cpp
    mains/trees1.cpp
    )

target_link_libraries(mc1 ${PROJECT_LINK_LIBS})
target_link_libraries(mc2 ${PROJECT_LINK_LIBS})
target_link_libraries(mc3 ${PROJECT_LINK_LIBS})
target_link_libraries(mc4 ${PROJECT_LINK_LIBS})
target_link_libraries(mc5 ${PROJECT_LINK_LIBS})
target_link_libraries(mc6 ${PROJECT_LINK_LIBS})
target_link_libraries(mc7 ${PROJECT_LINK_LIBS})
target_link_libraries(exotics1 ${PROJECT_LINK_LIBS})
target_link_libraries(trees1 ${PROJECT_LINK_LIBS})
