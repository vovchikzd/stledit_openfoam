cmake_minimum_required(VERSION 3.27.1)

project(stlEdit
    LANGUAGES CXX)

set (CMAKE_CXX_STANDART 20)

add_executable(
    stlEdit
    main.cpp
    src/parcer.cpp
    headers/parcer.h)
