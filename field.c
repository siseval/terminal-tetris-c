#include "field.h"

struct field* field_create(const uint8_t width, const uint8_t height)
{
    struct field* field = calloc(1, sizeof(struct field));

    field->width = width;
    field->height = height;

    field->cur_piece = calloc(1, sizeof(struct piece));
    field->grid = calloc(1, sizeof(enum shape) * width * height);

    return field;
}


void field_randomize_current_piece(struct field* field)
{
    struct piece piece = piece_create(1 + rand() % (NUM_SHAPES - 1));
    *field->cur_piece = piece;
}


uint8_t field_get_draw_width(struct field* field)
{
    return field->width * 2 + 2;
}
uint8_t field_get_draw_height(struct field* field)
{
    return field->height + 2;
}

static void draw_border(const uint8_t field_width, const uint8_t field_height, const uint8_t start_x, const uint8_t start_y)
{
    char top_bar[field_width * 2 + 3];
    char bot_bar[field_width * 2 + 3];

    memset(top_bar, '\0', sizeof(top_bar));
    memset(bot_bar, '\0', sizeof(bot_bar));

    strcat(top_bar, "╭");
    strcat(bot_bar, "╰");
    for (int i = 0; i < field_width * 2; i++)
    {
        strcat(top_bar, "─");
        strcat(bot_bar, "─");
    }
    strcat(top_bar, "╮\0");
    strcat(bot_bar, "╯\0");

    move(start_y, start_x);
    addstr(top_bar);
    move(start_y + field_height + 1, start_x);
    addstr(bot_bar);

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
    //piece_draw(*field->cur_piece, start_x + field->pos_x, start_x + field->pos_y);
    refresh();
}
