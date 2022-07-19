#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include "../parser/parser.h"
#include <sys/types.h>
#include <signal.h>


void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);