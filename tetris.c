#include "tetris.h"
#include "field.h"


static bool screen_dimensions_changed(uint16_t* screen_width, uint16_t* screen_height)
{
    clear(); bool changed = false;
    uint16_t new_screen_width = cli_get_scrw();
    uint16_t new_screen_height = cli_get_scrh();
    if (*screen_width !=  new_screen_width || *screen_height != new_screen_height)
    {
        changed = true;
    }
    *screen_width = new_screen_width;
    *screen_height = new_screen_height;
    return changed;
}

static bool screen_dimensions_too_large(uint16_t screen_width, uint16_t screen_height, struct field* field)
{
    return field_get_draw_width(field) > screen_width || field_get_draw_height(field) > screen_height;
}

static void handle_input(struct field* field)
{
    char input = getch();
    switch (input)
    {
        case 'h':
            field_move_cur_piece(field, -1, 0); 
            break;
        case 'l':
            field_move_cur_piece(field, 1, 0);
            break;
        case 'j':
            field_move_cur_piece(field, 0, 1);
            break;
    }
}

static void main_loop(struct field* field)
{
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;

    while (true)
    {
        bool redraw_border = screen_dimensions_changed(&screen_width, &screen_height);
        if (screen_dimensions_too_large(screen_width, screen_height, field)) { continue; }
        
        field_draw(field, screen_width / 2 - (field->width * 2) / 2, screen_height / 2 - field->height / 2, redraw_border);
        handle_input(field);
    }
}

void tetris_run()
{
    srand(time(NULL));
    struct field* field = field_create(10, 20);
    struct piece piece = piece_create(J);
    //field_randomize_current_piece(field);

    main_loop(field);
}


