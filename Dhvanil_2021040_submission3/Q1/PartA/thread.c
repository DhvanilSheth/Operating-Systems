#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sched.h>
#define MAX 4294967295

void *countA(void* arg){
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    int num = *(int*)arg;
    nice(num);
    for(long long i=0; i< MAX; i++);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec));
    return (void *) time_taken;
}

void *countB(){
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(long long i=0; i< MAX; i++);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec));
    return (void *) time_taken;
}

void *countC(){
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(long long i=0; i< MAX; i++);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec));
    return (void *) time_taken;
}

int main(int args,char *argv[]){

    //create 3 threads and run them in parallel using pthread_create, pthread_set_schedparam, pthread_join
    pthread_t thread1, thread2, thread3;
    struct sched_param param1, param2, param3;
    int num = atoi(argv[1]);
    void *return_value1, *return_value2, *return_value3;
    int ret;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    // pthread_create(&thread1, NULL, &countA, &id1);
    pthread_create(&thread1, NULL, countA, (void *)&num);
    pthread_create(&thread2, NULL, countB, NULL);
    pthread_create(&thread3, NULL, countC, NULL);

    param1.sched_priority = 0;
    //set priority of thread2 to argv[2]
    param2.sched_priority = atoi(argv[2]);
    //set priority of thread3 to argv[3]
    param3.sched_priority = atoi(argv[3]);

    ret = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    printf( "pthread_setschedparam returned %d for thread1 \n", ret); 
    if (ret != 0) {
        printf("Error in setting SCHED_OTHER policy for thread1");
        exit(1);
    }
    ret = pthread_setschedparam (thread2, SCHED_RR, &param2);
    printf( "pthread_setschedparam returned %d for thread2 \n", ret);
    if (ret != 0) {
        printf("Error in setting SCHED_RR policy for thread2");
        exit(1);
    }
    ret = pthread_setschedparam (thread3, SCHED_FIFO, &param3);
    printf( "pthread_setschedparam returned %d for thread3 \n", ret);
    if (ret != 0) {
        printf("Error in setting SCHED_FIFO policy for thread3");
        exit(1);
    }
    pthread_join(thread1, &return_value1);
    pthread_join(thread2, &return_value2);
    pthread_join(thread3, &return_value3);

    //write the return values to a file
    FILE *file;
    file = fopen("cache_values.txt", "a+");
    fprintf(file, "%ld %ld %ld\n", (long) return_value1/1000000, (long) return_value2/1000000, (long) return_value3/1000000);
    fclose(file);

    //print the return values in milliseconds
    printf("Time taken by thread1: %ld milliseconds \n", (long) return_value1/1000000);
    printf("Time taken by thread2: %ld milliseconds \n", (long) return_value2/1000000);
    printf("Time taken by thread3: %ld milliseconds \n", (long) return_value3/1000000);
    return 0;

}