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



int printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number, int margin_top, int margin_bottom, int num_pages, long long unsigned int n_choices);

void printHeader();

void printMenu(WINDOW* win, int rows_per_page, int margin_top);

void signalHandler(WINDOW* main,
                   long long unsigned idx, 
                   char* pid, 
                   long long unsigned pid_num, 
                   int ret_val, 
                   long long unsigned highlight,
                   int page_number,
                   int rows_per_page,
                   int margin_top,
                   long long unsigned n_choices,
                   char** choices,
                   int signal);



int main(){

	int c;				
    long long unsigned int n_choices, i;
	
	/* Initialize curses */
	initscr();
	    
    start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
    
    curs_set(0); //REMOVE Blinking cursor

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
  
    int margin_top = 5;
    int margin_bottom = 5;
    int rows_per_page = height-margin_bottom-margin_top;

    //Pages are 0-indexed
    int num_pages = n_choices/rows_per_page+1; //TODO: CHECK HERE
    long long unsigned highlight = 1; //START WITH ONE.
    int page_number = 0;
    

    //PRINT HEADER
    printHeader(main);
    printMenu(main, rows_per_page, margin_top);
    //-> page number starts from 1 to -> num_pages (index = i+rows_per_page*pageNumber)
    int written = printPage(main ,choices,highlight,rows_per_page, page_number,margin_top, margin_bottom ,num_pages, n_choices);
    wrefresh(main);
    
    long long unsigned idx;
    char* pid;
    long long unsigned pid_num;
    int ret_val;

    box(main ,0 ,0);

	while((c = wgetch(main)) != KEY_F(1)){
       
        switch(c){
            case KEY_DOWN:
                //pages indexes range [0, ... , num_pages-2,num_pages-1]
                
                if(page_number-1 == num_pages-1){
                    if(highlight < written)
                        highlight++;
                }else {
                    if(highlight == rows_per_page){
                        highlight = 1;
                        page_number++;
                    }
                    highlight++;
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
                //TODO: add listener to Npage e PPage
            case KEY_NPAGE:
                break;
            case KEY_PPAGE:
                break;
            case KEY_F(5): //Kill
                signalHandler(main, idx, pid, pid_num, ret_val, highlight, page_number, rows_per_page, margin_top, n_choices, choices, SIGKILL);
                break;
            case KEY_F(6)://Terminate 
                signalHandler(main, idx, pid, pid_num, ret_val, highlight, page_number, rows_per_page, margin_top, n_choices, choices, SIGTERM);
                break;
            case KEY_F(7): //Suspend  kill((pid_t)pid, 19) may be done with SIGTSTP too but it could be ignored
                signalHandler(main, idx, pid, pid_num, ret_val, highlight, page_number, rows_per_page, margin_top, n_choices, choices, SIGSTOP);
                break;
            case KEY_F(8): //Resume  kill((pid_t)pid, 18) [SIGCONT]
                signalHandler(main, idx, pid, pid_num, ret_val, highlight, page_number, rows_per_page, margin_top, n_choices, choices, SIGCONT);
                break;
            default:
                break;
        }
        //DEBUG mvwprintw(main,rows_per_page+margin_top+1,2,"page_number : %d , num_pages : %d rows_per_page: %llu  n_choices : %d\n",page_number, num_pages,rows_per_page, n_choices); //DEBUG
        written = printPage(main, choices,highlight, rows_per_page, page_number, margin_top, margin_bottom, num_pages, n_choices);
        //mvwprintw(main,rows_per_page+margin_top+3,2,"page_number : %d , rows_per_page : %d highlight : %d\n",page_number, rows_per_page, highlight); //DEBUG
        printHeader(main);
        printMenu(main, rows_per_page, margin_top);
                
        wrefresh(main);
    }	

    endwin();
    return 0;
}

void signalHandler(WINDOW* main,
                   long long unsigned idx, 
                   char* pid, 
                   long long unsigned pid_num, 
                   int ret_val, 
                   long long unsigned highlight,
                   int page_number,
                   int rows_per_page,
                   int margin_top,
                   long long unsigned n_choices,
                   char** choices,
                   int signal) {
    
                   idx = highlight-1+(page_number-1)*rows_per_page;
                   mvwprintw(main,rows_per_page+margin_top+3,2,"idx: %llu n_choices: %llu\n",idx, n_choices);
                       
                   if(idx < n_choices){
                       pid = strtok(choices[idx], "\t");
                       
                       if(pid){
                           //mvwprintw(main,rows_per_page+margin_top+4,2,"pid before atoi: %llu\n",pid_num);
                           pid_num = atoi(pid);
                           if(pid_num ){
                               ret_val = kill((pid_t)pid_num, signal);
                               if(ret_val < 0){
                                   mvwprintw(main,rows_per_page+margin_top+4,2,"%s failed for pid: %llu\n", sys_siglist[signal], pid_num);    
                               }
                               else{
                                   mvwprintw(main,rows_per_page+margin_top+4,2,"%s done for pid: %llu\n", sys_siglist[signal], pid_num);
                               }
                           }
                           else{
                               mvwprintw(main,rows_per_page+margin_top+4,2,"%s action didn't work for pid: %llu\n", sys_siglist[signal], pid_num);    
                           }
                           
                       }
                   }
}

void printHeader(WINDOW* win){
    if(!win) win = stdscr; 

    //TODO: replace 4 with #define "some costants"
    mvwprintw(win,4,2,"PID:\tUTIME\tSTIME\tN_THREADS\t\t\tSTART_TIME\t\tV_SIZE\tCOMM\n");
    
    return;
}

void printMenu(WINDOW* win, int rows_per_page, int margin_top){
    if(!win) win = stdscr;

    mvwprintw(win,rows_per_page+margin_top+1,2,"List of current active processes: \n");
    mvwprintw(win,rows_per_page+margin_top+2,2,"[F1] <-> Exit | [F5]<-> Kill | [F6] <--> Pause | [F7] Suspend | [F8] Resume\n");

    return;
}

//TODO: per dopo.
void printSysStat(){
    return;
}

//index = j+(page_number)*rows_per_page
int printPage(WINDOW* win , char** choices, int highlight, int rows_per_page , int page_number, int margin_top, int margin_bottom, int num_pages,long long unsigned int n_choices){
    
    if(!win) win = stdscr; 
    
    int written = 0;

    int j = 0;
    //pages indexes range [0, ... , num_pages-1]
    //Se è fino alla pagina numero 13 ...
    if(page_number-1 < num_pages-1){
        while(j < rows_per_page){
            if(highlight == j+1){
                wattron(win, A_REVERSE);
                if(page_number == 0)
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number)]);
                else
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number-1)]);
                wattroff( win, A_REVERSE);
            }else{
                if(page_number == 0)
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number)]);
                else
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page*(page_number-1)]);
            }
            j++;
        }
        written = rows_per_page;
    }else if(page_number == num_pages){
        int diff = num_pages*rows_per_page-n_choices-1;
        j = 0;
        int count = 1;
        while(j < diff){
            if(highlight == j+1){
                wattron(win, A_REVERSE);
                if(j+rows_per_page-1+rows_per_page*(num_pages-2)< n_choices){
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page-1+rows_per_page*(num_pages-2)]);
                    written++;
                }else
                    mvwprintw(win,j+margin_top,2,"\n");
                wattroff( win, A_REVERSE);
            }else{
                if(j+rows_per_page-1+rows_per_page*(num_pages-2) < n_choices){
                    mvwprintw(win,j+margin_top,2,"%s\n", choices[j+rows_per_page-1+rows_per_page*(num_pages-2)]);
                    written++;
                }else
                    mvwprintw(win,j+margin_top,2,"\n");
            }
            count++;
            j++;
        }
        while (j < rows_per_page){
            mvwprintw(win,j+margin_top,2,"\n");
            j++;    
        }
        
    }
    
    return written;
}