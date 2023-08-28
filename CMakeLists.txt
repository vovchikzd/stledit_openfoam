cmake_minimum_required(VERSION 3.25)

project(stlEdit
    LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_COMPILER clang++)
#set (CMAKE_CXX_COMPILER g++)
add_compile_options(-Wall -Wextra)

file(GLOB sources CONFIGURE_DEPENDS
    "src/*.cpp")

add_executable(stlEdit
    main.cpp)

target_include_directories(stlEdit
    PRIVATE
    headers/)

target_sources(stlEdit
    PRIVATE
    ${sources})
