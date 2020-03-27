cmake_minimum_required(VERSION 3.15)
project(OpenGL-Test)

# find_package(fmt REQUIRED) # not yet needed
find_package(OpenGL REQUIRED) # use system one, there's basically no other choice

set(ONLY_LIBS ON)
set(BUILD_UTILS OFF)
add_subdirectory(libs/glew-cmake)

set(GLM_TEST_ENABLE OFF)
add_subdirectory(libs/glm)

set(SFML_STATIC_LIBRARIES TRUE)
add_subdirectory(libs/SFML)

set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
add_subdirectory(libs/imgui-sfml)

add_library(OpenGL-Utils STATIC)
target_compile_features(OpenGL-Utils PUBLIC cxx_std_17)
target_include_directories(OpenGL-Utils PUBLIC include)
target_link_libraries(OpenGL-Utils PUBLIC libglew_static OpenGL::OpenGL)
target_sources(
    OpenGL-Utils
    PRIVATE src/OpenGL/Object.cpp src/OpenGL/Shader.cpp
            src/OpenGL/ShaderProgram.cpp src/OpenGL/Buffer.cpp
            src/OpenGL/VertexArray.cpp src/OpenGL/Texture.cpp
)

# add_library(ImGui-Bindings STATIC)
# target_compile_features(ImGui-Bindings PUBLIC cxx_std_17)
# target_include_directories(ImGui-Bindings PUBLIC include)
# target_link_libraries(ImGui-Bindings PUBLIC ImGui-SFML OpenGL-Utils)
# target_sources(ImGui-Bindings PRIVATE src/ImGui/Window.cpp src/ImGui/OpenGL.cpp)

add_executable(main)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE include)
target_link_libraries(
    main PRIVATE OpenGL-Utils ImGui-SFML sfml-window 
)
target_sources(
    main PRIVATE src/main.cpp src/Application.cpp src/ApplicationLogic.cpp
)
