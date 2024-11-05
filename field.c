#include "field.h"
#include "piece.h"
#include "tetris.h"

struct field* field_create(const uint8_t width, const uint8_t height)
{
    struct field* field = calloc(1, sizeof(struct field));

    field->width = width;
    field->height = height;

    field->cur_piece = calloc(1, sizeof(struct piece));
    field->grid = malloc(sizeof(enum piece_type) * width * height);
    memset(field->grid, NONE_TYPE, sizeof(enum piece_type) * width * height);

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
    struct piece piece = piece_create(rand() % (NUM_TYPES));
    *field->cur_piece = piece;
}


uint8_t field_get_draw_width(const struct field* field)
{
    return field->width * 2 + 2;
}
uint8_t field_get_draw_height(const struct field* field)
{
    return field->height + 2;
}


bool field_cur_piece_collides(const struct field* field, const int8_t dx, const int8_t dy)
{

    return false;
}


void field_move_cur_piece(struct field* field, const int8_t dx, const int8_t dy)
{
    
}


static void draw_border(const uint8_t field_width, const uint8_t field_height, const uint8_t start_x, const uint8_t start_y)
{
    set_color(BLACK);
    mvaddstr(start_y, start_x, "╭");
    mvaddstr(start_y + field_height + 1, start_x, "╰");
    for (int i = 1; i <= field_width * 2 + 1; i++)
    {
        mvaddstr(start_y, start_x + i, "─");
        mvaddstr(start_y + field_height + 1, start_x + i, "─");
    }
    mvaddstr(start_y, start_x + field_width * 2 + 2, "╮");
    mvaddstr(start_y + field_height + 1, start_x + field_width * 2 + 2, "╯"); 
    for (int i = 0; i < field_height; i++)
    {
        move(start_y + i + 1, start_x);
        addstr("│");
        move(start_y + i + 1, start_x + field_width * 2 + 2);
        addstr("│");
    }
}

void field_draw(const struct field* field, const uint16_t start_x, const uint16_t start_y, const bool redraw_border)
{
    if (redraw_border)
    {
        draw_border(field->width, field->height, start_x - 1, start_y - 1);
    }
    set_color(field->cur_piece->type);
    piece_draw(field->cur_piece, start_x + field->pos_x, start_y + field->pos_y);
    refresh();
}
