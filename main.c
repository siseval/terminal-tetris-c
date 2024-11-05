#include <curses.h>
#include "tetris.h"

static void init_curses()
{
    initscr();
    noecho();
    nocbreak();
    curs_set(0);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_WHITE, -1);
    init_pair(6, COLOR_BLUE, -1);
    init_pair(7, COLOR_MAGENTA, -1);
    init_pair(8, COLOR_BLACK, -1);
}

int main()
{
    setlocale(LC_ALL, "");
    init_curses();

    attron(A_BOLD);

    tetris_run();

    endwin();
}
