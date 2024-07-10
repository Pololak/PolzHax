cmake_minimum_required(VERSION 3.0.0)

project(mat-dash)

add_library(mat-dash INTERFACE)

target_include_directories(mat-dash INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
