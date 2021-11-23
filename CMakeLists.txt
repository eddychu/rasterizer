cmake_minimum_required(VERSION "3.16")
project(BabyRaster)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET FALSE)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
find_package(OpenMP)
include_directories(${OpenMP_INCLUDE_DIRS})

set(PROJECT_SOURCES
    Main.cpp
    Mesh.cpp
    Rasterizer.cpp
    Renderer.cpp
    Texture.cpp

)


if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_custom_target(
    copy-assets ALL
    COMMENT "Copying Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

add_dependencies(${PROJECT_NAME} copy-assets)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${GLM_LIBRARIES} ${OpenMP_LIBRARIES})