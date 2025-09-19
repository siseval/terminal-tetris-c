#ifndef TETRIS_H
#define TETRIS_H

#include <sys/time.h>
#include <curses.h>
#include <unistd.h>
#include <stats.h>

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
void tetris_lose(const struct stats stats, const uint8_t starting_level);
void tetris_quit(void);


#endif
