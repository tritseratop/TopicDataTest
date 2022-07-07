# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("TopicDataTest")

set(CMAKE_CXX_STANDARD 20)

# Include sub-projects.
add_subdirectory("DdsWsGatewayService")

message(STATUS "Configuring PublisherProject...")
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

set(DDS_PUBLISHER_SOURCES_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/PublisherProject
)

foreach(SOURCE_DIR ${DDS_PUBLISHER_SOURCES_DIRS})
    aux_source_directory(${SOURCE_DIR} DDS_PUBLISHER_SOURCES)
endforeach()

add_library(PublisherProjectLib STATIC ${DDS_PUBLISHER_SOURCES})
target_link_libraries(PublisherProjectLib 
    fastrtps 
    fastcdr
    DdsUtilities
)

message(STATUS "Configuring WsClient...")
if(NOT oatpp_FOUND)
    find_package(oatpp REQUIRED)
endif()

if(NOT oatpp-websocket_FOUND)
    find_package(oatpp-websocket REQUIRED)
endif()


set(WS_CLIENT_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/WsClient
)

foreach(SOURCE_DIR ${WS_CLIENT_DIRS})
    aux_source_directory(${SOURCE_DIR} WS_CLIENT_SOURCES)
endforeach()

add_library(WsClientLib STATIC ${WS_CLIENT_SOURCES})
target_link_libraries(WsClientLib 
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-test
    PUBLIC oatpp::oatpp-websocket
    PUBLIC WsUtilities
)

message(STATUS "Configuring TestDdsWsGateway...")
enable_testing()
find_package(GTest REQUIRED)

set(TEST_SOURCES_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDdsWsGateway
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDdsWsGateway/Helpers
)

foreach(SOURCE_DIR ${TEST_SOURCES_DIRS})
    aux_source_directory(${SOURCE_DIR} TEST_SOURCES)
endforeach()

add_executable(RunUnitTests ${TEST_SOURCES})
target_link_libraries(RunUnitTests ${GTEST_BOTH_LIBRARIES} 
	fastrtps 
    fastcdr
	WsUtilities
	DdsUtilities
	DdsWsGatewayLib
	WsClientLib
	PublisherProjectLib
)

set(targets PublisherProjectLib WsClientLib WsClientLib RunUnitTests)

foreach(target ${targets})
    target_include_directories(${target} 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()