add_library(glad STATIC)
target_sources(
    glad
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/glad/glad.h
           ${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/glad.c
)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/)
if(UNIX)
target_link_libraries(glad PRIVATE dl)
endif()