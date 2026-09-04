#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include <wchar.h>

#include "queuebag.h"
#include "stats.h"
#include "field.h"

#define BOX_TOP_LEFT_STR L"╭"
#define BOX_TOP_RIGHT_STR L"╮"
#define BOX_BOT_LEFT_STR L"╰"
#define BOX_BOT_RIGHT_STR L"╯"
#define BOX_VERT_LINE_STR L"│"
#define BOX_TOP_LINE_STR L"─"
#define BOX_BOT_LINE_STR L"─"

#define PIECE_SQUARE_STR L"▨"
#define PIECE_GHOST_SQUARE_STR L"▢"
#define UI_PIECE_SQUARE_STR L"▨"

#define GRID_EMPTY_CELL_STR L"·"

#define STATS_BOX_WIDTH 9
#define STATS_BOX_HEIGHT 13

#define HELD_BOX_WIDTH 9
#define HELD_BOX_HEIGHT 4

#define NEXT_BOX_WIDTH 9
#define NEXT_BOX_HEIGHT 8
#define NEXT_BOX_NUM_PIECES 3

 
enum draw_color
{
    CYAN = 0,
    YELLOW = 1,
    GREEN = 2,
    RED = 3,
    MAGENTA = 4,
    BLUE = 5,
    WHITE = 6,
    GRAY = 7
};

void draw_box(const int box_width, const int box_height, const int start_x, const int start_y, const enum draw_color color, const bool draw_sides);

void draw_game(const struct field* field, const int start_x, const int start_y, const uint64_t time_ms);

void draw_main_menu(const uint16_t screen_width, const uint16_t screen_height, const uint8_t selection);
void draw_lose_screen(const uint16_t screen_width, const uint16_t screen_height, const struct stats stats, const uint8_t selection);

void draw_next_and_held(const struct queuebag* queuebag, const uint16_t start_x, const uint16_t start_y);
void draw_stats(const struct stats stats, const uint16_t start_x, const uint16_t start_y);


#endif
