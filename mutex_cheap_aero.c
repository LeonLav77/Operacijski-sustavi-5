#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define ROWS 10
#define COLS 10
#define NUM_THREADS 100

int seating[ROWS][COLS] = {0};
pthread_mutex_t lock; 

int generateRandom(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

void login(){
    sleep(generateRandom(1,5));
    printf("Loginan\n");
}

void fetchSeats(){
    sleep(generateRandom(1,3));
    printf("Fetchali sjedala\n");
}

void chooseSeat(FILE *file) {
    int accepted;
    int attempts = 0;
    int totalSeats = ROWS * COLS;
    int maxAttempts = totalSeats * 2;

    while (attempts < maxAttempts) {
        accepted = generateRandom(0, 1);
        int row, col;

        pthread_mutex_lock(&lock); 
        do {
            row = generateRandom(0, ROWS - 1);
            col = generateRandom(0, COLS - 1);
        } while (seating[row][col] != 0);

        if (accepted == 1) {
            sleep(generateRandom(1,2));
            seating[row][col] = 1;
            printf("NADENO SJEDALO [%d][%d]\n", row, col);
            fprintf(file, "NADENO SJEDALO [%d][%d]\n", row, col); 
            pthread_mutex_unlock(&lock); 
            return; 
        }
        attempts++;
        pthread_mutex_unlock(&lock); 
    }

    printf("Thread timed out: No available seats\n");
}

void printSeats() {
    printf("Seating arrangement:\n");
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            printf("%2d ", seating[i][j]); // Adjust formatting as needed
        }
        printf("\n");
    }
}

int countValues(int value) {
    int count = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (seating[i][j] == value) {
                count++;
            }
        }
    }
    return count;
}

// Function that will be executed by the thread
void *check_in(void *arg) {
    sleep(generateRandom(0,1));
    login();
    fetchSeats();
    chooseSeat((FILE *)arg);
    pthread_exit(NULL);
}

int main() {
    FILE *file = fopen("cheap_seating.txt", "w"); // Open file for writing

    pthread_t thread_ids[NUM_THREADS]; // Array to store thread IDs
    int result;

    for (size_t i = 0; i < NUM_THREADS; i++) {
        result = pthread_create(&thread_ids[i], NULL, check_in, (void *)file);

        if (result != 0) {
            printf("Thread creation failed\n");
            return 1;
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    printSeats();
    printf("Ukupno zauzetih: %d\n", countValues(1));
    printf("Ukupno slobodnih: %d\n", countValues(0));

    fclose(file); // Close the file
    return 0;
}
