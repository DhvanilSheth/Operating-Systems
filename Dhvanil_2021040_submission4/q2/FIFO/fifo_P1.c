#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define FIFO_NAME "FIFO_CALLING"
#define STR_LEN 5
#define NUM_STRINGS 50
#define BATCH_SIZE 5

int main() {

    struct timespec START, STOP;
    double TOTAL_TIME;
    clock_gettime(CLOCK_REALTIME, &START);

    if (mkfifo(FIFO_NAME, 0666) < 0) {
        perror("Error creating FIFO");
        return 1;
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0) {
        perror("Error opening FIFO");
        return 1;
    }

    char STR_MATRIX[NUM_STRINGS][STR_LEN + 1];
    const char alphabets[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < NUM_STRINGS; i++) {
        for (int j = 0; j < STR_LEN; j++) {
            STR_MATRIX[i][j] = alphabets[rand() % (sizeof(alphabets) - 1)];
        }
        STR_MATRIX[i][STR_LEN] = '\0';
        printf("Random string %d: %s\n", i, STR_MATRIX[i]);
    }

    int ID_LAST = 0;
    while (ID_LAST < NUM_STRINGS) {
        for (int i = 0; i < BATCH_SIZE; i++) {
            int ID_STR = ID_LAST + i;
            write(fd, &ID_STR, sizeof(int));
            write(fd, STR_MATRIX[ID_STR], STR_LEN);
        }
        int ID_ACK;
        read(fd, &ID_ACK, sizeof(int));
        printf("Acknowledged ID : %d\n", ID_ACK);
        printf("______________________________________________________\n");
        sleep(3);
        ID_LAST = ID_ACK + 1;
    }

    if (close(fd) < 0) {
        perror("Error closing FIFO");
        return 1;
    }

    clock_gettime(CLOCK_REALTIME, &STOP);
    TOTAL_TIME = (STOP.tv_sec - START.tv_sec) + (STOP.tv_nsec - START.tv_nsec) * 1e-9;
    printf("Total Time: %d ns \n",TOTAL_TIME);
    
    return 0;
}