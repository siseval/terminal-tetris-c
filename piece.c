#include "piece.h"

struct piece piece_create(const enum piece_type type)
{
    struct piece piece = {type, 0};
    for (int8_t i = 0; i < PIECE_NUM_ROTATIONS; i++)
    {
        for (int8_t j = 0; j < PIECE_NUM_SQUARES; j++)
        {
            piece.coordinates[i][j][0] = piece_data_collision_coords[type][i][j][0];
            piece.coordinates[i][j][1] = piece_data_collision_coords[type][i][j][1];
        }
    }
    return piece;
}

void piece_rotate(struct piece* piece, const int8_t direction)
{
    piece->rotation += direction;
    if (piece->rotation >= PIECE_NUM_ROTATIONS)
    {
        piece->rotation = 0;
    }
    if (piece->rotation < 0)
    {
        piece->rotation = PIECE_NUM_ROTATIONS - 1;
    }
}

void piece_draw(const struct piece* piece, const uint8_t start_x, const uint8_t start_y)
{
    for (int8_t i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        int8_t cell_x = start_x + piece->coordinates[piece->rotation][i][0] * 2;
        int8_t cell_y = start_y + piece->coordinates[piece->rotation][i][1];
        mvaddstr(cell_y, cell_x, PIECE_SQUARE_STR);
    }
}
