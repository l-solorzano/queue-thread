cmake_minimum_required(VERSION 3.12)

project(queue-thread)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(source)
add_subdirectory(testing)

enable_testing()
