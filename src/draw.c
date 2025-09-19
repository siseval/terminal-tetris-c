#include <string.h>
#include <draw.h>


static void set_color(enum draw_color color)
{
    attron(COLOR_PAIR(color + 1));
    color == GRAY ? attroff(A_BOLD) : attron(A_BOLD);
}

void draw_box(const uint8_t box_width, const uint8_t box_height, const uint8_t start_x, const uint8_t start_y, const enum draw_color color, const bool draw_sides)
{
    int8_t top_y = start_y;
    int8_t bot_y = start_y + box_height + 1;
    int8_t left_x = start_x;
    int8_t right_x = start_x + box_width;

    set_color(color);
    mvaddstr(top_y, left_x, BOX_TOP_LEFT_STR);
    mvaddstr(bot_y, left_x, BOX_BOT_LEFT_STR);
    for (int i = 1; i <= right_x - left_x; i++)
    {
        mvaddstr(top_y, left_x + i, BOX_TOP_LINE_STR);
        mvaddstr(bot_y, left_x + i, BOX_BOT_LINE_STR);
    }
    mvaddstr(top_y, right_x + 1, BOX_TOP_RIGHT_STR);
    mvaddstr(bot_y, right_x + 1, BOX_BOT_RIGHT_STR); 
    if (!draw_sides)
    {
        return;
    }
    for (int i = 1; i < bot_y - top_y; i++)
    {
        mvaddstr(top_y + i, left_x, BOX_VERT_LINE_STR);
        mvaddstr(top_y + i, right_x + 1, BOX_VERT_LINE_STR);
    }
}

void draw_field_grid(const enum piece_type* grid, const uint8_t width, const uint8_t height, const uint8_t start_x, const uint8_t start_y)
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
        set_color((enum draw_color)cell_type);
        mvaddstr(start_y + cell_y, start_x + cell_x * 2, cell_str);
    }
}


static void draw_piece(const struct piece* piece, const uint8_t start_x, const uint8_t start_y, char* symbol)
{
    for (int8_t i = 0; i < PIECE_NUM_SQUARES; i++)
    {
        int8_t cell_x = start_x + piece->coordinates[piece->rotation][i][0] * 2;
        int8_t cell_y = start_y + piece->coordinates[piece->rotation][i][1];
        mvaddstr(cell_y, cell_x, symbol);
    }
}

static void draw_game_box_top(const struct field* field, const uint16_t start_x, const uint16_t start_y)
{
    mvaddstr(start_y, start_x + field->width / 2, "c-tetris");

    for (uint8_t i = 0; i < field->width * 2; i += 2)
    {
        mvaddstr(start_y - 1, start_x + i, "  ");
        mvaddstr(start_y - 2, start_x + i, "  ");
    }

}

static void draw_lose_field(const struct field* field, const uint16_t start_x, const uint16_t start_y, const uint64_t time_ms)
{
    bool close_to_losing = field_get_highest_square_height(field) < 3;
    char* lose_field_str = "• ";
    if (close_to_losing)
    {
        lose_field_str = (time_ms / 300) % 2 == 0 ? "· " : "• ";
    char* lose_field_str = "  ";
    }
    set_color(GRAY);
    attroff(A_BOLD);
    for (uint8_t i = 0; i < field->width * 2; i += 2)
    {
        mvaddstr(start_y, start_x + i, lose_field_str);
        mvaddstr(start_y + 1, start_x + i, lose_field_str);
    }
}

void draw_game(const struct field* field, const uint16_t start_x, const uint16_t start_y, const uint64_t time_ms)
{
    erase();

    draw_field_grid(field->grid, field->width, field->height, start_x, start_y);
    draw_lose_field(field, start_x, start_y - 2, time_ms);
    
    set_color((enum draw_color)field->cur_piece->type);
    draw_piece(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y + (field_get_lowest_height(field) - field->pos_y), PIECE_GHOST_SQUARE_STR);
    draw_piece(field->cur_piece, start_x + field->pos_x * 2, start_y + field->pos_y, PIECE_SQUARE_STR);

    int8_t game_box_start_y = start_y - 3;

    draw_box(field->width * 2 + 1, field->height + 2, start_x - 2, game_box_start_y, GRAY, false);
    draw_game_box_top(field, start_x, game_box_start_y);
}

