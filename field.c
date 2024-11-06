#include "field.h"
#include "piece.h"
#include "tetris.h"

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


void field_new_cur_piece(struct field* field)
{
    struct piece piece = piece_create(rand() % PIECE_NUM_TYPES);
    *field->cur_piece = piece;
    field->pos_x = (field->width / 2 - PIECE_NUM_SQUARES / 2) - (piece.type == I ? 1 : 0);
    field->pos_y = piece.type == I || piece.type == O ? -1 : 0;
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
    for (int i = 0; i < field->width * field->height; i++)
    {
        int8_t cell_x = i % field->width;
        int8_t cell_y = i / field->width;
        field_set_cell(field, cell_x, cell_y, NONE_TYPE);
    }
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

        if (cell_x < 0 || cell_x >= field->width || cell_y < 0 || cell_y >= field->height)
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
    for (int i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        uint8_t cell_x = field->pos_x + field->cur_piece->coordinates[field->cur_piece->rotation][i][0];
        uint8_t cell_y = field->pos_y + field->cur_piece->coordinates[field->cur_piece->rotation][i][1];
        field_set_cell(field, cell_x, cell_y, field->cur_piece->type);
    }
    field_new_cur_piece(field);
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

bool field_rotate_cur_piece(struct field* field, const int8_t direction)
{
    uint8_t attempted_rotation = piece_get_next_rotation(field->cur_piece, direction);
    if (!field_cur_piece_collides(field, 0, 0, attempted_rotation))
    {
        piece_rotate(field->cur_piece, direction);
        return true;
    }

    if (!field_cur_piece_collides(field, 0, -1, attempted_rotation))
    {
        field_move_cur_piece(field, 0, -1, false);
        piece_rotate(field->cur_piece, direction);
        return true;
    }

    for (int i = 0; i < PIECE_NUM_COLLISION_CHECKS; i++)
    {
        int8_t check_x = field->cur_piece->collision_checks[attempted_rotation][direction][i][0];
        int8_t check_y = field->cur_piece->collision_checks[attempted_rotation][direction][i][1];

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


static void draw_grid_border(const uint8_t field_width, const uint8_t field_height, const uint8_t start_x, const uint8_t start_y)
{
    int8_t top_y = start_y;
    int8_t bot_y = start_y + field_height + 1;
    int8_t left_x = start_x;
    int8_t right_x = start_x + field_width * 2;

    set_color(BLACK);
    mvaddstr(top_y, left_x, BORDER_TOP_LEFT_STR);
    mvaddstr(bot_y, left_x, BORDER_BOT_LEFT_STR);
    for (int i = 1; i <= right_x - left_x; i++)
    {
        mvaddstr(top_y, left_x + i, BORDER_TOP_LINE_STR);
        mvaddstr(bot_y, left_x + i, BORDER_BOT_LINE_STR);
    }
    mvaddstr(top_y, right_x, BORDER_TOP_RIGHT_STR);
    mvaddstr(bot_y, right_x, BORDER_BOT_RIGHT_STR); 
    for (int i = 1; i < bot_y - top_y; i++)
    {
        mvaddstr(top_y + i, left_x, BORDER_VERT_LINE_STR);
        mvaddstr(top_y + i, right_x, BORDER_VERT_LINE_STR);
    }
}

static void draw_stats_box_border(const uint8_t box_width, const uint8_t box_height, const uint8_t start_x, const uint8_t start_y)
{
    int8_t top_y = start_y;
    int8_t bot_y = start_y + box_height + 1;
    int8_t left_x = start_x;
    int8_t right_x = start_x + box_width * 2 + 2;

    set_color(BLACK);
    mvaddstr(top_y, left_x, BOX_TOP_LEFT_STR);
    mvaddstr(bot_y, left_x, BOX_BOT_LEFT_STR);
    for (int i = 1; i <= right_x - left_x; i++)
    {
        mvaddstr(top_y, left_x + i, BOX_TOP_LINE_STR);
        mvaddstr(bot_y, left_x + i, BOX_BOT_LINE_STR);
    }
    mvaddstr(top_y, right_x, BOX_TOP_RIGHT_STR);
    mvaddstr(bot_y, right_x, BOX_BOT_RIGHT_STR); 
    for (int i = 1; i < bot_y - top_y; i++)
    {
        mvaddstr(top_y + i, left_x, BOX_VERT_LINE_STR);
        mvaddstr(top_y + i, right_x, BOX_VERT_LINE_STR);
    }
}

static void draw_grid(const struct field* field, const uint8_t start_x, const uint8_t start_y)
{
    for (int i = 0; i < field->width * field->height; i++)
    {
        int8_t cell_x = i % field->width;
        int8_t cell_y = i / field->width;
        enum piece_type cell_type = field_get_cell(field, cell_x, cell_y);
        char* cell_str = cell_type == NONE_TYPE ? GRID_EMPTY_CELL_STR : PIECE_SQUARE_STR;
        set_color(cell_type);
        mvaddstr(start_y + cell_y, start_x + cell_x * 2, cell_str);
    }
}


void field_draw(struct field* field, const uint16_t start_x, const uint16_t start_y)
{
    erase();
    draw_grid_border(field->width, field->height, start_x - 1, start_y - 1);
    draw_stats_box_border(5, 12, start_x + field->width * 2 + 2, start_y + 3);
    draw_grid(field, start_x, start_y);
    set_color(field->cur_piece->type);
    piece_draw(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y);
}
