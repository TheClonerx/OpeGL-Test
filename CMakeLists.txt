cmake_minimum_required(VERSION 3.15)
project(OpenGL-Test)

# find_package(fmt REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


add_library(OpenGL-Utils STATIC)
target_compile_features(OpenGL-Utils PUBLIC cxx_std_17)
target_include_directories(OpenGL-Utils PUBLIC include)
target_link_libraries(OpenGL-Utils PUBLIC GLEW::GLEW OpenGL::OpenGL)
target_sources(OpenGL-Utils PRIVATE
    src/OpenGL/Object.cpp
    src/OpenGL/Shader.cpp
    src/OpenGL/ShaderProgram.cpp
    src/OpenGL/Buffer.cpp
    src/OpenGL/VertexArray.cpp
    src/OpenGL/Texture.cpp
)


add_library(GLFW-Utils STATIC)
target_compile_features(GLFW-Utils PUBLIC cxx_std_17)
target_include_directories(GLFW-Utils PUBLIC include)
target_link_libraries(GLFW-Utils PUBLIC glfw GLEW::GLEW)
target_sources(GLFW-Utils PRIVATE
    src/Window.cpp
    src/WindowDefaults.cpp
    src/WindowEvents.cpp
)

add_library(Assets-Utils STATIC)
target_compile_features(Assets-Utils PUBLIC cxx_std_17)
target_include_directories(Assets-Utils PUBLIC include libs/stb)
target_sources(Assets-Utils PRIVATE
    src/Image.cpp
)

add_executable(main)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE OpenGL-Utils GLFW-Utils Assets-Utils)
target_sources(main PRIVATE
    src/main.cpp 
    src/Application.cpp
    src/ApplicationLogic.cpp
)
