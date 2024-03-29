#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include "../parser/parser.h"
#include <sys/types.h>
#include <signal.h>
#include "errno.h"

typedef struct UiData{
    int height;
    int width;
    int margin_top;
    int margin_bottom;
    int rows_per_page;
    int num_pages;
    long long unsigned highlight;
    int page_number;
    long long unsigned int n_choices;
    char** choices;
    ListHead head;
}UiData;


int printPage(WINDOW* win ,UiData* ui);

void freeUi(UiData* ui);

void printHeader();

void printMenu(WINDOW* win, UiData* ui);

void signalHandler(WINDOW* main, UiData* ui,int signal);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);