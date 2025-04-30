cmake_minimum_required(VERSION 3.5.0)
project(SimpleRenderer VERSION 0.1.0 LANGUAGES C CXX)

add_executable(SimpleRenderer src/glad.c src/main.cpp)

target_include_directories(SimpleRenderer PRIVATE include)

find_package(assimp REQUIRED)
target_link_libraries(SimpleRenderer glfw GL assimp::assimp )