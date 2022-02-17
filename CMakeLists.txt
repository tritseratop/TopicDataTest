# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("TopicDataTest")

# Include sub-projects.
add_subdirectory ("PublisherProject")
add_subdirectory ("SubscriberProject")

# target_include_directories(SubscriberProject ${SOURCE_DIR}/src ${SOURCE_DIR} src)