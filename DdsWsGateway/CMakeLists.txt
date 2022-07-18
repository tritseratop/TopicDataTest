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
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/DdsService/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/DdsService
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/WsService
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib
)

set(COMMON_UTILITIES_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Common/nlohmann
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Common/ThreadSafeQueue
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Common/TimeConverter
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Common
)

set(DDS_UTILITIES_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/ConfigTopic
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/DDSAlarm
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/DDSAlarmEx
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/DDSData
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/DDSDataEx
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/TopicCommand
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds/TypeTopicsDDS/TopicStatus
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Dds
)

set(WS_UTILITIES_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Ws
)

set(GATEWAY_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(SOURCE_DIR ${LIB_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} LIB_SOURCES)
endforeach()

foreach(SOURCE_DIR ${COMMON_UTILITIES_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} COMMON_UTILITIES_SOURCES)
endforeach()
foreach(SOURCE_DIR ${DDS_UTILITIES_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} DDS_UTILITIES_SOURCES)
endforeach()
foreach(SOURCE_DIR ${WS_UTILITIES_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} WS_UTILITIES_SOURCES)
endforeach()

foreach(SOURCE_DIR ${GATEWAY_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} GATEWAY_SOURCES)
endforeach()

add_library(CommonUtilities STATIC ${COMMON_UTILITIES_SOURCES})
add_library(DdsUtilities STATIC ${DDS_UTILITIES_SOURCES})
add_library(WsUtilities STATIC ${WS_UTILITIES_SOURCES})

add_library(DdsWsGatewayLib STATIC ${LIB_SOURCES})

add_executable(DdsWsGatewayService ${GATEWAY_SOURCES})

set (utilities_targets CommonUtilities DdsUtilities WsUtilities)

foreach(target ${utilities_targets})
    target_include_directories(${target}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

target_link_libraries(DdsUtilities 
    PUBLIC fastrtps 
    PUBLIC fastcdr
)

target_link_libraries(WsUtilities 
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-test
    PUBLIC oatpp::oatpp-websocket
    PUBLIC CommonUtilities
)

set(targets DdsWsGatewayLib DdsWsGatewayService)

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
        PUBLIC CommonUtilities
        PUBLIC DdsUtilities
        PUBLIC WsUtilities
    )
endforeach ()

target_link_libraries(DdsWsGatewayService 
        PUBLIC DdsWsGatewayLib 
    )