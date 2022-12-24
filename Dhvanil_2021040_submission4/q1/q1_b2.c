
// // //This code creates a semaphore for the forks and a semaphore for the sauce bowls, and then creates a thread for each philosopher. The philosophers will acquire the semaphores for the forks and sauce bowls before eating and release them after they are finished. This will ensure that only one philosopher can access a fork or a sauce bowl at a time, preventing the deadlock from occurring.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS];
sem_t sauce_bowls;

void eat_sauce_bowl(int id) {
    printf("Philosopher %d is eating from a sauce bowl.\n", id);
    sleep(1);
}

void use_chopsticks(int id) {
    printf("Philosopher %d is using forks.\n", id);
    sleep(1);
}

void think(int id) {
    printf("\nPhilosopher %d is thinking.\n", id);
    sleep(1);
}

void *phil(void *arg) {
    int id = *((int*)arg);
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;
    while (1) {
        if (id % 2 == 0) {
            sem_wait(&forks[left]); // acquire left fork first to avoid deadlock
            sem_wait(&forks[right]); // acquire right fork
        } else {
            sem_wait(&forks[right]); // acquire right fork first to avoid deadlock
            sem_wait(&forks[left]); // acquire left fork
        }

        sem_wait(&sauce_bowls);
        eat_sauce_bowl(id); // eat from a sauce bowl if available (only 2 sauce bowls)
        sem_post(&sauce_bowls);

        sem_post(&forks[left]);
        sem_post(&forks[right]);

        think(id);
    }
}

int main(void) {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&sauce_bowls, 0, 2);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, phil, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&sauce_bowls);

    return 0;
}
