#include "ui.h"

#define MAX_ITEM_SIZE 256
#define MAX_ELEM_SIZE 32

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRL_D 	4

char* procDataToString(ProcData* data){
    char* buf = (char*) calloc(MAX_ITEM_SIZE, sizeof(char));
    char elem[MAX_ELEM_SIZE];
    
    int offset = 0;
    int ret = sprintf(elem, "%d\t",(int)data->pid); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }   
        
    ret = sprintf(elem, "%c\t",data->state); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%lu\t",data->utime); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%lu\t",data->stime); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%ld\t",data->num_threads); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%llu\t\t",data->starttime); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%lu\t",data->vsize); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = strlen(data->comm);
        memcpy(elem, data->comm ,ret );
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    

    buf[offset] = '\0'; 

    return buf;
}

ITEM ** getItems(){

    ListHead head;    
    List_init(&head);
    getAllProcData(&head);
    int listSize = head.size;

    int counter = 0;
    ITEM** listItem = (ITEM**) malloc(sizeof(ITEM*)*listSize);

    ListItem* it = head.first;
   
    while(it){

        ProcListItem* elem = (ProcListItem*) it;
        ProcData* pd = elem->data;
        
        //printw("%s\n", procDataToString(pd));
        char* name = procDataToString(pd);
        listItem[counter] = new_item(name,name);
        //TODO: continue here.
        if(! listItem[counter]){
            printw("%s\n",strerror(errno));
        }

        counter++;
        it=it->next;
    }

    return listItem;
}


int main(int argc, char const *argv[])
{


    //INIT
    initscr(); //Magic function that initialize window and curses mode.
    cbreak(); 
//            Disable Line buffering for the terminal. In this way no need to wait for char '\n'.
//            'cbreak()' -> CTRL+C passed to the program  making signal.  
//            'raw()' -> CTRL+C passed to the program WITHOUT making signal.
            
    noecho(); //Disable immediate echoing and enable it when needed.
   	keypad(stdscr, TRUE); //It enables the reading of function keys like F1, F2, arrow keys etc
    curs_set(0);
/*    
    UTILS:
    'getch()'
    The function getch() is used to get a character from user. 
    It is equivalent to normal getchar() except that we can disable the line buffering to avoid <enter> after input. 
    
    SET TEXT PROPERTY:
    attron(A_BOLD); -> ENABLE BOLD
    printw("%c", ch); -> PRINT BOLD
    attroff(A_BOLD); -> DISABLE BOLD
    
    All these functions take y co-ordinate first and then x in their arguments.
    
    OUTPUT:
    - addch() class: Print single character with attributes
    - printw() class: Print formatted output similar to printf()
    - addstr() class: Print strings

    INPUT:
    - getch() class: Get a character
    - scanw() class: Get formatted input
    - getstr() class: Get strings

    Useful macro KEY_F()

    MENU FLOW:
        Initialize curses
        Create items using new_item(). You can specify a name and description for the items.
        Create the menu with new_menu() by specifying the items to be attached with.
        Post the menu with menu_post() and refresh the screen.
        Process the user requests with a loop and do necessary updates to menu with menu_driver.
        Unpost the menu with menu_unpost()
        Free the memory allocated to menu by free_menu()
        Free the memory allocated to the items with free_item()
        End curses 

    
    */
    int row, col;
    getmaxyx(stdscr,col,row);	

    MENU *mainList;
    WINDOW * menu_win = newwin(col*0.9 , row , (col-col*0.9)/2 ,0);
    
    box(menu_win, 0,0);
    refresh();
    wrefresh(menu_win);
    keypad(menu_win,TRUE);

   
    ITEM** items = getItems();
    mainList = new_menu(items);
    
    printw("%d\n",item_count(mainList));

    set_menu_win(mainList, menu_win);
    post_menu(mainList);
    refresh();
    //Settare la 

    int c;
    while((c = getch()) != KEY_F(1)){
      
        switch (c){
        case KEY_DOWN:
		        menu_driver(mainList, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(mainList, REQ_UP_ITEM);
				break;
        default:
            break;
        }
        
    }

    //mainList = new_menu(getItems(row, col));
    
    //post_menu(mainList);
    refresh();

    //CLEAN-UP
    getch();
    unpost_menu(mainList);
    free_menu(mainList);
    //FREE ITEMS**
    //for(int i = 0; i < )
    endwin(); //Disable curses mode

    return 0;
}



/*
char *choices[] = {
                        "Choice 1",
                        "Choice 2",
                        "Choice 3",
                        "Choice 4",
			"Choice 5",
			"Choice 6",
			"Choice 7",
			"Choice 8",
			"Choice 9",
			"Choice 10",
                        "Exit",
                        (char *)NULL,
                  };

int main(){
    
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
	
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

	    n_choices = ARRAY_SIZE(choices);
        ITEM** my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], choices[i]);

	my_menu = new_menu((ITEM **)my_items);

	   
    int row, col;
    getmaxyx(stdscr,col,row);	
        my_menu_win = newwin((int)col*0.9 , row , (col-col*0.9)/2 ,0);
        keypad(my_menu_win, TRUE);
     
	    set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, col*0.7 , row*0.5, 10, 10));
	set_menu_format(my_menu, 50 ,50);
			
	    set_menu_mark(my_menu, " * ");

	    box(my_menu_win, 0, 0);
	//print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 100);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
        
	post_menu(my_menu);
	wrefresh(my_menu_win);
	
	attron(COLOR_PAIR(2));
	mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
	attroff(COLOR_PAIR(2));
	refresh();

	while((c = wgetch(my_menu_win)) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_NPAGE:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
		}
                wrefresh(my_menu_win);
	}	

	 Unpost and free all the memory taken up
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
*/