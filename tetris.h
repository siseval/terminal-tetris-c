#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <curses.h>
#include "cli.h"
#include "field.h"

#define set_color(T) attron(COLOR_PAIR(T + 1));

void tetris_run();

#endif
