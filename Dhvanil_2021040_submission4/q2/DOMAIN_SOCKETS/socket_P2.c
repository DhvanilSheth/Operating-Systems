#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SERVER_SOCK_NAME "/tmp/server_socket"
#define STR_LEN 5
#define BATCH_SIZE 5

int main(){
    struct sockaddr_un addr;

    int i;
    int ret;

    int data_socket;
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (data_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_SOCK_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));
    if (ret == -1){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("connect() returned success\n");

    for (i = 0; i < 10; i++){
        int ID_STR[BATCH_SIZE];
        char MATRIX_STR[BATCH_SIZE][STR_LEN + 1];

        for (int i = 0; i < BATCH_SIZE; i++) {
            ret = read(data_socket, &ID_STR[i], sizeof(int));
            if (ret == -1){
                perror("read1");
                exit(EXIT_FAILURE);
            }
            ret = read(data_socket, MATRIX_STR[i], STR_LEN);
            if (ret == -1){
                perror("read2");
                exit(EXIT_FAILURE);
            }
            printf("ID %d: %s \n", ID_STR[i], MATRIX_STR[i]);
        }
        printf("____________________________________________________\n");

        int ID_LAST;
        ID_LAST = ID_STR[BATCH_SIZE - 1];
        ret = write(data_socket, &ID_LAST, sizeof(int));
        if (ret == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    close(data_socket);
    exit(EXIT_SUCCESS);
    
}