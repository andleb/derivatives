cmake_minimum_required( VERSION 3.5 )

project( Derivatives CXX )

#NOTE: uncomment & modify this if building outside an IDE
include(cmake/definitions.cmake )
include(cmake/dirs.cmake)

#NOTE: CMAKE_BUILD_TYPE is usually set by the IDE
if( CMAKE_BUILD_TYPE MATCHES "Debug" )
    add_definitions( -DTESTING )
endif()

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
include(cmake/libraries/common.cmake)

add_executable(main
    ${HEADERS}
    ${SOURCES}
    )

# add_executable(generator
#     ${HEADERS}
#     ${SOURCES}
#     ${GENERATOR}
#     )


target_link_libraries( main ${PROJECT_LINK_LIBS} )
# target_link_libraries( generator ${PROJECT_LINK_LIBS} )
