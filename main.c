#include <curses.h>
#include "tetris.h"

static void init_curses()
{
    initscr();
    noecho();
    nocbreak();
    curs_set(0);
}

int main()
{
    setlocale(LC_ALL, "");
    init_curses();

    tetris_run();

    endwin();
}
