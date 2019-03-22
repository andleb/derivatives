cmake_minimum_required( VERSION 3.5 )

project( Derivatives CXX )

#NOTE: uncomment & modify this if building outside an IDE
include(cmake/definitions.cmake )
include(cmake/dirs.cmake)

#NOTE: CMAKE_BUILD_TYPE is usually set by the IDE
#if( CMAKE_BUILD_TYPE MATCHES "Debug" )
#    add_definitions( -DTESTING )
#endif()

#for headers
#include_directories( . )
# or
set(HEADERS
    derivatives.h
    )

set(SOURCES
    )

# set(ANALYZER
#     analyzermain.cpp
#     )

# set(GENERATOR
#     generatormain.cpp
#     )

#NOTE: this includes code specific to versioned build of boost, i.e. names are versioned by compiler & stdlib & debug & multithreaded
#include(cmake/libraries/threads.cmake)
#include(cmake/libraries/boost.cmake)
include(cmake/libraries/common.cmake)

add_executable(mc1
    ${HEADERS}
    mc1.cpp
    )

add_executable(mc2
    ${HEADERS}
    payoff1.h
    payoff1.cpp
    mc2.cpp
    )

add_executable(mc3
    ${HEADERS}
    payoff2.h
    payoff2.cpp
    mc3.cpp
    )

add_executable(mc4
    ${HEADERS}
    payoff2.h
    payoff2.cpp
    vanillaoption.h
    vanillaoption.cpp
    payoffbridge.h
    payoffbridge.cpp
    mc4.cpp
    )

add_executable(mc5
    ${HEADERS}
    payoff2.h
    payoff2.cpp
    vanillaoption2.h
    vanillaoption2.cpp
    payoffbridge.h
    payoffbridge.cpp
    parameters.h
    parameters.cpp
    mc5.cpp
    )

add_executable(mc6
    ${HEADERS}
    parameters.h
    parameters.cpp
    statistics.h
    statistics.cpp
    payoff2.h
    payoff2.cpp
    vanillaoption2.h
    vanillaoption2.cpp
    mc6.cpp
    )

add_executable(mc7
    ${HEADERS}
    parameters.h
    parameters.cpp
    statistics.h
    statistics.cpp
    random.h
    random.cpp
    mc7.cpp
    )

target_link_libraries( mc1 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc2 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc3 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc4 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc5 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc6 ${PROJECT_LINK_LIBS} )
target_link_libraries( mc7 ${PROJECT_LINK_LIBS} )

