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
    ret = sprintf(elem, "%ld\t\t\t\t",data->num_threads); //Check man sprintf
    if(ret >= 0){
        memcpy(buf + offset, elem, ret); //append to total buffer
        memset(elem , 0 , ret);  //empty elem buffer
        offset += ret;
    }
    ret = sprintf(elem, "%llu\t\t\t ",data->starttime); //Check man sprintf
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


void printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number, int margin_top, int margin_bottom, int num_pages);

void printHeader();

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
    height = height*0.9;
    WINDOW* main = newwin(height , width , 0, 0);
    keypad(main, TRUE);

    //height -2
  
    int intervalLengt = height-5;
    int margin_top = 5;
    int margin_bottom = 5;
    int rows_per_page = height-margin_bottom-margin_top;

    int num_pages = n_choices/rows_per_page;
    int highlight = 1; //START WITH ONE.
    int page_number = 0;
    

    //PRINT HEADER
    printHeader(main);

    //-> page number starts from 1 to -> num_pages (index = i+rows_per_page*pageNumber)
    printPage(main ,choices,highlight,rows_per_page, page_number,margin_top, margin_bottom ,num_pages);
    wrefresh(main);
    
    box(main ,0 ,0);
	while((c = wgetch(main)) != KEY_F(1)){
       
        switch(c){
            case KEY_DOWN:
                if(highlight == rows_per_page){
                    if (page_number < num_pages){
                        highlight = 1;      
                        page_number++;
                    }
                }else{
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
       
        printPage(main ,choices,highlight,rows_per_page , page_number, margin_top, margin_bottom, num_pages);
        wrefresh(main);
        if(chosen > 0)
            break;
            wrefresh(main);
	}	

    endwin();
    return 0;
}

void printHeader(WINDOW* win){
    if(!win) win = stdscr; 

    mvwprintw(win,4,2,"PID:\tUTIME\tSTIME\tN_THREADS\t\t\tSTART_TIME\t\tV_SIZE\tCOMM\n");
    
    return;
}

//TODO: per dopo.
void printSysStat(){
    return;
}

void printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number, int margin_top, int margin_bottom, int num_pages){
    
    if(!win) win = stdscr; 
    
    int j = 0;
    while(j < rows_per_page){
        if(highlight == j+1){
            wattron(win, A_REVERSE);
            if(page_number == num_pages){
                mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number-1)]);
            }else{
                mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number)]);
            }
            wattroff( win, A_REVERSE);
        }else{
            if(page_number == num_pages){
                mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number-1)]);
            }else{
                mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number)]);
            }
        }
        j++;
    }

    return;
}