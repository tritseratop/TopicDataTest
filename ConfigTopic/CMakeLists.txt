cmake_minimum_required (VERSION 3.8)

project(ConfigTopic)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

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

file(GLOB DDS_CONFIG_TOPIC_CPP "*.cxx")

# Add source to this project's executable.
#add_library (ConfigTopic  ${DDS_CONFIG_TOPIC_CPP})
#target_link_libraries(ConfigTopic fastrtps fastcdr)