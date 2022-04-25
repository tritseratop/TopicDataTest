# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("TopicDataTest")

set(CMAKE_CXX_STANDARD 17)

# Include sub-projects.
add_subdirectory ("DdsWsGatewayUtilities")
add_subdirectory ("PublisherProject")
add_subdirectory ("DdsWsGateway")
add_subdirectory ("WsClient")
add_subdirectory ("TestDdsWsGateway")

# target_include_directories(SubscriberProject ${SOURCE_DIR}/src ${SOURCE_DIR} src)