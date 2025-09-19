cmake_minimum_required(VERSION 3.10)
project(tetris)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tetris
    src/main.c
    src/tetris.c
    src/queuebag.c
    src/draw.c
    src/field.c
    src/piece.c
    src/piece-data.c
    src/stats.c
)

target_include_directories(tetris
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(tetris ${CURSES_LIBRARY})
