#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_NAME "FIFO_CALLING"
#define STR_LEN 5
#define BATCH_SIZE 5

int main() {
    
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0) {
        perror("Error opening FIFO");
        return 1;
    }

    for (int i = 0; i < 10; i++) {

        int ID_STR[BATCH_SIZE];
        char MATRIX_STR[BATCH_SIZE][STR_LEN + 1];

        for (int i = 0; i < BATCH_SIZE; i++) {
            read(fd, &ID_STR[i], sizeof(int));
            read(fd, MATRIX_STR[i], STR_LEN);
            printf("ID %d: %s\n", ID_STR[i], MATRIX_STR[i]);
        }
        printf("____________________________________________________\n");

        int ID_LAST;
        ID_LAST = ID_STR[BATCH_SIZE - 1];
        write(fd, &ID_LAST, sizeof(int));
    }
    
    if (close(fd) < 0) {
        perror("Error closing FIFO");
        return 1;
    }

    return 0;
}