#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>
#include <curses.h>
#include "field.h"
#include "queuebag.h"
#include "stats.h"
#include "menu.h"
#include "draw.h"

#define TETRIS_REFRESH_MS 5
#define TETRIS_MAX_JUGGLE_MOVES 15
#define TETRIS_QUEUE_LENGTH 5
#define TETRIS_LINES_PER_LEVEL 10
#define TETRIS_POINTS_PER_PIECE 1


struct timer
{
    bool running;
    uint64_t trigger_time;
    uint64_t prev_time;
};

void tetris_run(uint8_t starting_level);
void tetris_main_menu(void);
void tetris_lose(const struct stats stats);
void tetris_quit(void);


#endif
