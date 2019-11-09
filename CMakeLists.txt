cmake_minimum_required(VERSION 3.15)
project(OpenGL-Test)

find_package(fmt REQUIRED)
find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)


add_library(OpenGL-Utils STATIC)
target_compile_features(OpenGL-Utils PUBLIC cxx_std_17)
target_include_directories(OpenGL-Utils PUBLIC include)
target_link_libraries(OpenGL-Utils PUBLIC GLEW::GLEW OpenGL::OpenGL glm)
target_sources(OpenGL-Utils PRIVATE
    src/OpenGL/Object.cpp
    src/OpenGL/Shader.cpp
    src/OpenGL/ShaderProgram.cpp
    src/OpenGL/Buffer.cpp
    src/OpenGL/VertexArray.cpp
    src/OpenGL/Texture.cpp
)


add_executable(main)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE fmt::fmt sfml-window sfml-graphics OpenGL-Utils ImGui-SFML::ImGui-SFML)
target_sources(main PRIVATE
    src/main.cpp 
    src/Application.cpp
    src/ApplicationLogic.cpp
)
