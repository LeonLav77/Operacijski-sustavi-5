#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>


enum Food {
    COKOLADA,
    CEVAPI,
    JANJETINA,
    STRUKLE,
    GRAH,
    KEKSI,
    KELJ,
    JABUKA,
    PRAZNO,
};

pthread_mutex_t lock; 
int REMAINING_FOOD = 5;
const int FOOD_COUNT = 5;
enum Food foodOnTable[5];

int generateRandom(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

void placeFoodOnTableActivity(const char *name);
int eatActivity(const char *name);

int factorial(int n) {
    if (n <= 1) {
        return 1; // Base case: factorial of 0 and 1 is 1
    } else {
        return n * factorial(n - 1); // Recursive call to calculate factorial
    }
}

void sleepActivity(const char *name){
    printf("%s is sleeping.\n", name);
    sleep(generateRandom(5,10));
    if (strcmp(name, "Tin") == 0) {
        printf("Tin spava jos malo\n");
        sleep(generateRandom(1,3));
    }
}

void programmingActivity(const char *name){
    printf("%s is programming.\n", name);
    int factorialEntry = generateRandom(1,10);
    int notUsed = factorial(factorialEntry);
}

void carServiceActivity(const char *name){
    printf("%s is taking its car for a service.\n", name);
    int chance = generateRandom(0,100);
    if(chance < 25){
        sleep(4);
    }else if(chance < 45){
        sleep(2);
    }else{
        sleep(generateRandom(0,5));
    }
}

void printMealsByName(const char *name, int numberOfTimesAccessedTable, int numberOfTimeEaten, int numberOfTimeNotEatenDueToDislike) {
    printf("%s has eaten %d times, didn't like the food %d times, and has been left hungry %d times.\n", name, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike, numberOfTimesAccessedTable - numberOfTimeEaten - numberOfTimeNotEatenDueToDislike);
}

void baseActivity(){
    sleep(generateRandom(2,4));
}

void tenisActivity(const char *name){
    printf("%s is playing tennis.\n", name);
    baseActivity();
}
void listenToPianoActivity(const char *name){
    printf("%s is listening to piano.\n", name);
    baseActivity();
}
void watchTVActivity(const char *name){
    printf("%s is watching TV.\n", name);
    baseActivity();
}
void cookActivity(const char *name){
    printf("%s is cooking.\n", name);
    sleep(generateRandom(1,2));
}
void restActivity(const char *name){
    printf("%s is resting.\n", name);
    sleep(generateRandom(1,2));
}

int isTableEmpty(){
    // stavi novo samo ako je stol prazan
    for (int i = 0; i < FOOD_COUNT; i++) {
        // ako nade ijednu hranu nije prazan
        if(foodOnTable[i] != PRAZNO){
            return 0;
        }
    }
    return 1;
}

void *tinPipeline(void *arg)
{
    const char *name = "Tin";
    int numberOfTimesAccessedTable = 0;
    int numberOfTimeEaten = 0;
    int numberOfTimeNotEatenDueToDislike = 0;
    int eatResult;
    while (1)
    {
        sleepActivity(name);
        programmingActivity(name);
        numberOfTimesAccessedTable++;
        pthread_mutex_lock(&lock);
        eatResult = eatActivity(name);
        numberOfTimeEaten += (eatResult >= 0) ? eatResult : 0;
        if (eatResult == -1)
        {
            numberOfTimeNotEatenDueToDislike++;
        }
        
        pthread_mutex_unlock(&lock); 
        carServiceActivity(name);
        if (REMAINING_FOOD <= 0 && isTableEmpty() == 1)
        {
            break;
        }
    }
    printf("TIMES ACCESSED: %d, TIMES EATEN %d, TIMES DISLIKED %d, TIMES EMPTY TABLE: %d\n", numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike, numberOfTimesAccessedTable - numberOfTimeEaten - numberOfTimeNotEatenDueToDislike);
    // printMealsByName(name, numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike);
    pthread_exit(NULL);
}
void *davorPipeline(void *arg)
{
    const char *name = "Davor";
    int numberOfTimesAccessedTable = 0;
    int numberOfTimeEaten = 0;
    int numberOfTimeNotEatenDueToDislike = 0;
    int eatResult;
    while (1)
    {
        sleepActivity(name);
        programmingActivity(name);
        numberOfTimesAccessedTable++;
        pthread_mutex_lock(&lock); 
        eatResult = eatActivity(name);
        numberOfTimeEaten += (eatResult >= 0) ? eatResult : 0;
        if (eatResult == -1)
        {
            numberOfTimeNotEatenDueToDislike++;
        }
        
        pthread_mutex_unlock(&lock); 
        watchTVActivity(name);
        if (REMAINING_FOOD <= 0 && isTableEmpty() == 1)
        {
            break;
        }
    }
    printf("TIMES ACCESSED: %d, TIMES EATEN %d, TIMES DISLIKED %d, TIMES EMPTY TABLE: %d\n", numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike, numberOfTimesAccessedTable - numberOfTimeEaten - numberOfTimeNotEatenDueToDislike);
    // printMealsByName(name, numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike);
    pthread_exit(NULL);
}
void *ivicaPipeline(void *arg)
{
    const char *name = "Ivica";
    int numberOfTimesAccessedTable = 0;
    int numberOfTimeEaten = 0;
    int numberOfTimeNotEatenDueToDislike = 0;
    int eatResult;
    while (1)
    {
        sleepActivity(name);
        tenisActivity(name);
        numberOfTimesAccessedTable++;
        pthread_mutex_lock(&lock); 
                eatResult = eatActivity(name);
        numberOfTimeEaten += (eatResult >= 0) ? eatResult : 0;
        if (eatResult == -1)
        {
            numberOfTimeNotEatenDueToDislike++;
        }
        
        pthread_mutex_unlock(&lock); 
        programmingActivity(name);
        if (REMAINING_FOOD <= 0 && isTableEmpty() == 1)
        {
            break;
        }
    }
    printf("TIMES ACCESSED: %d, TIMES EATEN %d, TIMES DISLIKED %d, TIMES EMPTY TABLE: %d\n", numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike, numberOfTimesAccessedTable - numberOfTimeEaten - numberOfTimeNotEatenDueToDislike);

    // printMealsByName(name, numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike);
    pthread_exit(NULL);
}
void *ivanPipeline(void *arg)
{
    const char *name = "Ivan";
    int numberOfTimesAccessedTable = 0;
    int numberOfTimeEaten = 0;
    int numberOfTimeNotEatenDueToDislike = 0;
    int eatResult;
    while (1)
    {
        sleepActivity(name);
        listenToPianoActivity(name);
        numberOfTimesAccessedTable++;
        pthread_mutex_lock(&lock); 
                eatResult = eatActivity(name);
        numberOfTimeEaten += (eatResult >= 0) ? eatResult : 0;
        if (eatResult == -1)
        {
            numberOfTimeNotEatenDueToDislike++;
        }
        
        pthread_mutex_unlock(&lock); 
        programmingActivity(name); 
        if (REMAINING_FOOD <= 0 && isTableEmpty() == 1)
        {
            break;
        }
    }
    printf("TIMES ACCESSED: %d, TIMES EATEN %d, TIMES DISLIKED %d, TIMES EMPTY TABLE: %d\n", numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike, numberOfTimesAccessedTable - numberOfTimeEaten - numberOfTimeNotEatenDueToDislike);

    // printMealsByName(name, numberOfTimesAccessedTable, numberOfTimeEaten, numberOfTimeNotEatenDueToDislike);
    pthread_exit(NULL);
}

void *kuharicaPipeline(void *arg)
{
    const char *name = "Kuharica";
    while (1)
    {
        cookActivity(name);
        pthread_mutex_lock(&lock);
        placeFoodOnTableActivity(name);
        pthread_mutex_unlock(&lock); 
        restActivity(name);
        if (REMAINING_FOOD <= 0 && isTableEmpty() == 1)
        {
            break;
        }
        
    }
    pthread_exit(NULL);
}

void printTable(){
    for (size_t i = 0; i < FOOD_COUNT; i++)
    {
        printf("%d", foodOnTable[i]);
    }
    printf("\n");
}

void placeFoodOnTableActivity(const char *name){
    if(REMAINING_FOOD <= 0){
        return;
    }
    int emptyTable = isTableEmpty();
    if(emptyTable == 1){
        for (int i = 0; i < FOOD_COUNT; i++) {
            foodOnTable[i] = generateRandom(0,7);
        }
        REMAINING_FOOD -= FOOD_COUNT;
        printf("%s puts food on the table.\n", name);
        return;
    }
    printf("%s doesnt put anything on the table because it is not needed.\n", name);
    printTable();
}

int eatActivity(const char *name){
    // return -1 for didnt like food
    // return 0 for table empty
    // return 1 for ate
    // Tin vegetarijansko - 0,3,4,5,6,7
    // Davor neslatko - 1,2,4,6,7
    // Ivica necuspajs - 0,1,2,3,5,6,7
    // Ivan  nestrukla - 0,1,2,4,5,6,7
    printf("%s is eating.\n", name);
    printTable();
    if(isTableEmpty() == 1){
        printf("%s se nije najeo jer je stol bio prazan\n", name);
        return 0;
    }else if (strcmp(name, "Tin") == 0) 
    {
       for (int i = 0; i < FOOD_COUNT; i++) {
            // Check if the food on the table matches Tin's vegetarian preferences
            if (foodOnTable[i] == COKOLADA || foodOnTable[i] == STRUKLE || foodOnTable[i] == GRAH ||
                foodOnTable[i] == KEKSI || foodOnTable[i] == KELJ || foodOnTable[i] == JABUKA) {
                printf("%s is eating %d.\n", name, foodOnTable[i]);
                foodOnTable[i] = PRAZNO;
                return 1;
            }
        }

    }else if(strcmp(name, "Davor"))
    {

        for (int i = 0; i < FOOD_COUNT; i++) {
            // Check if the food on the table matches Tin's vegetarian preferences
            if (foodOnTable[i] == CEVAPI || foodOnTable[i] == JANJETINA || foodOnTable[i] == GRAH ||
                foodOnTable[i] == KELJ || foodOnTable[i] == JABUKA) {
                // Tin can eat this food item
                printf("%s is eating %d.\n", name, foodOnTable[i]);
                foodOnTable[i] = PRAZNO;
                return 1;
            
            }
        }

    }else if(strcmp(name, "Ivica"))
    {

        for (int i = 0; i < FOOD_COUNT; i++) {
            // Check if the food on the table matches Tin's vegetarian preferences
            if (foodOnTable[i] == COKOLADA || foodOnTable[i] == CEVAPI || foodOnTable[i] == JANJETINA || foodOnTable[i] == STRUKLE ||
                foodOnTable[i] == KEKSI || foodOnTable[i] == KELJ || foodOnTable[i] == JABUKA) {
                // Tin can eat this food item
                printf("%s is eating %d.\n", name, foodOnTable[i]);
                foodOnTable[i] = PRAZNO;
                return 1;
            
            }
        }


    }else if(strcmp(name, "Ivan"))
    {
        for (int i = 0; i < FOOD_COUNT; i++) {
            // Check if the food on the table matches Tin's vegetarian preferences
            if (foodOnTable[i] == COKOLADA || foodOnTable[i] == CEVAPI || foodOnTable[i] == JANJETINA || foodOnTable[i] == GRAH ||
                foodOnTable[i] == KEKSI || foodOnTable[i] == KELJ || foodOnTable[i] == JABUKA) {
                // Tin can eat this food item
                printf("%s is eating %d.\n", name, foodOnTable[i]);
                foodOnTable[i] = PRAZNO;
                return 1;
            }
        }
    }
    printf("%s SE NE SVIDA HRANA\n", name);
    return -1;
}

int main() {
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }
    srand(time(NULL));
    for (size_t i = 0; i < FOOD_COUNT; i++)
    {
        // initial zapis kako bi kuharica mogla spremit
        foodOnTable[i] = PRAZNO;
    }
     

    pthread_t tin; 
    pthread_t davor; 
    pthread_t ivica; 
    pthread_t ivan;
    pthread_t kuharica;

    pthread_create(&tin, NULL, tinPipeline, NULL); 
    pthread_create(&davor, NULL, davorPipeline, NULL); 
    pthread_create(&ivica, NULL, ivicaPipeline, NULL); 
    pthread_create(&ivan, NULL, ivanPipeline, NULL); 
    pthread_create(&kuharica, NULL, kuharicaPipeline, NULL); 

    pthread_join(tin, NULL); 
    pthread_join(davor, NULL); 
    pthread_join(ivica, NULL); 
    pthread_join(ivan, NULL); 
    pthread_join(kuharica, NULL); 

    pthread_mutex_destroy(&lock); 
    return 0;
}
