cmake_minimum_required(VERSION 3.5)

project(Derivatives CXX)

# NOTE: these definitions can and should be overriden by the IDE
include(cmake/definitions.cmake)
include(cmake/dirs.cmake)

# NOTE: CMAKE_BUILD_TYPE can and should be overriden by the IDE
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DTESTING)
#    add_definitions(-DDUMPINTERMEDIATE)
endif()

# Headers are located in root
include_directories(.)
set(HEADERS
    derivatives.h
    derivatives.cpp
   )


#include(cmake/libraries/threads.cmake)
#include(cmake/libraries/boost.cmake)
include(cmake/libraries/common.cmake)

add_executable(mc1
    ${HEADERS}
    mains/mc1.cpp
    )

add_executable(mc2
    ${HEADERS}
    payoff1.h
    payoff1.cpp
    mains/mc2.cpp
    )

add_executable(mc3
    ${HEADERS}
    payoff2.h
    payoff2.cpp
    mains/mc3.cpp
    )

add_executable(mc4
    ${HEADERS}
    payoff2.h
    payoff2.cpp
    vanillaoption.h
    vanillaoption.cpp
    payoffbridge.h
    payoffbridge.cpp
    mains/mc4.cpp
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
    mains/mc5.cpp
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
    mains/mc6.cpp
    )

add_executable(mc7
    ${HEADERS}
    parameters.h
    parameters.cpp
    payoff2.h
    payoff2.cpp
    statistics.h
    statistics.cpp
    random.h
    mains/mc7.cpp
    )

add_executable(exotics1
    ${HEADERS}
    exoticengine.h
    exoticengine.cpp
    parameters.h
    parameters.cpp
    payoff2.h
    payoff2.cpp
    pathdependent.h
    pathdependent.cpp
    random.h
    statistics.h
    statistics.cpp
    mains/exotics1.cpp
    )

add_executable(trees1
    ${HEADERS}
    treeproduct.h
    treeproduct.cpp
    parameters.h
    parameters.cpp
    payoff2.h
    payoff2.cpp
    random.h
    statistics.h
    statistics.cpp
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
