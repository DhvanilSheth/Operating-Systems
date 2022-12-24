#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>

int main(){
    time_t now = time(NULL);
    printf("%ld\n", now);
    char *script1 = "./script1.sh";
    char *script2 = "./script2.sh";
    char *script3 = "./script3.sh";
    time_t startA , endA , startB , endB , startC , endC;

    pid_t pid1 = fork();

    if (pid1 < 0 ){
        printf("fork error: %s\n", strerror(errno));
    }
    else if (pid1 == 0 ){
        execlp(script1, NULL);
        return 0;
        printf("Child Process1 \n");
    }
    else{
        clock_gettime(CLOCK_REALTIME, &startA);
        printf("Parent Process Pid1 \n");
        pid_t pid2 = fork();
        if (pid2 < 0 ){
            printf("fork error: %s\n", strerror(errno));
        }
        else if (pid2 == 0){
            execlp (script2, NULL);
            return 0;
            printf("Child Process2 \n");
        }
        else{
            clock_gettime(CLOCK_REALTIME, &startB);
            printf("Parent Process Pid2 \n");
            pid_t pid3 = fork();
            if (pid3 < 0 ){
                printf("fork error: %s\n", strerror(errno));
            }
            else if (pid3 == 0){
                execlp (script3, NULL);
                return 0;  
                printf("Child Process3 \n");
            }
            else{
                clock_gettime(CLOCK_REALTIME, &startC);
                printf("Parent Process Pid3 \n");
                wait(NULL);
                clock_gettime(CLOCK_REALTIME, &endC);
                printf("End Time for process 3 : %ld\n", endC);
            }
            wait(NULL);
            clock_gettime(CLOCK_REALTIME, &endB);
            //print end time
            printf("End Time for process 2 : %ld\n", endB);

        }
        wait(NULL);
        clock_gettime(CLOCK_REALTIME, &endA);
        //print end time
        printf("End Time for process 1 : %ld\n", endA);
    }
}
// #!/bin/bash
// cd /
// (cd /home/user/submission3/linux-stock1 && make mrproper )
// cp /root/linux-stock2/.config /home/user/submission3/linux-stock1/
// cd /home/user/submission3/linux-stock1
// make -j6