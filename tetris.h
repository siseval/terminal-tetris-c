#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>
#include <curses.h>
#include "cli.h"
#include "field.h"

#define TETRIS_REFRESH_MS 50
#define TETRIS_MAX_JUGGLE_MOVES 15

#define set_color(T) attron(COLOR_PAIR(T + 1));

void tetris_run();
void tetris_draw(struct field* field, const uint16_t start_x, const uint16_t start_y, const enum piece_type* next_grid, const enum piece_type* held_grid);

struct timer
{
    bool running;
    uint64_t trigger_time;
    uint64_t prev_time;
};

#endif
