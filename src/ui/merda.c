#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "ui.h"

#define MAX_ITEM_SIZE 256
#define MAX_ELEM_SIZE 32


WINDOW *pad;


static int mrow, mcol;
static char *host, *user, *pass;

void quit(void){
  // end curses
  endwin();
}

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



int main(int argc, char **argv){
  // initialize curses
  initscr();
  atexit(quit);
  clear();
  cbreak();
  keypad(stdscr, TRUE);
  start_color();

  // get terminal size
  getmaxyx(stdscr, mrow, mcol);

  // Cursor off
  curs_set(0);

  ListHead head;    
  List_init(&head);
  getAllProcData(&head);
  int n_choices = head.size;
  
  int num_fields = 8;
  int rowcount = n_choices;

  
  // create pad
  pad = newpad (rowcount + 1, mcol);

  // col titles
  wprintw(pad, "Auftrag \tName \t\t\tZeit\n");
  
 
  char* dataToString[n_choices];
  
  ListItem* it = head.first;  
  int counter =0; 
  while(it){
      ProcListItem* elem = (ProcListItem*) it;
      ProcData* pd = elem->data;
      
      char* name = procDataToString(pd);
      if(name){
          dataToString[counter] = name;
          wprintw(pad, "ciao %s\n", dataToString[counter] );
          counter++;
      }
      it=it->next;
  }
/*
  while (row = mysql_fetch_row(result)){
    int i = 0;

    for (i = 0; i < num_fields; i++){
      wprintw(pad, "%s\t", row[i]); 
    }

    wprintw(pad, "\n");
  }
*/
  // Show content of pad
  int mypadpos = 0;
  prefresh(pad, mypadpos, 0, 0, 0, mrow, mcol);
  // wait for exit key
  int ch;
  while((ch = wgetch(pad)) != 'q'){
    switch (ch){
      case KEY_UP:
        if (mypadpos >= 0){
            mypadpos--;
        }
        prefresh(pad, mypadpos, 0, 0, 0, mrow, mcol);
        break;
      case KEY_DOWN:
        if (mypadpos <= rowcount+1){
            mypadpos++;
        }
        prefresh(pad, mypadpos, 0, 0, 0, mrow-1, mcol);
        break;
     }
  }

  // remove window
  delwin(pad);
  clear();
  refresh();

  return (0);
}
