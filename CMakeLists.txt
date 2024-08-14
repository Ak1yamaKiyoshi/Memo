cmake_minimum_required(VERSION 3.13)
project(Memo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_SOURCE_FILES main.cpp)

add_library(libmemo ${LIB_SOURCE_FILES})

set(EXEC_SOURCE_FILES main.cpp)

add_executable(memo ${EXEC_SOURCE_FILES})
target_link_libraries(memo libmemo)
