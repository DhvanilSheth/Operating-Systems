#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define TRUE 1 

// Semaphores to represent the forks
sem_t forks[NUM_FORKS];

// Function to simulate the actions of a philosopher
void* philosopher(void* num) {

    int philosopher_num = *((int *) num);
    int left_fork = philosopher_num;
    int right_fork = (philosopher_num + 1) % NUM_FORKS;

    while (TRUE) {
      printf("Philosopher %d is thinking\n", philosopher_num);
        sleep(1);

        // Determine which fork to pick up first based on priority
        if (left_fork > right_fork) {
            int temp = left_fork;
            left_fork = right_fork;
            right_fork = temp;
        }

        // Pick up left fork & right fork
        sem_wait(&forks[left_fork]);
        printf("Philosopher %d picked up fork %d and is waiting for fork %d\n" , philosopher_num, left_fork, right_fork);
        sem_wait(&forks[right_fork]);
        printf("Philosopher %d picked up fork %d and is eating with fork %d\n" , philosopher_num, right_fork, left_fork);

        // Eat
        printf("\nPhilosopher %d is eating\n\n", philosopher_num);
        sleep(1);

        // Put down left fork & right fork
        sem_post(&forks[left_fork]);
        printf("Philosopher %d put down fork %d and is waiting for fork %d\n" , philosopher_num, left_fork, right_fork);
        sem_post(&forks[right_fork]);
        printf("Philosopher %d put down fork %d and is thinking\n" , philosopher_num, right_fork);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_nums[NUM_PHILOSOPHERS];

    // Initialize the semaphores representing the forks
    for (int i = 1; i <= NUM_FORKS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create the threads for each philosopher
    for (int i = 1; i <= NUM_PHILOSOPHERS; i++) {
        philosopher_nums[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_nums[i]);
    }

    // Wait for the threads to finish
    for (int i = 1; i <= NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
