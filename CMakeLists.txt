cmake_minimum_required(VERSION 3.5)

project(qvinace VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

find_package(Qt6 COMPONENTS Core Widgets Quick QuickWidgets REQUIRED)


if(MSVC)
    add_compile_options(/MP)
endif()

add_subdirectory(src)



