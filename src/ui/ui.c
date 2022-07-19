#include "ui.h"

#define MAX_ITEM_SIZE 256
#define MAX_ELEM_SIZE 32

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRL_D 	4

/*
    descr: 
    args:   
    retval: 
    author: [MZ] [NDP] 
*/
void procDataToString(char* buf ,ProcData* data){
    char* elem = (char*) malloc(sizeof(char*)*MAX_ELEM_SIZE);
    
    int offset = 0;
    int ret = sprintf(elem, "%d\t",(int)data->pid); //Check man sprintf
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
    ret = sprintf(elem, "%llu\t\t ",data->starttime); //Check man sprintf
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
    return ;
}


void printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number);

int main(){

	int c;				
    int n_choices, i, chosen=-1;
	
	/* Initialize curses */
	initscr();
	
    
    start_color();
        cbreak();
       noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
            
    //PRENDO I DATI.
    ListHead head;    
    List_init(&head);
    getAllProcData(&head);

    n_choices = head.size;
    ListItem* it = head.first;
    
    //Popoliamo choices.
    i = 0;
    char** choices = (char**) malloc(sizeof(char*)*n_choices);
    while(it && i < n_choices ){
        ProcListItem* elem = (ProcListItem*) it;
        ProcData* pd = elem->data;

        choices[i] = (char*) malloc(sizeof(char)*100); 
        
        procDataToString(choices[i], pd);    

        i++;
        it=it->next;
    }

    //PRINTO I DATI
    int height, width;
    getmaxyx(stdscr,height, width);

    WINDOW* main = newwin(height*0.9 , width , 3, 2);
    keypad(main, TRUE);

    //height -2
  
    int intervalLengt = height-5;
    int rows_per_page = height-7;

    int num_pages = n_choices/rows_per_page;
    int highlight = 1; //START WITH ONE.
    int page_number = 0;
    

    //-> page number starts from 1 to -> num_pages (index = i+rows_per_page*pageNumber)
        
    printPage(main ,choices,highlight,rows_per_page, page_number);
    wrefresh(main);
    
    box(main ,0 ,0);
    //GESTISCO INPUT
	while((c = wgetch(main)) != KEY_F(1)){
       
        switch(c){
            case KEY_DOWN:
                if(highlight == rows_per_page){
                    highlight = 1;
                    page_number++;
                }
                else{
                    if(highlight < rows_per_page)
                    ++highlight;
                } 
                break;
            case KEY_UP:
                if(highlight == 1 && page_number == 0)
                    highlight = 1;
                else if(highlight == 1 && page_number > 0 ){
                    page_number--;
                    highlight = rows_per_page;
                }
                else{
                    if(highlight > 1)
                        highlight--;
                }
                break;
            case 10: //ENTER KEY
                chosen = highlight;
                break;
            case KEY_NPAGE:
                break;
            case KEY_PPAGE:
                break;
            default:
                break;
        }
        
        printPage(main ,choices,highlight,rows_per_page , page_number);
        wrefresh(main);
        if(chosen > 0)
            break;
            wrefresh(main);
	}	

    endwin();
    return 0;
}


void printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number){
    
    if(!win) win = stdscr; 
    
    int i = 0;
    while(i < rows_per_page){
        if(highlight == i+1){
            wattron(win, A_REVERSE); 
            mvwprintw(win,i+1,2,"%s\n", choices[i+rows_per_page*page_number]);
            wattroff( win, A_REVERSE);
        }else{
            mvwprintw(win,i+1,2,"%s\n", choices[i+rows_per_page*page_number]);
        }
        i++;
    }


}