#ifndef TETROMINO_H
#define TETROMINO_H

#include <curses.h>
#include <stdint.h>
#include "piece-data.c"

#define NUM_SQUARES 4
#define SQUARE_STR "::"

enum shape
{
    NONE = 0,
    I,
    O,
    S,
    Z,
    L,
    J,
    T,
    NUM_SHAPES
};

struct piece
{
    enum shape shape;
    int8_t rotation;
    int8_t coordinates[4][2];
};

struct piece piece_create(const enum shape shape);
void piece_rotate(struct piece* tetromino, const int8_t direction);

void piece_draw(const struct piece piece, const uint8_t start_x, const uint8_t start_y);

#endif
