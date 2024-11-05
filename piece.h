#ifndef TETROMINO_H
#define TETROMINO_H

#include <curses.h>
#include <stdint.h>
#include "piece-data.c"

#define NUM_SQUARES 4
#define NUM_ROTATIONS 4
#define NUM_TYPES 7
#define SQUARE_STR "::"

enum piece_type
{
    I = 0,
    O = 1,
    S = 2,
    Z = 3,
    L = 4,
    J = 5,
    T = 6,
    NONE_TYPE
};

enum piece_color
{
    CYAN = 0,
    YELLOW = 1,
    GREEN = 2,
    RED = 3,
    BLUE = 4,
    MAGENTA = 5,
    WHITE = 6,
    BLACK = 7
};

struct piece
{
    enum piece_type type;
    int8_t rotation;
    int8_t coordinates[4][4][2];
};

struct piece piece_create(const enum piece_type type);
void piece_rotate(struct piece* tetromino, const int8_t direction);

void piece_draw(const struct piece* piece, const uint8_t start_x, const uint8_t start_y);

#endif
