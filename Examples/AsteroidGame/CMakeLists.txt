cmake_minimum_required(VERSION 3.14)

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(MyProject
    Source/Main.cpp
    Source/ImGuiIntegration.cpp
)
target_include_directories(MyProject PRIVATE ../../Include)

add_subdirectory(External/raylib)
target_link_libraries(MyProject PRIVATE raylib)

project(imgui VERSION 1.89.9)
file(GLOB IMGUI_SOURCE_FILES External/imgui/*.cpp External/imgui/*.h)
add_library(imgui ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC External/imgui)
target_link_libraries(MyProject PRIVATE imgui)

project(raylib-extras VERSION 1.0)
file(GLOB RAYLIB_EXTRAS_SOURCE_FILES External/raylib-extras/*.cpp External/raylib-extras/*.h)
add_library(raylib-extras ${RAYLIB_EXTRAS_SOURCE_FILES})
target_link_libraries(raylib-extras PUBLIC imgui raylib)
target_include_directories(raylib-extras PUBLIC External/raylib-extras)
target_link_libraries(MyProject PRIVATE raylib-extras)

if(UNIX AND NOT APPLE)
    target_link_libraries(MyProject PRIVATE pthread dl m rt)
elseif(APPLE)
    target_link_libraries(MyProject PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()
