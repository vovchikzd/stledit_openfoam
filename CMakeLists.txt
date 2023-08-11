cmake_minimum_required(VERSION 3.25)

project(stlEdit
    LANGUAGES CXX)

set (CMAKE_CXX_STANDART 20)

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

add_compile_options(-Wall -Wextra)