static void draw_ui_piece(const enum piece_type piece_type, const uint16_t start_x, const uint16_t start_y)
{
    if (piece_type != NONE_TYPE)
    {
        uint8_t held_x = start_x + 1;
        uint8_t held_y = start_y;
        if (piece_type == I || piece_type == O)
        {
            held_x -= 1;
            held_y -= 1;
        }
        struct piece held_piece = piece_create(piece_type);
        set_color((enum draw_color)piece_type);
        draw_piece(&held_piece, held_x + 1, held_y, UI_PIECE_SQUARE_STR);
    }
}

void draw_next_and_held(const struct queuebag* queuebag, const uint16_t start_x, const uint16_t start_y)
{
    draw_ui_piece(queuebag->held_piece_type, start_x, start_y); 
    draw_box(HELD_BOX_WIDTH, HELD_BOX_HEIGHT / 2 - (queuebag->held_piece_type == I ? 1 : 0), start_x - 1, start_y - 1, GRAY, true);
    mvaddstr(start_y - 1, start_x + 3, "held");

    uint8_t running_height_sum = HELD_BOX_HEIGHT;

    for (uint8_t i = 0; i < NEXT_BOX_NUM_PIECES; i++)
    {
        draw_ui_piece(queuebag->queue[i], start_x, start_y + running_height_sum);
        running_height_sum += queuebag->queue[i] == I ? 2 : 3;
    }
    draw_box(NEXT_BOX_WIDTH, running_height_sum - 5, start_x - 1, start_y + HELD_BOX_HEIGHT - 1, GRAY, true);
    mvaddstr(start_y + HELD_BOX_HEIGHT - 1, start_x + 3, "next");
}

static void draw_stats_element(const uint16_t start_x, const uint16_t start_y, const uint8_t width, const char* stat_label, const char* stat_str, const bool centered)
{
    set_color(GRAY);
    mvaddstr(start_y + 1, start_x + 2, stat_label);
    attron(A_BOLD);
    mvaddstr(start_y + 2, start_x + !centered + width / 2 - strlen(stat_str) / 2, stat_str);
}

void draw_stats(const struct stats stats, const uint16_t start_x, const uint16_t start_y)
{
    draw_box(STATS_BOX_WIDTH, STATS_BOX_HEIGHT, start_x, start_y, GRAY, true);
    mvaddstr(start_y, start_x + 3, "stats");

    char points_str[16];
    sprintf(points_str, "%d", stats.points);

    char lines_cleared_str[16];
    sprintf(lines_cleared_str, "%d", stats.lines_cleared);

    char level_str[16];
    sprintf(level_str, "%d", stats.level);

    char time_str[16];
    stats_time_as_str(time_str, stats.time);

    draw_stats_element(start_x, start_y + 1, STATS_BOX_WIDTH, "score:", points_str, false);
    draw_stats_element(start_x, start_y + 4, STATS_BOX_WIDTH, "lines:", lines_cleared_str, false);
    draw_stats_element(start_x, start_y + 7, STATS_BOX_WIDTH, "level:", level_str, false);
    draw_stats_element(start_x, start_y + 10, STATS_BOX_WIDTH, "time:", time_str, false);
}

