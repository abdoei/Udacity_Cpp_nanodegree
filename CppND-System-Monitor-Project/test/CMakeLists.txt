cmake_minimum_required(VERSION 3.22)

project("System Monitor Tests")

# add the source files to the project
set(SOURCES testing.cpp)

# add the executable for the tests
add_executable(MyTests ${SOURCES})

# link the gtest and gtest_main libraries as well as the pthread library
target_link_libraries(MyTests gtest gtest_main pthread)

# Enable CTest testing
option(BUILD_TESTING "Build tests" ON)

# add a test for the MyTests executable
add_test(NAME MyTest COMMAND MyTests)
