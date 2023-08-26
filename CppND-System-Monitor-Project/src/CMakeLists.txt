cmake_minimum_required(VERSION 3.22)

project("System Monitor build")

add_executable(SystemMonitor main.cpp)

target_link_libraries(SystemMonitor ncurses)