void draw_lose_screen(const uint16_t screen_width, const uint16_t screen_height, const struct stats stats, const uint8_t selection)
{
    erase();
    uint16_t center_pos_x = screen_width / 2;
    uint16_t center_pos_y = screen_height / 2;

    uint16_t top_box_pos_y = center_pos_y - 10;
    draw_box(9, 1, center_pos_x - 5, top_box_pos_y, GRAY, false);
    set_color(WHITE);
    mvaddstr(top_box_pos_y + 1, center_pos_x - 2, "stats");

    uint16_t main_box_pos_y = top_box_pos_y + 4;
    uint8_t main_box_width = 13;
    uint8_t main_box_height = 8;
    draw_box(main_box_width, main_box_height, center_pos_x - main_box_width / 2 - 1, main_box_pos_y, GRAY, false);

    char points_str[16];
    sprintf(points_str, "%d", stats.points);

    char lines_cleared_str[16];
    sprintf(lines_cleared_str, "%d", stats.lines_cleared);

    char level_str[16];
    sprintf(level_str, "%d", stats.level);

    char time_str[16];
    stats_time_as_str(time_str, stats.time);

    uint16_t stats_start_x = center_pos_x - 4;

    draw_stats_element(stats_start_x, main_box_pos_y, STATS_BOX_WIDTH, "score", points_str, true);
    draw_stats_element(stats_start_x, main_box_pos_y + 3, STATS_BOX_WIDTH, "lines", lines_cleared_str, true);
    draw_stats_element(stats_start_x, main_box_pos_y + 6, STATS_BOX_WIDTH, "level", level_str, true);

    uint16_t quit_button_pos_y = main_box_pos_y + main_box_height + 3;
    draw_box(11, 1, center_pos_x - 6, quit_button_pos_y, selection == 0 ? YELLOW : GRAY, true);
    set_color(selection == 0 ? YELLOW : WHITE);
    mvaddstr(quit_button_pos_y + 1, center_pos_x - 4, "main menu");

    uint16_t retry_button_pos_y = quit_button_pos_y + 3;
    draw_box(7, 1, center_pos_x - 4, retry_button_pos_y, selection == 1 ? YELLOW : GRAY, true);
    set_color(selection == 1 ? YELLOW : WHITE);
    mvaddstr(retry_button_pos_y + 1, center_pos_x - 2, "retry");

}

void draw_main_menu(const uint16_t screen_width, const uint16_t screen_height, const uint8_t selection)
{
    static const uint8_t menu_width = 38;
    static const uint8_t menu_height = 15;
    static const uint8_t button_width = 5;
    static const uint8_t button_height = 1;

    erase();
    uint16_t center_pos_x = screen_width / 2;
    uint16_t center_pos_y = screen_height / 2;

    uint16_t top_box_pos_y = screen_height / 2 - 7;
    draw_box(13, 1, screen_width / 2 - 7, top_box_pos_y, GRAY, false);
    set_color(WHITE);
    mvaddstr(top_box_pos_y + 1, center_pos_x - 4, "main-menu");

    uint16_t main_box_pos_y = screen_height / 2 - button_height - 1;
    draw_box(menu_width - 7, button_height + 2, screen_width / 2 - menu_width / 2 + 3, main_box_pos_y, GRAY, false);
    mvaddstr(main_box_pos_y, center_pos_x - 2, "level");

    char* starting_levels_str[] = { " 0", " 5", "1 0", "1 5", "2 0" };
    for (uint8_t i = 0; i < 5; i++)
    {
        enum draw_color button_color = selection == i ? YELLOW : GRAY;
        
        uint16_t button_pos_x = screen_width / 2 - menu_width / 2 + (button_width + 3) * i;
        uint16_t button_pos_y = screen_height / 2 - button_height;
        draw_box(button_width, button_height, button_pos_x, button_pos_y, button_color, true);
        set_color(selection == i ? YELLOW : WHITE);

        mvaddstr(button_pos_y + 1, button_pos_x + button_width / 2, starting_levels_str[i]);
    }

    uint16_t quit_button_pos_y = screen_height / 2 + 5;
    draw_box(3, 1, center_pos_x - 2, quit_button_pos_y, selection == 5 ? YELLOW : GRAY, true);
    set_color(selection < 5 ? WHITE : YELLOW);
    mvaddstr(quit_button_pos_y + 1, center_pos_x - 0, "Q");
}
