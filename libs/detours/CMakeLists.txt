cmake_minimum_required(VERSION 3.21)

add_library(detours INTERFACE)
target_link_libraries(detours INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libdetours.lib)