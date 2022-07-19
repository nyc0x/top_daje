#include "ui.h"

#define MAX_ITEM_SIZE 256
#define MAX_ELEM_SIZE 32



char* procDataToString(ProcData* data){
    char* buf =(char*) calloc(MAX_ITEM_SIZE, sizeof(char));
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

ITEM ** getItems(int rows , int cols){

    ListHead head;    
    List_init(&head);
    getAllProcData(&head);
    int listSize = head.size;

    ITEM** listItem = (ITEM**) malloc(sizeof(ITEM*)*listSize);

    ListItem* it = head.first;
    while(it){
        ProcListItem* elem = (ProcListItem*) it;
        ProcData* pd = elem->data;

        //printw("%s\n",procDataToString(pd));
        //printw("%d\t%s\t%c\t%lu\t%lu\t%ld\t%llu\t%lu\n",(int)pd->pid, pd->comm, pd->state, pd->utime, pd->stime, pd->num_threads,pd->starttime, pd->vsize);
        
        it=it->next;
    }

    return NULL;
}


int main(int argc, char const *argv[])
{


    //INIT
    initscr(); //Magic function that initialize window and curses mode.
    cbreak(); /*
            Disable Line buffering for the terminal. In this way no need to wait for char '\n'.
            'cbreak()' -> CTRL+C passed to the program  making signal.  
            'raw()' -> CTRL+C passed to the program WITHOUT making signal.
            */
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
    wgetch(menu_win);

    int highlight = 0;
    int choice;

    char* choices[3] = {"Entry 1","Entry 2","Entry3"};

    while(1){
        for (int i = 0; i < 3; i++){
            if(i == highlight)
                wattr_on(menu_win , A_REVERSE);
            mvwprintw(menu_win,i+1 , 1, choices[i]);
            wattr_off(menu_win , A_REVERSE);
            
        }
        
    }

    //mainList = new_menu(getItems(row, col));
    
    //post_menu(mainList);
    //refresh();

    //CLEAN-UP
    getch();
    endwin(); //Disable curses mode

    return 0;
}
