# CMakeList.txt : CMake project for WsClient, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(WsClient)

# Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

message(STATUS "Configuring HelloWorld example...")

find_package(oatpp REQUIRED)
find_package(oatpp-websocket REQUIRED)

message(STATUS "Configuring HelloWorld example...")

file(GLOB WS_CLIENT "*.cpp" "*.h" "*.hpp")
file(GLOB WS_CLIENT_LIB "WSClient.cpp" "WSListener.cpp")

# Add source to this project's executable.
add_executable (WsClient  ${WS_CLIENT})
add_library (WsClientLib  ${WS_CLIENT_LIB})

target_link_libraries(WsClient 
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-websocket
        PUBLIC DdsWsGatewayUtilities
        PUBLIC DdsWsGatewayLib
)
target_link_libraries(WsClientLib 
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-websocket
        PUBLIC DdsWsGatewayUtilities
        PUBLIC DdsWsGatewayLib
)

# TODO: Add tests and install targets if needed.
