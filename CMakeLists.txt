cmake_minimum_required(VERSION 4.4.0)
project(nile)

set(NILE_LIB_SRC_DIR src)
set(NILE_LIB_INCLUDE_DIR include)

set(NILE_LIB_SOURCE_FILES 
    ${NILE_LIB_SRC_DIR}/glad.c
    ${NILE_LIB_SRC_DIR}/color.c
    ${NILE_LIB_SRC_DIR}/input.c
    ${NILE_LIB_SRC_DIR}/renderer.c
    ${NILE_LIB_SRC_DIR}/shader.c
    ${NILE_LIB_SRC_DIR}/stb_image.c
    ${NILE_LIB_SRC_DIR}/texture.c
    ${NILE_LIB_SRC_DIR}/window.c
)

set(GL_LIBRARY GL)
set(GLFW_LIBRARY glfw)
set(X11_LIBRARY X11)
set(LIB_MATH m)

set(NILE_LIBRARY nile)
add_library(${NILE_LIBRARY} ${NILE_LIB_SOURCE_FILES})
target_include_directories(${NILE_LIBRARY} PUBLIC ${NILE_LIB_INCLUDE_DIR})
target_link_libraries(${NILE_LIBRARY} ${GL_LIBRARY} ${GLFW_LIBRARY} ${X11_LIBRARY} ${LIB_MATH})

set(NILE_SAMPLE nile_sample)
set(NILE_SAMPLE_SOURCE_FILES ${NILE_LIB_SRC_DIR}/sample.c)
add_executable(${NILE_SAMPLE} ${NILE_SAMPLE_SOURCE_FILES})
target_include_directories(${NILE_SAMPLE} PUBLIC ${NILE_LIB_INCLUDE_DIR})
target_link_libraries(${NILE_SAMPLE} ${GL_LIBRARY} ${GLFW_LIBRARY} ${X11_LIBRARY} ${NILE_LIBRARY} ${LIB_MATH})
