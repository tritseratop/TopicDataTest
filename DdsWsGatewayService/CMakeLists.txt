# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (DdsWsGatewayService)

set(CMAKE_CXX_STANDARD 17)

# Include sub-projects.
add_subdirectory ("Lib")
add_subdirectory ("Utilities")

add_executable(DdsWsGatewayService "main.cpp")

target_link_libraries(DdsWsGatewayService ${GTEST_BOTH_LIBRARIES} 
	DdsWsGateway
	DdsWsGatewayUtilities
)


# target_include_directories(SubscriberProject ${SOURCE_DIR}/src ${SOURCE_DIR} src)