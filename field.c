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


void field_randomize_current_piece(struct field* field)
{
    struct piece piece = piece_create(L);
    *field->cur_piece = piece;
}


enum piece_type field_get_cell(const struct field* field, const int8_t cell_x, const int8_t cell_y)
{
    return field->grid[cell_y * field->width + cell_x];
}

void field_set_cell(struct field* field, const int8_t cell_x, const int8_t cell_y, const enum piece_type piece_type)
{
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
}


void field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy)
{
    if (field_cur_piece_collides(field, dx, dy, field->cur_piece->rotation))
    {
        return;
    }
    field->pos_x += dx;
    field->pos_y += dy;
}

void field_rotate_cur_piece(struct field* field, const int8_t direction)
{
    piece_rotate(field->cur_piece, direction);
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
        char* cell_str = field_get_cell(field, cell_x, cell_y) == NONE_TYPE ? GRID_EMPTY_CELL_STR : PIECE_SQUARE_STR;
        mvaddstr(start_y + cell_y, start_x + cell_x * 2, cell_str);
    }
}


void field_draw(struct field* field, const uint16_t start_x, const uint16_t start_y, const bool redraw_border)
{
    erase();
    draw_grid_border(field->width, field->height, start_x - 1, start_y - 1);
    draw_stats_box_border(5, 12, start_x + field->width * 2 + 2, start_y + 3);
    draw_grid(field, start_x, start_y);
    set_color(field->cur_piece->type);
    piece_draw(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y);
}
