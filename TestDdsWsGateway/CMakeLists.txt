cmake_minimum_required(VERSION 3.0)
project(TestService)

# GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

#set(CMAKE_BUILD_TYPE Debug)

# Unit Tests
# Add test cpp file
file(GLOB TEST_SOURCES "*.cpp" "Helpers/*.cpp")

add_executable (RunUnitTests ${TEST_SOURCES})
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc" "-static-libstdc++")
# Link test executable against gtest & gtest_main
target_link_libraries(RunUnitTests ${GTEST_BOTH_LIBRARIES} 
	fastrtps 
    fastcdr
	DdsWsGatewayUtilities
	DdsWsGatewayLib
	WsClientLib
	PublisherProjectLib
)
add_test(RunUnitTests RunUnitTests)

#include(GoogleTest)
#gtest_discover_tests(RunUnitTests)