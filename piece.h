#ifndef TETROMINO_H
#define TETROMINO_H

#include <curses.h>
#include <stdint.h>
#include "piece-data.c"

#define PIECE_NUM_SQUARES 4
#define PIECE_NUM_ROTATIONS 4
#define PIECE_NUM_COLLISION_CHECKS 4
#define PIECE_NUM_TYPES 7

#define PIECE_SQUARE_STR "◉" //"▒▒"
#define PIECE_GHOST_SQUARE_STR "◯"

enum piece_type
{
    I = 0,
    O = 1,
    S = 2,
    Z = 3,
    L = 4,
    J = 5,
    T = 6,
    NONE_TYPE = 7
};

enum piece_color
{
    CYAN = 0,
    YELLOW = 1,
    GREEN = 2,
    RED = 3,
    MAGENTA = 4,
    BLUE = 5,
    WHITE = 6,
    BLACK = 7
};

struct piece
{
    enum piece_type type;
    int8_t rotation;
    uint8_t coordinates[PIECE_NUM_ROTATIONS][PIECE_NUM_TYPES][2];
    int8_t collision_checks[PIECE_NUM_ROTATIONS][2][PIECE_NUM_COLLISION_CHECKS][2];
};

struct piece piece_create(const enum piece_type type);

void piece_rotate(struct piece* tetromino, const int8_t direction);
uint8_t piece_get_next_rotation(const struct piece* piece, const int8_t direction);

void piece_draw(const struct piece* piece, const uint8_t start_x, const uint8_t start_y, char* symbol);

#endif
