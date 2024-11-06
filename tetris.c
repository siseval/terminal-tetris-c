#include "tetris.h"
#include "field.h"
#include <stdint.h>


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


static void handle_input(struct field* field, struct timer* game_clock, struct timer* lock_timer, uint8_t* moves_made)
{
    char input = getch();
    switch (input)
    {
        case 'h':
            field_move_cur_piece(field, -1, 0);
            break;
        case 'l':
            field_move_cur_piece(field, 1, 0);
            handle_lock_timer(field, lock_timer, moves_made, true);
            break;
        case 'j':
            field_move_cur_piece(field, 0, 1);
            handle_lock_timer(field, lock_timer, moves_made, false);
            game_clock->prev_time = time_ms();
            break;
        case 'a':
            field_rotate_cur_piece(field, -1);
            handle_lock_timer(field, lock_timer, moves_made, true);
            break;
        case 's':
            field_rotate_cur_piece(field, 1);
            handle_lock_timer(field, lock_timer, moves_made, true);
            break;
    }
}

static void main_loop(struct field* field)
{
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;

    struct timer game_clock = { true, 480, time_ms() };
    struct timer lock_timer = { false, 480, time_ms() };

    uint8_t moves_made = 0;

    while (true)
    {
        if (screen_dimensions_too_large(&screen_width, &screen_height, field)) { continue; }

        if (update_timer(&game_clock))
        {
            field_move_cur_piece(field, 0, 1);
            handle_lock_timer(field, &lock_timer, &moves_made, false);
        }
        if (update_timer(&lock_timer))
        {
            field_lock_cur_piece(field);
        }
        
        handle_input(field, &game_clock, &lock_timer, &moves_made);
        field_draw(field, screen_width / 2 - (field->width * 2) / 2, screen_height / 2 - field->height / 2);
    }
}

void tetris_run()
{
    srand(time(NULL));
    struct field* field = field_create(10, 20);
    field_clear_grid(field);
    field_new_cur_piece(field);

    main_loop(field);
}


