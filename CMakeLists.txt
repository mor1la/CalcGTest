cmake_minimum_required(VERSION 3.22)
project(CalcGTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Подключаем FetchContent ===
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Загружаем и делаем доступным GTest + GMock
FetchContent_MakeAvailable(googletest)

# === Основная библиотека проекта ===
add_library(CalcLib
    ICalculator.h
    SimpleCalculator.h SimpleCalculator.cpp
    IHistory.h
    InMemoryHistory.h InMemoryHistory.cpp
)

# === Тесты ===
enable_testing()

add_executable(CalcTests
    tests/SimpleCalculatorTest.cpp  
)

target_link_libraries(CalcTests
    CalcLib          # наша библиотека
    gtest
    gmock
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(CalcTests)