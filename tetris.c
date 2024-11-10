#include "tetris.h"

static int get_scrw(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}

static int get_scrh(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

static bool screen_dimensions_changed(uint16_t* screen_width, uint16_t* screen_height)
{
    bool changed = false;
    uint16_t new_screen_width = get_scrw();
    uint16_t new_screen_height = get_scrh();
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
    *screen_width = get_scrw();
    *screen_height = get_scrh();
    return field_get_draw_width(field) > *screen_width || field_get_draw_height(field) > *screen_height;
}

static uint64_t time_ms(void) 
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

        if (*moves_made + moved >= TETRIS_MAX_JUGGLE_MOVES)
        {
            lock_timer->prev_time = 0;
        }
        else if (moved)
        {
            lock_timer->prev_time = time_ms();
        }
        *moves_made += moved;
    }
    else
    {
        lock_timer->running = false;
        lock_timer->prev_time = time_ms();
    }
}

static void lock_cur_piece(struct field* field, struct timer* game_clock, uint8_t* moves_made, struct queuebag* queuebag, uint32_t* cur_points)
{
    field_lock_cur_piece(field);
    field_set_cur_piece(field, queuebag_queue_pull(queuebag));
    game_clock->prev_time = time_ms();
    queuebag->can_hold = true;
    *moves_made = 0;
    *cur_points += TETRIS_POINTS_PER_PIECE;
}


static void hold_piece(struct field* field, struct queuebag* queuebag)
{
    if (!queuebag->can_hold)
    {
        return;
    }
    queuebag->can_hold = false;
    if (queuebag->held_piece_type != NONE_TYPE)
    {
        enum piece_type held_buffer = queuebag->held_piece_type;
        queuebag->held_piece_type = field->cur_piece->type;
        field_set_cur_piece(field, held_buffer);
        return;
    }
    queuebag->held_piece_type = field->cur_piece->type;
    field_set_cur_piece(field, queuebag_bag_pull(queuebag));
}

static void handle_input(struct field* field, struct timer* game_clock, struct timer* lock_timer, uint8_t* moves_made, struct queuebag* queuebag, uint32_t* cur_points)
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
            lock_cur_piece(field, game_clock, moves_made, queuebag, cur_points);
            break;
        case 'd':
            hold_piece(field, queuebag);
            break;
        case 'q':
            field_destroy(field);
            tetris_main_menu();
            return;
    }
    handle_lock_timer(field, lock_timer, moves_made, did_move);
}


static void add_points(uint32_t* cur_points, uint8_t lines_cleared, uint16_t cur_level, uint16_t cur_combo_chain)
{
    static const uint16_t points_per_line_clear[] = { 100, 300, 500, 800 };
    static const uint16_t points_per_combo_level = 50;

    *cur_points += points_per_line_clear[lines_cleared - 1] * cur_level;
    *cur_points += points_per_combo_level * cur_combo_chain;
}

static void level_up(uint16_t* cur_level, const uint8_t levels_gained, struct timer* game_clock, struct timer* lock_timer)
{
    if (levels_gained <= 0)
    {
        return;
    }
    static const uint16_t timer_times_ms[] = { 480, 410, 365, 325, 285, 250, 215, 190, 175, 145, 125, 105, 90, 75, 60, 45, 35, 25, 12, 0 };

    *cur_level += levels_gained; 
    if (*cur_level <= 20)
    {
        game_clock->trigger_time = timer_times_ms[*cur_level - 1];
    }
    else
    {
        lock_timer->trigger_time = timer_times_ms[(*cur_level - 20) - 1];
    }
}


static void main_loop(struct field* field, uint8_t starting_level)
{
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;

    struct timer game_clock = { true, 480, time_ms() };
    struct timer lock_timer = { false, 480, time_ms() };

    uint8_t moves_made = 0;

    struct queuebag* queuebag = queuebag_create();

    field_set_cur_piece(field, queuebag_queue_pull(queuebag));

    enum piece_type held_piece_type = NONE_TYPE;
    bool has_held = false;

    struct stats stats = { 0, 0, 0, 0 };
    uint64_t start_time = time_ms();

    uint16_t cur_combo_chain = 0;

    level_up(&stats.level, starting_level, &game_clock, &lock_timer);

    while (true)
    {
        if (screen_dimensions_too_large(&screen_width, &screen_height, field)) 
        {
            continue;
        }

        if (update_timer(&game_clock))
        {
            field_move_cur_piece(field, 0, 1, true);
            handle_lock_timer(field, &lock_timer, &moves_made, false);
        }
        if (update_timer(&lock_timer))
        {
            lock_cur_piece(field, &game_clock, &moves_made, queuebag, &stats.points);
        }

        handle_input(field, &game_clock, &lock_timer, &moves_made, queuebag, &stats.points);

        uint8_t lines_cleared_this_loop = field_clear_lines(field);

        if (stats.lines_cleared >= TETRIS_LINES_PER_LEVEL * stats.level)
        {
            level_up(&stats.level, 1, &game_clock, &lock_timer);
        }

        stats.lines_cleared += lines_cleared_this_loop;
        stats.time = time_ms() - start_time;

        cur_combo_chain = lines_cleared_this_loop > 0 ? cur_combo_chain + 1 : 0;
        add_points(&stats.points, lines_cleared_this_loop, stats.level, cur_combo_chain);

        uint16_t game_start_x = screen_width / 2 - (field->width * 2) / 2;
        uint16_t game_start_y = screen_height / 2 - field->height / 2;
        draw_game(field, game_start_x, game_start_y);
        draw_stats(stats, game_start_x + field->width * 2 + 2, game_start_y + 2);
        draw_next_and_held(queuebag, game_start_x - PIECE_NUM_SQUARES * 2 - 6, game_start_y + 3);
    }
}


void tetris_run(uint8_t starting_level)
{
    srand(time(NULL));
    struct field* field = field_create(10, 20);
    field_clear_grid(field);

    main_loop(field, starting_level);
}

void tetris_main_menu(void)
{
    uint8_t selection = 0;
    bool quit_selected = false;
    static const uint8_t starting_levels[] = { 0, 5, 10, 15, 20 };

    while(true)
    {
        switch (getch())
        {
            case 'h':
                selection -= selection > 0 ? 1 : 0;
                break;
            case 'l':
                selection += selection < 4 ? 1 : 0;
                break;
            case 'k':
                quit_selected = false;
                break;
            case 'j':
                quit_selected = true;
                break;
            case 'q':
                tetris_quit();
                return;
            case 'f':
            case ' ':
            case 10:
                !quit_selected ? tetris_run(starting_levels[selection]) : tetris_quit();
                return;
        }
        draw_main_menu(get_scrw(), get_scrh(), quit_selected ? 5 : selection);
    }
}
void tetris_quit(void)
{
    endwin();
    exit(0);
}

