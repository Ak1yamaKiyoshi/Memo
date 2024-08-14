cmake_minimum_required(VERSION 3.13)
project(Memo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIB_SOURCE_FILES
    src/memories.cpp
    src/repetitions.cpp
    src/search.cpp
    src/string_utils.cpp
    src/utils.cpp
    src/vocablurary.cpp
)

add_library(libmemo STATIC ${LIB_SOURCE_FILES})
set(EXEC_SOURCE_FILES
    main.cpp
)

add_executable(memo ${EXEC_SOURCE_FILES})

target_link_libraries(memo libmemo)

set_target_properties(memo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(libmemo PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
