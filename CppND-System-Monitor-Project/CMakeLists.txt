cmake_minimum_required(VERSION 3.22)

project("System Monitor")

enable_testing()
include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# add the build configuration as a subdirectory
add_subdirectory(src)

# add the test configuration as a subdirectory
add_subdirectory(test)