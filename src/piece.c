#include <piece.h>

struct piece piece_create(const enum piece_type type)
{
    struct piece piece = {type, 0};
    for (uint8_t i = 0; i < PIECE_NUM_ROTATIONS; i++)
    {
        for (uint8_t j = 0; j < PIECE_NUM_SQUARES; j++)
        {
            piece.coordinates[i][j][0] = piece_data_collision_coords[type][i][j][0];
            piece.coordinates[i][j][1] = piece_data_collision_coords[type][i][j][1];
        }
    }
    for (uint8_t i = 0; i < PIECE_NUM_ROTATIONS; i++)
    {
        for (uint8_t j = 0; j < PIECE_NUM_COLLISION_CHECKS; j++)
        {
            piece.collision_checks[i][0][j][0] = piece_data_collision_checks[type][i][0][j][1];
            piece.collision_checks[i][0][j][1] = piece_data_collision_checks[type][i][0][j][0];
            piece.collision_checks[i][1][j][0] = piece_data_collision_checks[type][i][1][j][1];
            piece.collision_checks[i][1][j][1] = piece_data_collision_checks[type][i][1][j][0];
        }
    }
    return piece;
}


void piece_rotate(struct piece* piece, const int8_t direction)
{
    piece->rotation = piece_get_next_rotation(piece, direction);
}

uint8_t piece_get_next_rotation(const struct piece* piece, const int8_t direction)
{
    if (piece->rotation + direction >= PIECE_NUM_ROTATIONS)
    {
        return 0;
    }
    if (piece->rotation + direction < 0)
    {
        return PIECE_NUM_ROTATIONS - 1;
    }
    return piece->rotation + direction;
}
