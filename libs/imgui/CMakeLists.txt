cmake_minimum_required(VERSION 3.21)

add_library(imgui STATIC
    imgui.cpp
	imgui_widgets.cpp
	imgui_tables.cpp
    imgui_draw.cpp
    imgui_demo.cpp
    backends/imgui_impl_win32.cpp
    backends/imgui_impl_opengl3.cpp
    misc/cpp/imgui_stdlib.cpp
)