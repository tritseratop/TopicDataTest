cmake_minimum_required(VERSION 3.0)
project(TestService)

# GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_BUILD_TYPE Debug)

# Unit Tests
# Add test cpp file
add_executable( RunUnitTests Tests.cpp)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc" "-static-libstdc++")
# Link test executable against gtest & gtest_main
target_link_libraries(RunUnitTests ${GTEST_BOTH_LIBRARIES} DataTransferService)
add_test( RunUnitTests RunUnitTests)

#include(GoogleTest)
#gtest_discover_tests(RunUnitTests)