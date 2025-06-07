cmake_minimum_required(VERSION 3.16)

project(BlockWorld)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("dependencies/GraphicEngine3D")

add_executable(
    BlockWorld src/main.cpp
    src/scene/MainScene.cpp
    src/world/Chunk.cpp
    src/world/ChunkManager.cpp
    src/world/BlockAtlas.cpp
    src/world/ChunkBuilder.cpp
    src/utils/LCG.cpp
)

target_include_directories(BlockWorld
    PRIVATE
    dependencies/FastNoiseLite/Cpp
    src
)

target_link_libraries(BlockWorld PRIVATE Engine)
