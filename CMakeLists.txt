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
include(cmake/libraries/threads.cmake)
include(cmake/libraries/boost.cmake)
#include(cmake/libraries/common.cmake)

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
    mc3.cpp
    )


target_link_libraries( mc2 ${PROJECT_LINK_LIBS} )
# target_link_libraries( generator ${PROJECT_LINK_LIBS} )
