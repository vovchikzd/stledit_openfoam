cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(stlEdit
    LANGUAGES CXX)

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

