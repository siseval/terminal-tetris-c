#include "tetris.h"


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

static void lock_cur_piece(struct field* field, uint8_t* moves_made, enum piece_type queue[QUEUE_LENGTH], enum piece_type bag[PIECE_NUM_TYPES], uint8_t* bag_index)
{
    field_lock_cur_piece(field);
    field_set_cur_piece(field, queue_pull(queue, bag, bag_index));
    *moves_made = 0;
}


static void handle_input(struct field* field, struct timer* game_clock, struct timer* lock_timer, uint8_t* moves_made, enum piece_type queue[QUEUE_LENGTH], enum piece_type bag[PIECE_NUM_TYPES], uint8_t* bag_index)
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
            lock_cur_piece(field, moves_made, queue, bag, bag_index);
            break;
    }
    handle_lock_timer(field, lock_timer, moves_made, did_move);
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
    for (uint8_t i = 0; i < PIECE_NUM_TYPES; i++)
    {
        bag[i] = i;
    }
    bag_shuffle(bag);

    enum piece_type queue[TETRIS_QUEUE_LENGTH];
    memset(queue, NONE_TYPE, sizeof(queue));
    queue_fill(queue, bag, &bag_index);
    field_set_cur_piece(field, queue_pull(queue, bag, &bag_index));

    while (true)
    {
        if (screen_dimensions_too_large(&screen_width, &screen_height, field)) { continue; }

        if (update_timer(&game_clock))
        {
            field_move_cur_piece(field, 0, 1, true);
            handle_lock_timer(field, &lock_timer, &moves_made, false);
        }
        if (update_timer(&lock_timer))
        {
            lock_cur_piece(field, &moves_made, queue, bag, &bag_index);            
        }

        handle_input(field, &game_clock, &lock_timer, &moves_made, queue, bag, &bag_index);
        field_clear_lines(field);

        draw_game(field, screen_width / 2 - (field->width * 2) / 2, screen_height / 2 - field->height / 2);
        //draw_piece_grid(next_piece, field->width * 2 + 2, screen_height / 2 - field->height / 2 + 2);
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


