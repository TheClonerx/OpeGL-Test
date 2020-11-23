cmake_minimum_required(VERSION 3.15)
project(OpenGL-Test C CXX)

# find_package(fmt REQUIRED) # not yet needed
find_package(SDL2 REQUIRED)
find_package(SFML 2.5 REQUIRED COMPONENTS window)

add_subdirectory(libs/glad)

include(FetchContent)

FetchContent_Declare(glm URL https://github.com/g-truc/glm/archive/0.9.9.8.zip)
option(GLM_TEST_ENABLE "" OFF)
FetchContent_MakeAvailable(glm)

# set(BUILD_SHARED_LIBS FALSE) add_subdirectory(libs/SFML)
# unset(BUILD_SHARED_LIBS)

FetchContent_Declare(fmt URL https://github.com/fmtlib/fmt/archive/7.1.2.zip)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    spdlog URL https://github.com/gabime/spdlog/archive/v1.8.1.zip
)
set(SPDLOG_FMT_EXTERNAL TRUE)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    imgui URL https://github.com/ocornut/imgui/archive/v1.79.zip
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_PRIVATE_SOURCES
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
    )
    set(IMGUI_PUBLIC_SOURCES ${imgui_SOURCE_DIR}/imconfig.h
                             ${imgui_SOURCE_DIR}/imgui.h
    )
    add_library(imgui STATIC)
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
    target_sources(
        imgui
        PRIVATE ${IMGUI_PRIVATE_SOURCES}
        PUBLIC ${IMGUI_PUBLIC_SOURCERS}
    )
endif()

add_library(OpenGL-Utils STATIC)
target_compile_features(OpenGL-Utils PUBLIC cxx_std_17)
target_include_directories(OpenGL-Utils PUBLIC include)
target_link_libraries(OpenGL-Utils PUBLIC glad glm)
target_sources(
    OpenGL-Utils
    PRIVATE src/OpenGL/Object.cpp src/OpenGL/Shader.cpp
            src/OpenGL/ShaderProgram.cpp src/OpenGL/VertexArray.cpp
            src/OpenGL/Texture.cpp
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
