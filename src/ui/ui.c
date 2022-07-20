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



int printPage(WINDOW* win ,UiData* ui);

void printHeader();

void printMenu(WINDOW* win, UiData* ui);

void signalHandler(WINDOW* main, UiData* ui,int signal);

void initUI(){
    
    initscr();
	    
    start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
    
    curs_set(0); //REMOVE Blinking cursor
    return;
}


void initUiData(UiData* u){
    if(u){
        getmaxyx(stdscr,u->height, u->width);
        u->height = u->height*0.9;

        u->margin_top = 5;
        u->margin_bottom = 5;
        u->rows_per_page = u->height-(u->margin_bottom)-(u->margin_top);
        u->n_choices = 0;       //NEED TO BE SET
        u->num_pages = u->n_choices/(u->rows_per_page)+1;
        u->highlight = 1;
        u->page_number = 1;
        u->choices = NULL;      //NEED TO BE SET

    }
}

void getData(UiData* ui){
    if(ui){
        ListHead head;    
        List_init(&head);
        getAllProcData(&head);

        ListItem* it = head.first;
        
        long long unsigned int i = 0;
        char** choices = (char**) malloc(sizeof(char*)*head.size);
        
        //TODO: create update/setter function for UiData.
        ui->choices = choices;
        
        while(it && i < head.size ){
            ProcListItem* elem = (ProcListItem*) it;
            ProcData* pd = elem->data;

            choices[i] = (char*) malloc(sizeof(char)*100); 
            
            procDataToString(choices[i], pd);    

            i++;
            it=it->next;
        }

        ui->n_choices = head.size;
        ui->num_pages = ui->n_choices/(ui->rows_per_page)+1;
    }
}

//TODO: complete here
void keyDownHandler(UiData* ui, int written){
    
    if(ui){
         if((ui->page_number)-1 == (ui->num_pages)-1){
                    if(ui->highlight < written)
                        ui->highlight++;
                }else {
                    if(ui->highlight == ui->rows_per_page){
                        ui->highlight = 1;
                        ui->page_number++;
                    }
                   ui->highlight++;
                }   
    }
}

int main(){

    
    int c;				
    long long unsigned int  i;
	
    
    /* Initialize curses */
	initUI();

    
    UiData* ui = (UiData*)malloc(sizeof(UiData));
    
    initUiData(ui);
    
    getData(ui);
    
    
    WINDOW* main = newwin(ui->height ,ui->width , 0, 0);
    keypad(main, TRUE);

    //PRINT HEADER
    printHeader(main);
    printMenu(main, ui);
    //-> page number starts from 1 to -> num_pages (index = i+rows_per_page*pageNumber)
    int written = printPage(main ,ui);

    //mvwprintw(main,ui->rows_per_page+ui->margin_top+1,2,"page_number : %d , num_pages : %d rows_per_page: %llu  n_choices : %d\n",ui->page_number, ui->num_pages,ui->rows_per_page, ui->n_choices); //DEBUG
    wrefresh(main);
    //mvwprintw(main,ui->rows_per_page+ui->margin_top+1,2,"page_number : %d , num_pages : %d rows_per_page: %llu  n_choices : %d\n",ui->page_number, ui->num_pages,ui->rows_per_page, ui->n_choices); //DEBUG
        
    box(main ,0 ,0);

	while((c = wgetch(main)) != KEY_F(1)){
       
        switch(c){
            case KEY_DOWN:
                //pages indexes range [1, ... , num_pages-1,num_pages]

                if((ui->page_number) == (ui->num_pages)){
                    if(ui->highlight < written)
                        ui->highlight++;
                }else {
                    if(ui->highlight == ui->rows_per_page){
                        ui->highlight = 1;
                        ui->page_number++;
                    }else{
                        ui->highlight++;
                    }
                }   
                break;

            case KEY_UP:
                if(ui->highlight == 1 && ui->page_number == 1)
                    ui->highlight = 1;
                else if(ui->highlight == 1 && ui->page_number > 1 ){
                    ui->page_number--;
                    ui->highlight = ui->rows_per_page;
                }else{
                    if(ui->highlight > 1)
                        ui->highlight--;
                }
                break;
            case KEY_F(5): //Kill
                
                signalHandler(main,ui, SIGKILL);
                break;
            case KEY_F(6)://Terminate
                signalHandler(main,ui, SIGTERM);
                break;
            case KEY_F(7): //Suspend  kill((pid_t)pid, 19) may be done with SIGTSTP too but it could be ignored
                signalHandler(main,ui, SIGSTOP);
                break;
            case KEY_F(8): //Resume  kill((pid_t)pid, 18) [SIGCONT]
                signalHandler(main,ui, SIGCONT);
                break;
            default:
                break;
        }
        written = printPage(main, ui);
        mvwprintw(main,ui->rows_per_page+ui->margin_top+1,2,"[in Main()] written : %d , highlight : %d\n", written , ui->highlight);
        printHeader(main);
        printMenu(main,ui);
                
        wrefresh(main);
    }	

    endwin();
    return 0;
}

