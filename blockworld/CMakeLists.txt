cmake_minimum_required(VERSION 3.16)

project(Game)

add_executable(
    Game main.cpp
    src/scene/MainScene.cpp
)

target_include_directories(Game
    PRIVATE
    src
)

target_link_libraries(Game PRIVATE Engine)

