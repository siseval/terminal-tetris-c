#ifndef FIELD_H
#define FIELD_H

#include <stdint.h>
#include "piece.h"

struct field
{
    uint16_t width;
    uint16_t height;

    int16_t pos_x;
    int16_t pos_y;

    struct piece* cur_piece;

    enum piece_type* grid;
};

struct field* field_create(const uint16_t width, const uint16_t height);
void field_destroy(struct field* field);

void field_set_cur_piece(struct field* field, const enum piece_type piece_type);

enum piece_type field_get_cell(const struct field* field, const int16_t cell_x, const int16_t cell_y);

void field_set_cell(struct field* field, const int16_t cell_x, const int16_t cell_y, const enum piece_type piece_type);

void field_clear_grid(struct field* field);
uint16_t field_clear_lines(struct field* field);

uint16_t field_get_draw_width(const struct field* field);
uint16_t field_get_draw_height(const struct field* field);

bool field_cur_piece_collides(const struct field* field, const int16_t dx, const int16_t dy, const int16_t rotation);

void field_lock_cur_piece(struct field* field);
bool field_should_lose(struct field* field);

bool field_move_cur_piece(struct field* field, const int16_t dx, const int16_t dy, bool do_collision_check);
uint16_t field_get_lowest_height(const struct field* field);
int16_t field_get_highest_square_height(const struct field* field);
void field_soft_drop_cur_piece(struct field* field);
void field_slam_cur_piece(struct field* field);
bool field_rotate_cur_piece(struct field* field, const int16_t direction);
bool field_cur_piece_will_lock(struct field* field);

#endif
