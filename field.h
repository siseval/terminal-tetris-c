#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <stdlib.h>
#include <string.h>
#include "piece.h"

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

uint8_t field_get_draw_width(const struct field* field);
uint8_t field_get_draw_height(const struct field* field);

void field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy);

void field_draw(const struct field* field, const uint16_t start_x, const uint16_t start_y, const bool redraw_border);

#endif
