#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <stdlib.h>
#include <string.h>
#include "piece.h"

#define BORDER_TOP_LEFT_STR "―"//"╭"
#define BORDER_TOP_RIGHT_STR "―"//"╮"
#define BORDER_BOT_LEFT_STR "―"//"╘"  //"╰"
#define BORDER_BOT_RIGHT_STR "―"// "╛" //"╯"
#define BORDER_VERT_LINE_STR " "//"｜" // "│"
#define BORDER_TOP_LINE_STR "―"//"─"
#define BORDER_BOT_LINE_STR "―"//"═"

#define BOX_TOP_LEFT_STR "╭"//"╭"
#define BOX_TOP_RIGHT_STR "╮"//"╮"
#define BOX_BOT_LEFT_STR "╰"//"╘"  //"╰"
#define BOX_BOT_RIGHT_STR "╯"// "╛" //"╯"
#define BOX_VERT_LINE_STR "│"//"｜" // "│"
#define BOX_TOP_LINE_STR "─"//"─"
#define BOX_BOT_LINE_STR "─"//"═"

#define GRID_EMPTY_CELL_STR "◌"


struct field
{
    uint8_t width;
    uint8_t height;

    uint8_t pos_x;
    uint8_t pos_y;

    struct piece* cur_piece;

    enum piece_type* grid;
};

struct field* field_create(const uint8_t width, const uint8_t height);
void field_destroy(struct field* field);

void field_randomize_current_piece(struct field* field);

enum piece_type field_get_cell(const struct field* field, const int8_t cell_x, const int8_t cell_y);
void field_set_cell(struct field* field, const int8_t cell_x, const int8_t cell_y, const enum piece_type piece_type);

void field_clear_grid(struct field* field);

uint8_t field_get_draw_width(const struct field* field);
uint8_t field_get_draw_height(const struct field* field);

bool field_cur_piece_collides(const struct field* field, const int8_t dx, const int8_t dy, const int8_t rotation);

void field_lock_cur_piece(struct field* field);

void field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy);
void field_rotate_cur_piece(struct field* field, const int8_t direction);

void field_draw(struct field* field, const uint16_t start_x, const uint16_t start_y, const bool redraw_border);

#endif
