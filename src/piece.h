#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <curses.h>

#include "piece-data.c"

#define PIECE_NUM_SQUARES 4
#define PIECE_NUM_ROTATIONS 4
#define PIECE_NUM_COLLISION_CHECKS 4
#define PIECE_NUM_TYPES 7


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

struct piece
{
    enum piece_type type;
    int16_t rotation;
    uint16_t coordinates[PIECE_NUM_ROTATIONS][PIECE_NUM_TYPES][2];
    int16_t collision_checks[PIECE_NUM_ROTATIONS][2][PIECE_NUM_COLLISION_CHECKS][2];
};

struct piece piece_create(const enum piece_type type);

void piece_rotate(struct piece* tetromino, const int16_t direction);
uint16_t piece_get_next_rotation(const struct piece* piece, const int16_t direction);

#endif
