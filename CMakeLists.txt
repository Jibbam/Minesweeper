# CMakeList.txt : CMake project for Minesweeper_crossplatform, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(Minesweeper_crossplatform VERSION 1.0.0)

# Add source to this project's executable.
add_executable (Minesweeper_crossplatform
    "main.cpp"
    "include/Globals.h"
    "include/Engine/Button.h"
    "include/Engine/Rectangle.h"
    "include/Engine/Image.h"
    "include/Engine/Text.h"
    "include/Engine/Window.h"
    "include/Engine/Random.h"
    "include/Minesweeper/Grid.h"
    "include/Minesweeper/UI.h"
    "include/Minesweeper/MinesweeperCell.h"
    "src/MinesweeperCell.cpp"
    "include/Minesweeper/NewGameButton.h"
    "include/Minesweeper/FlagCounter.h"
)

target_include_directories(
  Minesweeper_crossplatform PUBLIC 
  ${PROJECT_SOURCE_DIR}
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/include/Engine"
  "${PROJECT_SOURCE_DIR}/include/Minesweeper"
)

add_subdirectory("external/SDL-release-2.30.6")
add_subdirectory("external/SDL_image-release-2.8.2")
add_subdirectory("external/SDL_ttf-release-2.22.0")
target_link_libraries(Minesweeper_crossplatform PRIVATE
    SDL2
    SDL2_image
    SDL2_ttf
)

if (WIN32)
  target_link_libraries(
    Minesweeper_crossplatform PRIVATE SDL2main
  )
endif()

set(AssetDirectory "${PROJECT_SOURCE_DIR}/assets")

add_custom_command(
  TARGET Minesweeper_crossplatform POST_BUILD COMMAND
  ${CMAKE_COMMAND} -E copy_if_different 
    "$<TARGET_FILE:SDL2>"
    "$<TARGET_FILE:SDL2_image>" 
    "$<TARGET_FILE:SDL2_ttf>"
    "${AssetDirectory}/Bomb.png"
    "${AssetDirectory}/Rubik-SemiBold.ttf"
    "${AssetDirectory}/flag-48.png"
    $<TARGET_FILE_DIR:Minesweeper_crossplatform>
  VERBATIM 
)

