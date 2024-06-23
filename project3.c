#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 10

int CurrentID = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int notMyTurnCount = 0;

void *threadFunction(void *threadId) {
    int myId = *((int*)threadId);
    int i;
    
    for (i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        
        while (CurrentID != myId) {
            printf("Not My Turn! Thread %d\n", myId);
            notMyTurnCount++;
            pthread_cond_wait(&cond, &mutex);
        }
        
        printf("My Turn! Thread %d\n", myId);
        CurrentID++;
        if (CurrentID == NUM_THREADS + 1) {
            CurrentID = 1;
        }
        
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];
    int i;
    
    for (i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i + 1;
        if (pthread_create(&threads[i], NULL, threadFunction, &threadIds[i])) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(1);
        }
    }
    
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(1);
        }
    }
    
    printf("Total number of times 'Not My Turn!' printed: %d\n", notMyTurnCount);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}
