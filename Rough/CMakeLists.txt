cmake_minimum_required(VERSION 3.8)
project(Rough)
set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES Node.h Rand_int.h Utils.cpp Utils.h pearls.cpp)
set(SOURCE_FILES main.cpp Funcs.cpp Funcs.h)
add_executable(Rough ${SOURCE_FILES})