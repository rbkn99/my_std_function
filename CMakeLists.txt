cmake_minimum_required(VERSION 3.12)
project(my_std_function)

set(CMAKE_CXX_STANDARD 14)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()

# add_executable(my_std_function function.h main.cpp)
add_executable(my_std_function function.h functionTest.cpp)