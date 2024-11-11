#include "field.h"
#include "piece.h"


struct field* field_create(const uint8_t width, const uint8_t height)
{
    struct field* field = calloc(1, sizeof(struct field));

    field->width = width;
    field->height = height;

    field->cur_piece = calloc(1, sizeof(struct piece));

    field->grid = calloc(1, sizeof(enum piece_type) * width * height);

    return field;
}

void field_destroy(struct field* field)
{
    free(field->cur_piece);
    free(field->grid);
    free(field);
}


void field_set_cur_piece(struct field* field, const enum piece_type piece_type)
{
    struct piece piece = piece_create(piece_type);
    *field->cur_piece = piece;
    field->pos_x = field->width / 2 - PIECE_NUM_SQUARES / 2;
    field->pos_y = piece_type == O || piece_type == I ? -3 : -2;
}


enum piece_type field_get_cell(const struct field* field, const int8_t cell_x, const int8_t cell_y)
{
    if (cell_x < 0 || cell_x >= field->width || cell_y < 0 || cell_y >= field->height)
    {
        return NONE_TYPE;
    }
    return field->grid[cell_y * field->width + cell_x];
}

void field_set_cell(struct field* field, const int8_t cell_x, const int8_t cell_y, const enum piece_type piece_type)
{
    if (cell_x < 0 || cell_x >= field->width || cell_y < 0 || cell_y >= field->height)
    {
        return;
    }
    field->grid[cell_y * field->width + cell_x] = piece_type;
}

void field_clear_grid(struct field* field)
{
    for (uint16_t i = 0; i < field->width * field->height; i++)
    {
        int8_t cell_x = i % field->width;
        int8_t cell_y = i / field->width;
        field_set_cell(field, cell_x, cell_y, NONE_TYPE);
    }
}


static bool line_should_clear(const struct field* field, const uint8_t line_index)
{
    bool broken = false;
    for (int8_t i = 0; i < field->width; i++)
    {
        if (field_get_cell(field, i, line_index) == NONE_TYPE)
        {
            broken = true;
        }
    }
    return !broken;
}

static void clear_line(struct field* field, const uint8_t line_index)
{
    if (line_index == 0)
    {
        for (uint8_t i = 0; i < field->width; i++)
        {
            field_set_cell(field, i, line_index, NONE_TYPE);
        }
    }
    for (uint8_t i = line_index; i > 0; i--)
    {
        for (uint8_t j = 0; j < field->width; j++)
        {
            field_set_cell(field, j, i, field_get_cell(field, j, i - 1));
        }
    }
}

uint8_t field_clear_lines(struct field* field)
{
    uint8_t lines_cleared = 0;
    for (uint8_t i = 0; i < field->height; i++)
    {
        if (line_should_clear(field, i))
        {
            clear_line(field, i);
            lines_cleared += 1;
        }
    }
    return lines_cleared;
}


uint8_t field_get_draw_width(const struct field* field)
{
    return field->width * 2 + 2;
}

uint8_t field_get_draw_height(const struct field* field)
{
    return field->height + 2;
}


bool field_cur_piece_collides(const struct field* field, const int8_t dx, const int8_t dy, const int8_t rotation)
{
    for (int i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        int cell_x = field->pos_x + field->cur_piece->coordinates[rotation][i][0] + dx;
        int cell_y = field->pos_y + field->cur_piece->coordinates[rotation][i][1] + dy;

        if (cell_x < 0 || cell_x >= field->width || cell_y >= field->height)
        {
            return true;
        }
        if (field_get_cell(field, cell_x, cell_y) != NONE_TYPE)
        {
            return true;
        }
    }
    return false;
}


void field_lock_cur_piece(struct field* field)
{
    for (uint8_t i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        int8_t cell_x = field->pos_x + field->cur_piece->coordinates[field->cur_piece->rotation][i][0];
        int8_t cell_y = field->pos_y + field->cur_piece->coordinates[field->cur_piece->rotation][i][1];
        field_set_cell(field, cell_x, cell_y, field->cur_piece->type);
    }
}
bool field_should_lose(struct field* field)
{
    for (int8_t i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        int8_t cell_y = field->pos_y + field->cur_piece->coordinates[field->cur_piece->rotation][i][1];
        if (cell_y < 0)
        {
            return true;
        }
    }
    return false;
}


bool field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy, bool do_collision_check)
{
    if (do_collision_check && field_cur_piece_collides(field, dx, dy, field->cur_piece->rotation))
    {
        return false;
    }
    field->pos_x += dx;
    field->pos_y += dy;
    return true;
}

uint8_t field_get_lowest_height(const struct field* field)
{
    for (int8_t i = field->pos_y + 1; i < field->height; i++)
    {
        if (field_cur_piece_collides(field, 0, i - field->pos_y, field->cur_piece->rotation))
        {
            return i - 1;
        }
    }
    return 0;
}

int8_t field_get_highest_square_height(const struct field* field)
{
    for (int16_t i = 0; i < field->width * field->height; i++)
    {
        if (field_get_cell(field, i % field->width, i / field->width) != NONE_TYPE)
        {
            return i / field->width;
        }
    }
    return field->height;
}

void field_slam_cur_piece(struct field* field)
{
    field_move_cur_piece(field, 0, field_get_lowest_height(field) - field->pos_y, false);
    field_lock_cur_piece(field);
}

void field_soft_drop_cur_piece(struct field* field)
{
    field_move_cur_piece(field, 0, field_get_lowest_height(field) - field->pos_y, false);
}

bool field_rotate_cur_piece(struct field* field, const int8_t direction)
{
    uint8_t attempted_rotation = piece_get_next_rotation(field->cur_piece, direction);
    if (!field_cur_piece_collides(field, 0, 0, attempted_rotation))
    {
        piece_rotate(field->cur_piece, direction);
        return true;
    }

    for (int i = 0; i < PIECE_NUM_COLLISION_CHECKS; i++)
    {
        int8_t check_x = field->cur_piece->collision_checks[field->cur_piece->rotation][direction == 1][i][0];
        int8_t check_y = field->cur_piece->collision_checks[field->cur_piece->rotation][direction == 1][i][1];

        if (!field_cur_piece_collides(field, check_x, check_y, attempted_rotation))
        {
            field_move_cur_piece(field, check_x, check_y, false);
            piece_rotate(field->cur_piece, direction);
            return true;
        }
    }

    return false;
}


bool field_cur_piece_will_lock(struct field* field)
{
    return field_cur_piece_collides(field, 0, 1, field->cur_piece->rotation);
}
