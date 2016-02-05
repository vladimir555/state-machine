project("state-machine")
cmake_minimum_required(VERSION 2.8)
aux_source_directory(source SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR}/source)

add_compile_options("-std=c++11")
add_compile_options("-Wall")

file(GLOB_RECURSE SOURCE
    ${CMAKE_SOURCE_DIR}/source/*.h
    ${CMAKE_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})
