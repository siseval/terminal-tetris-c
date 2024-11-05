#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>
#include <curses.h>
#include "cli.h"
#include "field.h"

#define REFRESH_MS 50

#define set_color(T) attron(COLOR_PAIR(T + 1));

void tetris_run();

struct timer
{
    uint64_t trigger_time;
    uint64_t prev_time;
};

#endif
