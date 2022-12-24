#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PHIL_COUNT 5
#define FORK_COUNT 5
#define TRUE 1

int forks[FORK_COUNT];

void *phil (void *num) {

    int philosopher_num = *((int *) num);
    int left_fork = philosopher_num;
    int right_fork = (philosopher_num + 1) % FORK_COUNT;

    while (TRUE) {
        printf("Philosopher %d is thinking\n", philosopher_num);
        sleep(3);

        while (forks[left_fork] || forks[right_fork]) {
            // If the forks are not available, wait and check again
            sleep(1);
        }

        forks[left_fork] = 1; // Pick up the left fork
        printf("Philosopher %d picked up fork %d and is waiting for fork %d\n" , philosopher_num, left_fork, right_fork);
        forks[right_fork] = 1; // Pick up the right fork
        printf("Philosopher %d picked up fork %d and is eating with fork %d\n" , philosopher_num, right_fork, left_fork);

        printf("\nPhilosopher %d is eating\n\n", philosopher_num);
        sleep(1);

        forks[left_fork] = 0; // Put down the left fork
        printf("Philosopher %d put down fork %d and is waiting for fork %d\n" , philosopher_num, left_fork, right_fork);
        forks[right_fork] = 0; // Put down the right fork
        printf("Philosopher %d put down fork %d and is thinking\n" , philosopher_num, right_fork);
    }
}

int main() {
    
    pthread_t philosophers[PHIL_COUNT];
    int philosopher_arr[PHIL_COUNT];

    // Initialize the forks
    for (int i = 1; i <= FORK_COUNT; i++) {
        forks[i] = 0;
    }

    // Create the philosophers
    for (int i = 1; i <= PHIL_COUNT; i++) {
        philosopher_arr[i] = i;
        pthread_create(&philosophers[i], NULL, phil, &philosopher_arr[i]);
    }

    // Wait for the philosophers to finish
    for (int i = 1; i <= PHIL_COUNT; i++) {
      pthread_join(philosophers[i], NULL);
    }

    return 0;
}
