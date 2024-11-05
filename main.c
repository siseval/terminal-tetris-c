#include <curses.h>
#include "tetris.h"

static void init_curses()
{
    initscr();
    noecho();
    nocbreak();
    curs_set(0);
    timeout(REFRESH_MS);

    start_color();
    use_default_colors();

    init_pair(CYAN + 1, COLOR_CYAN, -1);
    init_pair(YELLOW + 1, COLOR_YELLOW, -1);
    init_pair(GREEN + 1, COLOR_GREEN, -1);
    init_pair(RED + 1, COLOR_RED, -1);
    init_pair(BLUE + 1, COLOR_BLUE, -1);
    init_pair(MAGENTA + 1, COLOR_MAGENTA, -1);
    init_pair(WHITE + 1, COLOR_WHITE, -1);
    init_pair(BLACK + 1, COLOR_BLACK, -1);
}

int main()
{
    setlocale(LC_ALL, "");
    init_curses();

    attron(A_BOLD);

    tetris_run();

    endwin();
}
