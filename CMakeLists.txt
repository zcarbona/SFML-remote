cmake_minimum_required(VERSION 3.20)

project(EndlessShooter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS
    Graphics
    Audio
)

add_executable(EndlessShooter
    src/main.cpp
    src/logic.cpp
    src/Player.cpp
    src/bullet.cpp
    src/creatingOBJ.cpp
)

target_include_directories(EndlessShooter PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(EndlessShooter PRIVATE
    SFML::Graphics
    SFML::Audio
)