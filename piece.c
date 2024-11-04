#include "piece.h"

struct piece piece_create(const enum shape shape)
{
    struct piece piece = {shape, 0};
    for (int i = 0; i < NUM_SQUARES; i++)
    {
        piece.coordinates[i][0] = piece_data_collision_coords[shape - 1][0][i][0];
        piece.coordinates[i][1] = piece_data_collision_coords[shape - 1][0][i][1];
    }
    return piece;
}

void piece_rotate(struct piece* piece, const int8_t direction)
{
    piece->rotation += direction;
    for (int i = 0; i < 4; i++)
    {
        piece->coordinates[i][0] = piece_data_collision_coords[piece->shape - 1][piece->rotation][i][0];
        piece->coordinates[i][1] = piece_data_collision_coords[piece->shape - 1][piece->rotation][i][1];
    }
}

void piece_draw(const struct piece piece, const uint8_t start_x, const uint8_t start_y)
{
    for (int i = 0; i < NUM_SQUARES; i++)
    {
        move(start_y + piece.coordinates[i][1], (start_x + piece.coordinates[i][0]) * 2);
        addstr(SQUARE_STR);
    }
}
