#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


pthread_t updater;
pthread_t printer;
pthread_t handler;

int isOn = 0;

/*
update -> scrive su SHM
*/
void* update(void* arg){
    //Check if program is ON.
    while(isOn){
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
    while(isOn){
        //Wait a timer.
        int ret = sleep(1); //Error handling on sleep         
        
        printf("STO GRABBANDO DALLA SHM E PRINTANDO la SHM \n"); //SIMULATION
    }
    pthread_exit(NULL);
}

void* dispatch(void* arg){
    int key = *(int*)arg; 
    if(isOn){
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

    isOn = 1; // 1 program running 0 program not running

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
            isOn = 0;
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
    

    return 0;
}
