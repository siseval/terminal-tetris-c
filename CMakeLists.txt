cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)
project(tetris)
add_executable(tetris
  main.c
  tetris.c
  queuebag.c
  draw.c
  field.c
  piece.c
  piece-data.c
  stats.c
)

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(tetris ${CURSES_LIBRARY})
