cmake_minimum_required (VERSION 3.8)

project(DdsWsGateway)

if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()
if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

if(NOT oatpp_FOUND)
    find_package(oatpp REQUIRED)
endif()
if(NOT oatpp-websocket_FOUND)
    find_package(oatpp-websocket REQUIRED)
endif()

set(LIB_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/DdsService
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/WsService
)

set(UTILITIES_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/ConfigTopic
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/nlohmann
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TestUtilities
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/ThreadSafeQueue
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TimeConverter
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/DDSAlarm
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/DDSAlarmEx
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/DDSData
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/DDSDataEx
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/TopicCommand
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TypeTopicsDDS/TopicStatus
)

set(GATEWAY_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(SOURCE_DIR ${LIB_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} LIB_SOURCES)
endforeach()

foreach(SOURCE_DIR ${UTILITIES_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} UTILITIES_SOURCES)
endforeach()

foreach(SOURCE_DIR ${GATEWAY_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} GATEWAY_SOURCES)
endforeach()

add_library(DdsWsGatewayLib STATIC ${LIB_SOURCES})
add_library(DdsWsGatewayUtilities STATIC ${UTILITIES_SOURCES})

add_executable(DdsWsGatewayService ${GATEWAY_SOURCES})

set(targets DdsWsGatewayLib DdsWsGatewayUtilities DdsWsGatewayService)
#set(targets DdsWsGatewayLib DdsWsGatewayUtilities)

foreach (target ${targets})
    target_include_directories(${target} 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(${target} 
        PUBLIC fastrtps 
        PUBLIC fastcdr
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-websocket
    )
endforeach ()

target_link_libraries( DdsWsGatewayService 
        PUBLIC DdsWsGatewayUtilities
        PUBLIC DdsWsGatewayLib 
    )