#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#define SERVER_SOCK_NAME "/tmp/server_socket"
#define STR_LEN 5
#define NUM_STRINGS 50
#define BATCH_SIZE 5

int main(int argc , char *argv[]){

    struct timespec START, STOP;
    double TOTAL_TIME;
    clock_gettime(CLOCK_REALTIME,&START);

    struct sockaddr_un name;
    struct sockaddr_un{
        sa_family_t sun_family;
        char sun_path[108];
    };

    int ret; 
    int connection_socket;
    int data_socket;

    unlink(SERVER_SOCK_NAME); // remove the socket if it already exists 

    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0); // create the socket and return the socket descriptor 
    if (connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("The server socket was created\n");
    memset(&name, 0, sizeof(struct sockaddr_un));  // clear the structure 

    name.sun_family = AF_UNIX; // set the family 
    strncpy(name.sun_path, SERVER_SOCK_NAME, sizeof(name.sun_path) - 1); // copy the name of the socket 

    if (bind(connection_socket, (const struct sockaddr *) &name, sizeof(struct sockaddr_un)) == -1){ // bind the socket to the socket name 
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() returned success\n");

    if (listen(connection_socket, 20) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("listen() returned success\n");
    while(1){  
        printf("Waiting on accept() \n");
        data_socket = accept(connection_socket, NULL, NULL);

        if (data_socket == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("accept() returned success\n");

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
                ret = write(data_socket, &ID_STR, sizeof(int));
                if (ret == -1){
                    perror("write1");
                    exit(EXIT_FAILURE);
                }
                ret = write(data_socket, STR_MATRIX[ID_STR], STR_LEN);
                if (ret == -1){
                    perror("write2");
                    exit(EXIT_FAILURE);
                }
            }
            int ID_ACK;
            ret = read(data_socket, &ID_ACK, sizeof(int));
            if (ret == -1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("Acknowledged ID : %d\n", ID_ACK);
            printf("______________________________________________________\n");
            sleep(3);
            ID_LAST = ID_ACK + 1;
        }
        close (data_socket);
        printf("The data socket is closed\n");
    }
    close(connection_socket);
    printf("The server socket is closed\n");
    unlink(SERVER_SOCK_NAME);
    printf("The server socket is unlinked\n");

    clock_gettime(CLOCK_REALTIME, &STOP);
    TOTAL_TIME = (STOP.tv_sec - START.tv_sec) + (STOP.tv_nsec - START.tv_nsec) * 1e-9;
    printf("Total Time: %d ns \n",TOTAL_TIME);
    return 0;

}