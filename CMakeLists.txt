# Let CMake choose proper file extension (e.g. .exe on Windows)
set(TARGET_NAME SimpleRenderer)
add_executable(${TARGET_NAME} src/glad.c src/main.cpp)

target_include_directories(${TARGET_NAME} PRIVATE include)

find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${TARGET_NAME} glfw OpenGL::GL assimp::assimp)
