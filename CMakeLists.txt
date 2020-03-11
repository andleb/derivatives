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

add_executable(ch1
    ${HEADERS}
    mains/ch1.cpp
    )

add_executable(ch2
    ${HEADERS}
    src/payoff1.h
    src/payoff1.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/ch2.cpp
    )

add_executable(ch3
    ${HEADERS}
    src/payoff.h
    src/payoff.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    mains/ch3.cpp
    )

add_executable(ch4
    ${HEADERS}
    src/payoff.h
    src/payoff.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    src/vanillaoption.h
    src/vanillaoption.cpp
    mains/ch4.cpp
    )

add_executable(ch5
    ${HEADERS}
    src/payoff.h
    src/payoff.cpp
    src/parameters.h
    src/parameters.cpp
    src/simspot.h
    src/simspot.cpp
    src/statistics.h
    src/statistics.cpp
    src/vanillaoption.h
    src/vanillaoption.cpp
    mains/ch5.cpp
    )

add_executable(ch6
    ${HEADERS}
    src/parameters.h
    src/parameters.cpp
    src/payoff.h
    src/payoff.cpp
    src/simspot.h
    src/simspot.cpp
    src/statistics.h
    src/statistics.cpp
    src/random.h
    src/vanillaoption.h
    src/vanillaoption.cpp
    mains/ch6.cpp
    )

add_executable(exotics1
    ${HEADERS}
    src/exoticengine.h
    src/exoticengine.cpp
    src/parameters.h
    src/parameters.cpp
    src/payoff.h
    src/payoff.cpp
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
    src/payoff.h
    src/payoff.cpp
    src/random.h
    src/statistics.h
    src/statistics.cpp
    mains/trees1.cpp
    )

target_link_libraries(ch1 ${PROJECT_LINK_LIBS})
target_link_libraries(ch2 ${PROJECT_LINK_LIBS})
target_link_libraries(ch3 ${PROJECT_LINK_LIBS})
target_link_libraries(ch4 ${PROJECT_LINK_LIBS})
target_link_libraries(ch5 ${PROJECT_LINK_LIBS})
target_link_libraries(ch6 ${PROJECT_LINK_LIBS})
target_link_libraries(exotics1 ${PROJECT_LINK_LIBS})
target_link_libraries(trees1 ${PROJECT_LINK_LIBS})
