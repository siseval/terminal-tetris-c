#include "tetris.h"
#include "field.h"
#include "piece.h"
#include <stdint.h>
#include <stdlib.h>


static bool screen_dimensions_changed(uint16_t* screen_width, uint16_t* screen_height)
{
    bool changed = false;
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

static bool screen_dimensions_too_large(uint16_t* screen_width, uint16_t* screen_height, struct field* field)
{
    *screen_width = cli_get_scrw();
    *screen_height = cli_get_scrh();
    return field_get_draw_width(field) > *screen_width || field_get_draw_height(field) > *screen_height;
}

uint64_t time_ms(void) 
{
    struct timeval timeval;
    gettimeofday(&timeval, NULL);
    return (((uint64_t)timeval.tv_sec) * 1000) + (timeval.tv_usec / 1000);
}

static bool update_timer(struct timer* timer)
{
    if (!timer->running)
    {
        timer->prev_time = time_ms();
        return false;
    }
    if (time_ms() - timer->prev_time >= timer->trigger_time)
    {
        timer->prev_time = time_ms();
        return true;
    }
    return false;
}

static void handle_lock_timer(struct field* field, struct timer* lock_timer, uint8_t* moves_made, bool moved)
{
    if (field_cur_piece_will_lock(field))
    {
        lock_timer->running = true;

        *moves_made += moved;
        if (*moves_made >= TETRIS_MAX_JUGGLE_MOVES)
        {
            lock_timer->prev_time = 0;
        }
        else if (moved)
        {
            lock_timer->prev_time = time_ms();
        }
    }
    else
    {
        lock_timer->running = false;
        lock_timer->prev_time = time_ms();
    }
}

static void lock_cur_piece(struct field* field, uint8_t* moves_made, enum piece_type bag[PIECE_NUM_TYPES], uint8_t* bag_index)
{
    field_lock_cur_piece(field);
    field_set_cur_piece(field, bag[*bag_index]);
    *bag_index += 1;
    *moves_made = 0;
}

static void shuffle_bag(enum piece_type bag[PIECE_NUM_TYPES])
{
    for (uint8_t i = 0; i < PIECE_NUM_TYPES * 2; i++)
    {
        uint8_t first_index = rand() % PIECE_NUM_TYPES;
        uint8_t second_index = rand() % PIECE_NUM_TYPES;

        enum piece_type buffer = bag[first_index];
        bag[first_index] = bag[second_index];
        bag[second_index] = buffer;
    }
}


static void handle_input(struct field* field, struct timer* game_clock, struct timer* lock_timer, uint8_t* moves_made, enum piece_type bag[PIECE_NUM_TYPES], uint8_t* bag_index)
{
    bool did_move = false;
    char input = getch();
    switch (input)
    {
        case 'h':
            did_move = field_move_cur_piece(field, -1, 0, true);
            break;
        case 'l':
            did_move = field_move_cur_piece(field, 1, 0, true);
            break;
        case 'j':
            field_move_cur_piece(field, 0, 1, true);
            game_clock->prev_time = time_ms();
            break;
        case 'a':
            did_move = field_rotate_cur_piece(field, -1);
            break;
        case 's':
            did_move = field_rotate_cur_piece(field, 1);
            break;
        case ' ':
            field_slam_cur_piece(field);
            lock_cur_piece(field, moves_made, bag, bag_index);
            break;
    }

    handle_lock_timer(field, lock_timer, moves_made, did_move);
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

static void draw_field_grid(const enum piece_type* grid, const uint8_t width, const uint8_t height, const uint8_t start_x, const uint8_t start_y)
{
    for (int i = 0; i < width * height; i++)
    {
        int8_t cell_x = i % width;
        int8_t cell_y = i / width;
        enum piece_type cell_type = grid[cell_y * width + cell_x];
        char* cell_str = cell_type == NONE_TYPE ? GRID_EMPTY_CELL_STR : PIECE_SQUARE_STR;
        if (cell_type < 0 || cell_type >= PIECE_NUM_TYPES)
        {
            int s = 0;
        }
        set_color(cell_type);
        mvaddstr(start_y + cell_y, start_x + cell_x * 2, cell_str);
    }
}

static void draw_field(struct field* field, const uint16_t start_x, const uint16_t start_y)
{
    erase();
    draw_grid_border(field->width, field->height, start_x - 1, start_y - 1);
    draw_field_grid(field->grid, field->width, field->height, start_x, start_y);

    set_color(field->cur_piece->type);
    piece_draw(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y + (field_get_lowest_height(field) - field->pos_y), PIECE_GHOST_SQUARE_STR);
    piece_draw(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y, PIECE_SQUARE_STR);
}

static void draw_piece_grid(const struct piece piece, const uint16_t start_x, const uint16_t start_y)
{
    draw_grid_border(PIECE_NUM_SQUARES, PIECE_NUM_SQUARES, start_x, start_y);
    // for (int i = 0; i < PIECE_NUM_SQUARES * PIECE_NUM_SQUARES; i++)
    // {
    //     uint8_t cell_x = i % PIECE_NUM_SQUARES;
    //     uint8_t cell_y = i / PIECE_NUM_SQUARES;
    //     set_color(NONE_TYPE);
    //     mvaddstr(1 + start_y + cell_y, 1 + start_x + cell_x * 2, GRID_EMPTY_CELL_STR);
    // }
    set_color(piece.type);
    piece_draw(&piece, 1 + start_x, 1 + start_y, PIECE_SQUARE_STR);
}


static void main_loop(struct field* field)
{
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;

    struct timer game_clock = { true, 480, time_ms() };
    struct timer lock_timer = { false, 480, time_ms() };

    uint8_t moves_made = 0;

    uint8_t bag_index = 1;
    enum piece_type bag[PIECE_NUM_TYPES];
    for (int i = 0; i < PIECE_NUM_TYPES; i++)
    {
        bag[i] = i;
    }
    shuffle_bag(bag);
    field_set_cur_piece(field, bag[0]);

    struct piece next_piece;
    struct piece held_piece;

    while (true)
    {
        if (screen_dimensions_too_large(&screen_width, &screen_height, field)) { continue; }

        if (bag_index >= PIECE_NUM_TYPES)
        {
            shuffle_bag(bag);
            bag_index = 0;
        }

        next_piece = piece_create(bag[bag_index]);

        if (update_timer(&game_clock))
        {
            field_move_cur_piece(field, 0, 1, true);
            handle_lock_timer(field, &lock_timer, &moves_made, false);
        }
        if (update_timer(&lock_timer))
        {
            lock_cur_piece(field, &moves_made, bag, &bag_index);            
        }

        handle_input(field, &game_clock, &lock_timer, &moves_made, bag, &bag_index);
        field_clear_lines(field);

        draw_field(field, screen_width / 2 - (field->width * 2) / 2, screen_height / 2 - field->height / 2);
        draw_piece_grid(next_piece, field->width * 2 + 2, screen_height / 2 - field->height / 2 + 2);
        //draw_piece_grid(next_grid, field->width * 2 + 2, screen_height / 2 - field->height / 2 + 10);
    }
}


void tetris_run()
{
    srand(time(NULL));
    struct field* field = field_create(10, 20);
    field_clear_grid(field);

    main_loop(field);
}


