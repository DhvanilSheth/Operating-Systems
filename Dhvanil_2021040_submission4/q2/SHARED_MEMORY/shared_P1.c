#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/shm.h>
#include <time.h>

#define STR_SIZE 5 // fixed length of each string
#define NUM_STRINGS 50 // number of strings to generate
#define BATCH_SIZE 5 // number of strings to send in each batch
#define ALPHABET_SIZE 52 // number of alphabets


int main(){

    struct timespec START, STOP;
    double TOTAL_TIME;
    clock_gettime(CLOCK_REALTIME,&START);

    //Create an array of random strings
    char *ALL_STR[NUM_STRINGS];
    for (int i = 0; i < NUM_STRINGS; i++) {

        const char alphabets[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char *STRING = malloc(sizeof(char)*(STR_SIZE+3));
        int temp1, temp2 ;

        for(int j = 0; j < STR_SIZE; j++){
            STRING[j] = alphabets[rand() % (51)];
        }
        temp1 = i / 10;
        temp2 = i % 10;

        sprintf(STRING, "%s%d%d", STRING, temp1, temp2);
        STRING[STR_SIZE + 2] = '\0';
        ALL_STR[i] = STRING;
        printf("Random String %d : %.*s\n", i, STR_SIZE, ALL_STR[i]);


    }

    //Create shared memory
    key_t ID = ftok("Shared_Memory_Segment",100); //65 is the key value for shared memory segment
    int SHM_ID = shmget(ID, 1024, 0666|IPC_CREAT);
    void *SHM_MEM = shmat(SHM_ID, NULL, 0);
    
    char SENT[1024];
    char RET_STRING[1024];
    int index=0;
    
    while( atoi(RET_STRING) < 49){

        sprintf(SENT,"%s%s%s%s%s", ALL_STR[index], ALL_STR[index+1], ALL_STR[index+2], ALL_STR[index+3] ,ALL_STR[index+4]);
        strcpy(SHM_MEM, SENT);
        sleep(3);

        strcpy(RET_STRING, (char *)SHM_MEM);
        printf("Acknowledgement ID : %d\n",atoi(RET_STRING));
        printf("____________________________________________________\n");
        if (atoi(RET_STRING) != 0) index = atoi(RET_STRING) + 1;
        else index = 0;
    }
    
    clock_gettime(CLOCK_REALTIME, &STOP);
    TOTAL_TIME = (STOP.tv_sec - START.tv_sec) + (STOP.tv_nsec - START.tv_nsec) * 1e-9;
    printf("Total Time: %d ns \n",TOTAL_TIME);
    
    return 0;

}