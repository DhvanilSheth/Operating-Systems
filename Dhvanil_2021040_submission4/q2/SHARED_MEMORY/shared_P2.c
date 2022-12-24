#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/shm.h>

#define STR_LEN 5 // fixed length of each string
#define NUM_STRINGS 50 // number of strings to generate
#define BATCH_SIZE 5 // number of strings to send in each batch

int main() {
    // read from shared memory
    key_t ID = ftok("Shared_Memory_Segment", 100);
    int SHM_ID = shmget(ID, 1024, 0666 | IPC_CREAT);
    void* STRING = shmat(SHM_ID, NULL, 0);

    char RECV[1024];
    char SENT[1024];
    int count = 0 ;
    while (atoi(SENT) < 49) {
      strcpy(RECV, (char*)STRING);
      char substrings[5][6];  // 5 substrings of length 5 + null terminator

      for (int i = 0; i < 5; i++) {
          strncpy(substrings[i], &RECV[i * 7], 5);  // Copy 5 characters starting at offset i * 7
          substrings[i][5] = '\0';  // Add null terminator
          printf("ID %d : %s \n",count,  substrings[i]);
          count++;
      }
      printf("____________________________________________________\n");

      sprintf(SENT, "%c%c", RECV[33], RECV[34]); 
      strcpy(STRING, SENT);
      sleep(3);
    }

    return 0;
}

