#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_t updater;
pthread_t printer;
pthread_t handler;

int isON = 0;

/*
update -> scrive su SHM
*/
void update(){
    //Check if program is ON.
    while(isON){
        //Wait a timer.
        int ret = sleep(1); //Error handling on sleep         
        
        printf("STO SCRIVENDO E AGGIORNANDO LA SHM \n"); //SIMULATION
    }
        
    return;
}
/*
SHM -> HashTable -> printa a schermo
*/
void getData(){
    while(isON){
        //Wait a timer.
        int ret = sleep(1); //Error handling on sleep         
        
        printf("STO GRABBANDO DALLA SHM E PRINTANDO la SHM \n"); //SIMULATION
    }
    return;
}

void dispatch(int key){
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
    return;
}

int main(int argc, char *argv[]){

    isON = 1; // 1 program running 0 program not running

    //Creo thread di aggiornamento "updater"
    if(pthread_create(&updater, NULL, update, NULL)){
        exit(EXIT_FAILURE);
    }

    while(1){
        //Wait for keystroke . 
        int key = getchar();
        
        //EXIT CONDITION.
        /*if(key == EXIT){
            isON = 0;
            //Uccidi tuti i thread rimanenti. 
            //libera eventuale memoria
            break;
        }*/
        
        if(pthread_create(&handler, NULL, dispatch, &key)){
            exit(EXIT_FAILURE);
        }

        //leggo da shm a intervalli di mezzo secondo

        //Aggiorno a schermo con i dati della shm.

        
    }
    

    return 0;
}
