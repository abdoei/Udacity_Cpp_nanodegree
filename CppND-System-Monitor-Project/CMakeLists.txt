cmake_minimum_required(VERSION 3.22)

project("System Monitor")

enable_testing()
include(CTest)

# add the build configuration as a subdirectory
add_subdirectory(src)

# add the test configuration as a subdirectory
add_subdirectory(test)

# set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# add_subdirectory(thirdparty/googletest)
# include(GoogleTest)
# gtest_discover_tests(MyTests)