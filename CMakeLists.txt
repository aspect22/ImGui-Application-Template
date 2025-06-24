cmake_minimum_required(VERSION 3.10.0)
project(templateAPP VERSION 0.1.0 LANGUAGES C CXX)

add_executable(templateAPP
    main.cpp
)

find_package(glad CONFIG REQUIRED)
target_link_libraries(templateAPP PRIVATE glad::glad)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(templateAPP PRIVATE imgui::imgui glfw opengl32)