void signalHandler(WINDOW* main, UiData* ui,int signal) {
    
    long long unsigned idx; 

    if(ui->page_number< ui->num_pages)
        idx = ui->highlight-1+ui->rows_per_page*(ui->page_number-1);
    else
        idx = ui->highlight+(ui->rows_per_page-1)*(ui->page_number-1);
    //mvwprintw(main,ui->rows_per_page+ui->margin_top+3,2,"idx: %llu n_choices: %llu\n",idx, ui->n_choices);
        
    if(idx < ui->n_choices){
        char* pid = strtok(ui->choices[idx], "\t");
        
        if(pid){
            long long unsigned pid_num = atoi(pid);
            //mvwprintw(main,ui->rows_per_page+ui->margin_top+1,2,"pid_num to signal: %llu pid_from_choices %s\n",pid_num, pid );
            if(pid_num ){
                int ret_val = kill((pid_t)pid_num, signal);
                if(ret_val < 0){
                    mvwprintw(main,ui->rows_per_page+ui->margin_top+4,2,"%s pid: %llu failed\n", sys_siglist[signal], pid_num);    
                }
                else{
                    mvwprintw(main,ui->rows_per_page+ui->margin_top+4,2,"%s pid: %llu \n", sys_siglist[signal], pid_num);
                }
            }
            else{
                mvwprintw(main,ui->rows_per_page+ui->margin_top+4,2,"%s action didn't work for pid: %llu\n", sys_siglist[signal], pid_num);    
            }
        }
    }
    /*
    if(signal == SIGKILL ){
        getData(ui);
    }*/
    
}

void printHeader(WINDOW* win){
    if(!win) win = stdscr; 
    //TODO: replace 4 with #define "some costants"
    mvwprintw(win,4,2,"PID:\tUTIME\tSTIME\tN_THREADS\t\t\tSTART_TIME\t\tV_SIZE\tCOMM\n");
    return;
}

void printMenu(WINDOW* win,UiData* ui){
    if(ui){
        if(!win) win = stdscr;
        mvwprintw(win,ui->rows_per_page+ui->margin_top+1,2,"List of current active processes: \n");
        mvwprintw(win,ui->rows_per_page+ui->margin_top+2,2,"[F1] Exit | [F5] Kill | [F6] Terminate | [F7] Suspend | [F8] Resume\n");
    }
    return;
}




//TODO: resolve indexes of last processes.
//index = j+(page_number)*rows_per_page
int printPage(WINDOW* win ,UiData* ui){
    int written = 0;

    if(ui){

        //pages indexes range [0, ... , num_pages-1]
        //Se è fino alla pagina numero 13 ...
       
        if(!win) win = stdscr; 
        
        int written = 0;

        int j = 0;
        //pages indexes range [1, ... , num_pages]
        //controllo se non sono nella pagina finale ...
        if(ui->page_number < ui->num_pages){
            while(j < ui->rows_per_page){
                if(ui->highlight == 1+j){
                    wattron(win, A_REVERSE);
                    mvwprintw(win,j+ui->margin_top,2,"%s\n", ui->choices[j+ui->rows_per_page*(ui->page_number-1)]);
                    wattroff( win, A_REVERSE);
                }else{
                    mvwprintw(win,j+ui->margin_top,2,"%s\n", ui->choices[j+ui->rows_per_page*(ui->page_number-1)]);
                }
                j++;
            }
            written = ui->rows_per_page;
            return written;
        }else if(ui->page_number == ui->num_pages){
            
            int diff = ui->num_pages*ui->rows_per_page-ui->n_choices;
            j = 0;
            while(j < diff){
                long long unsigned int indice= j+1+(ui->rows_per_page-1)*(ui->page_number-1);
                //mvwprintw(win,ui->rows_per_page+ui->margin_top+2,2,"IN PRINTING LAST PAGE indice : %llu diff :%d highlight %llu\n",indice, diff, ui->highlight);
                if(ui->highlight == j+1){
                    wattron(win, A_REVERSE);
                    if(indice < ui->n_choices && indice >= 0)
                        mvwprintw(win,j+ui->margin_top,2,"%s\n", ui->choices[indice]);
                    wattroff( win, A_REVERSE);
                }else{
                    if(indice < ui->n_choices && indice >= 0)
                        mvwprintw(win,j+ui->margin_top,2,"%s\n", ui->choices[indice]);
                }
                j++;
            }

            written = diff;
            while (j < ui->rows_per_page){
                mvwprintw(win,j+ui->margin_top,2,"\n");
                j++;    
            }
            return written;
        }

    }
}