#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <stdlib.h>
#include <string.h>
#include "piece.h"


struct field
{
    uint8_t width;
    uint8_t height;

    int8_t pos_x;
    int8_t pos_y;

    struct piece* cur_piece;

    enum piece_type* grid;
};

struct field* field_create(const uint8_t width, const uint8_t height);
void field_destroy(struct field* field);

void field_set_cur_piece(struct field* field, const enum piece_type piece_type);

enum piece_type field_get_cell(const struct field* field, const int8_t cell_x, const int8_t cell_y);

void field_set_cell(struct field* field, const int8_t cell_x, const int8_t cell_y, const enum piece_type piece_type);

void field_clear_grid(struct field* field);
void field_clear_lines(struct field* field);

uint8_t field_get_draw_width(const struct field* field);
uint8_t field_get_draw_height(const struct field* field);

bool field_cur_piece_collides(const struct field* field, const int8_t dx, const int8_t dy, const int8_t rotation);

void field_lock_cur_piece(struct field* field);

bool field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy, bool do_collision_check);
uint8_t field_get_lowest_height(const struct field* field);
void field_slam_cur_piece(struct field* field);
bool field_rotate_cur_piece(struct field* field, const int8_t direction);
bool field_cur_piece_will_lock(struct field* field);

#endif
