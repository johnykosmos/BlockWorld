cmake_minimum_required(VERSION 3.16)

project(BlockWorld)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("dependencies/GraphicEngine3D")

add_executable(
    BlockWorld src/main.cpp
    src/scene/MainScene.cpp
    src/world/Chunk.cpp
    src/world/ChunkManager.cpp
)

target_include_directories(BlockWorld
    PRIVATE
    src
)

target_link_libraries(BlockWorld PRIVATE Engine)
