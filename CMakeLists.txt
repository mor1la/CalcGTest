cmake_minimum_required(VERSION 3.22)
project(CalcGTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)


add_library(CalcLib
    ICalculator.h
    Consts.h
    SimpleCalculator.h SimpleCalculator.cpp
    IHistory.h
    InMemoryHistory.h InMemoryHistory.cpp
)


enable_testing()

add_executable(CalcTests
    tests/SimpleCalculatorTest.cpp  
)

target_link_libraries(CalcTests
    CalcLib          
    gtest
    gmock
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(CalcTests)
