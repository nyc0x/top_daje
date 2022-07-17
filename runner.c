#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

struct termios info;

pthread_t updater;
pthread_t printer;
pthread_t handler;

int isON = 0;

/*
update -> scrive su SHM
*/
void* update(void* arg){
    //Check if program is ON.
    while(isON){
        //Wait a timer.
        int ret = sleep(1); //Error handling on sleep         
        
        printf("STO SCRIVENDO E AGGIORNANDO LA SHM \n"); //SIMULATION
    }
    pthread_exit(NULL);
}
/*
SHM -> HashTable -> printa a schermo
*/
void* getData(void* arg){
    while(isON){
        //Wait a timer.
        int ret = sleep(1); //Error handling on sleep         
        
        printf("STO GRABBANDO DALLA SHM E PRINTANDO la SHM \n"); //SIMULATION
    }
    pthread_exit(NULL);
}

void* dispatch(void* arg){
    int key = *(int*)arg; 
    if(isON){
        switch (key){
        case 1 :
            printf("Dispatch to test \n");
            //kill(getPIDfromSHM())
            break;
        default:
            break;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &info); /* set immediately */


    isON = 1; // 1 program running 0 program not running

    //Creo thread di aggiornamento "updater"
    if(pthread_create(&updater, NULL, &update, NULL)){
        exit(EXIT_FAILURE);
    }
    
    //leggo da shm a intervalli di mezzo secondo
    //Aggiorno a schermo con i dati della shm.
    if(pthread_create(&printer, NULL, &getData, NULL)){
        exit(EXIT_FAILURE);
    } 
    

    while(1){
        int ch = getchar();
        
        //EXIT CONDITION
        if(ch == 27){ //27 <-> ESC
            isON = 0;
            break;
        }
        if(ch < 0 )
            printf("ERROR HANDLING \n");
        else
            if(pthread_create(&handler, NULL, &dispatch, &ch)){
                exit(EXIT_FAILURE);
            }
    }
    
    
    //Clean-up
    pthread_join(updater ,NULL);
    pthread_join(printer ,NULL);
    pthread_join(handler,NULL);
    
    //SET BACK TERMINAL CONFIGURATION
    tcgetattr(0, &info);
    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);
    
    return 0;
}
