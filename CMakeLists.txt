cmake_minimum_required(VERSION 3.28)

project(zephyr_prometheus_library)

# add -Wall flag to enable all warnings for c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# include directories
include_directories(include)

# add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
