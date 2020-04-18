cmake_minimum_required(VERSION 3.15)
project(OpenGL-Test)

# find_package(fmt REQUIRED) # not yet needed
find_package(OpenGL REQUIRED) # use system one, there's basically no other
                              # choice

set(ONLY_LIBS ON)
set(BUILD_UTILS OFF)
add_subdirectory(libs/glew-cmake)

set(GLM_TEST_ENABLE OFF)
add_subdirectory(libs/glm)

set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(libs/SFML)
unset(BUILD_SHARED_LIBS)

add_subdirectory(libs/fmt)

set(SPDLOG_FMT_EXTERNAL TRUE)
add_subdirectory(libs/spdlog)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
set(IMGUI_PRIVATE_SOURCES
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
)
set(IMGUI_PUBLIC_SOURCES ${IMGUI_DIR}/imconfig.h ${IMGUI_DIR}/imgui.h)

add_library(OpenGL-Utils STATIC)
target_compile_features(OpenGL-Utils PUBLIC cxx_std_17)
target_include_directories(OpenGL-Utils PUBLIC include)
target_link_libraries(
    OpenGL-Utils PUBLIC libglew_static OpenGL::OpenGL glm::glm
)
target_sources(
    OpenGL-Utils
    PRIVATE src/OpenGL/Object.cpp src/OpenGL/Shader.cpp
            src/OpenGL/ShaderProgram.cpp src/OpenGL/Buffer.cpp
            src/OpenGL/VertexArray.cpp src/OpenGL/Texture.cpp
)

add_library(imgui STATIC)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_sources(
    imgui
    PRIVATE ${IMGUI_PRIVATE_SOURCES}
    PUBLIC ${IMGUI_PUBLIC_SOURCERS}
)

add_library(ImGui-Bindings STATIC)
target_compile_features(ImGui-Bindings PUBLIC cxx_std_17)
target_include_directories(ImGui-Bindings PUBLIC include)
target_link_libraries(
    ImGui-Bindings
    PUBLIC imgui sfml-graphics
    PRIVATE OpenGL-Utils spdlog
)
target_sources(
    ImGui-Bindings
    PRIVATE src/ImGui/SFML.cpp src/ImGui/OpenGL.cpp
    PUBLIC include/ImGui.hpp
)

add_library(Utilities STATIC)
target_compile_features(Utilities PUBLIC cxx_std_17)
target_include_directories(Utilities PUBLIC include)
target_link_libraries(Utilities PUBLIC spdlog glm)
target_sources(
    Utilities
    PRIVATE src/Utilities/ParseINI.cpp
            # src/Utilities/ParseOBJ.cpp
    PUBLIC include/Utilities.hpp
)

add_executable(main)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE include)
target_link_libraries(
    main PRIVATE OpenGL-Utils ImGui-Bindings sfml-window Utilities
)
target_sources(
    main PRIVATE src/main.cpp src/Application.cpp src/ApplicationLogic.cpp
)
