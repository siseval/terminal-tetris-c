#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <curses.h>
#include "cli.h"
#include "field.h"


void tetris_run();
void tetris_main_loop(struct field* field);
void tetris_handle_input();

#endif
