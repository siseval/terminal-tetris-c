#include "piece.h"

struct piece piece_create(const enum piece_type type)
{
    struct piece piece = {type, 0};
    for (int i = 0; i < NUM_ROTATIONS; i++)
    {
        for (int j = 0; i < NUM_SQUARES; j++)
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
}

void piece_draw(const struct piece* piece, const uint8_t start_x, const uint8_t start_y)
{
    for (int i = 0; i < NUM_SQUARES; i++)
    {
        mvaddstr(start_y + piece->coordinates[piece->rotation][i][1], start_x + piece->coordinates[piece->rotation][i][0] * 2, SQUARE_STR);
    }
}
