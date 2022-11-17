cmake_minimum_required(VERSION 3.23)
project(RSA)

set(CMAKE_CXX_STANDARD 14)

add_executable(RSA src/main.cpp src/BigInteger.cpp src/BigInteger.h src/utils.h src/SmallPrimeSieve.cpp src/SmallPrimeSieve.h src/rsa.h)

add_subdirectory(./googletest)
include_directories(./googletest/googletest/include ./googletest/googletest ./src)

add_executable(GooGleTests test/FunctionalTests.cpp src/BigInteger.cpp src/BigInteger.h src/utils.h src/SmallPrimeSieve.cpp src/rsa.h src/SmallPrimeSieve.h test/PerformanceTests.cpp)
target_link_libraries(GooGleTests gtest gtest_main)