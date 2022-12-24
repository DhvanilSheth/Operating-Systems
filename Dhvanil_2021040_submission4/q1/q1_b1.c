#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

int forks[NUM_PHILOSOPHERS];
int sauce_bowls[2];

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
    int id = *((int *)arg);
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;
    while (1) {
        int bowl = -1;
        for (int i = 0; i < 2; i++) {
            if (sauce_bowls[i] == 0) {
                bowl = i;
                break;
            }
        }
        if (bowl == -1) {
        // all sauce bowls are in use, wait until one becomes available
            while (1) {
                for (int i = 0; i < 2; i++) {
                    if (sauce_bowls[i] == 0) {
                        bowl = i;
                        break;
                    }
                }
                if (bowl != -1) {
                    break;
                }
            }
        }
        sauce_bowls[bowl] = 1; // mark sauce bowl as in use

        if (id % 2 == 0) {
            // acquire left fork first to avoid deadlock
            while (forks[left] != 0) { } // wait until left fork becomes available
            forks[left] = 1; // mark left fork as in use

            // acquire right fork
            while (forks[right] != 0) { } // wait until right fork becomes available
            forks[right] = 1; // mark right fork as in use
        } 
        else {
            // acquire right fork first to avoid deadlock
            while (forks[right] != 0) { } // wait until right fork becomes available
            forks[right] = 1; // mark right fork as in use

            // acquire left fork
            while (forks[left] != 0) { } // wait until left fork becomes available
            forks[left] = 1; // mark left fork as in use
        }

        eat_sauce_bowl(id); // eat from sauce bowl

        forks[left] = 0; // release left fork
        forks[right] = 0; // release right fork
        sauce_bowls[bowl] = 0; // release sauce bowl

        think(id);
    }
}
int main(){
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // initialize forks and sauce bowls
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = 0;
    }
    sauce_bowls[0] = 0;
    sauce_bowls[1] = 0;

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, phil, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
