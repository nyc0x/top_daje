#include <pthread.h>
#include <stdlib.h>
pthread_t updater;



void update(){
    //Check if program is ON.
    while(isON){
        //Wait a timer.
        
        printf("STO AGGIORNANDO LA SHM \n");
    }
        
    return;
}

int main(int argc, char *argv[]){

    //Creo thread di aggiornamento "updater"
    if(pthread_create(&updater, NULL, update, NULL)){
        exit(EXIT_FAILURE);
    }

    while(1){
        //Wait for keystroke . 
            //Crea thread e dispaccialo con la callback dell'evento.
        //leggo da shm a intervalli di mezzo secondo

        //Aggiorno a schermo con i dati della shm.

    }
    

    return 0;
}
