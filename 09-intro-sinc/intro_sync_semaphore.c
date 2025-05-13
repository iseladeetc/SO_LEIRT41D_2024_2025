#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>


const int NUM_THREADS    = 2;
const int NUM_ITERATIONS = 50000000;


// Global variables
int x = 0;


sem_t semMutex;


void *th_inc_func (void *args)
{
    for (int i = 0; i < NUM_ITERATIONS; i++) {

        int retval;
        
        // Begin of critical section
        retval = sem_wait(&semMutex);
        if (retval < 0) {
            errno = retval;
            perror("sem_wait failed"); 
            exit(EXIT_FAILURE); 
        }

        ++x;

        // End of critical section
        retval = sem_post(&semMutex);
        if (retval < 0) {
            errno = retval;
            perror("sem_post"); 
            exit(EXIT_FAILURE); 
        }
    }

    return NULL;
}

int main (int argc, char* argv[])
{
    int retval = sem_init(&semMutex, 0, 1);
    if (retval < 0) {
        errno = retval;
    	perror("sem_init failed"); 
        exit(EXIT_FAILURE); 
    }
    
    // Creation of threads
    pthread_t threads[NUM_THREADS];

    for (int idTh = 0; idTh < NUM_THREADS; idTh++) {
        
        printf("Creating thread %d...\n", idTh);

        int retval = pthread_create(&threads[idTh], NULL, th_inc_func, NULL);
        if (retval != 0) {
            errno = retval;
            perror("pthread_create failed"); 
            exit(EXIT_FAILURE);
        }
    }

    // Wait for termination of all threads
    for (int idThJoin = 0; idThJoin < NUM_THREADS; ++idThJoin) {

        int retval = pthread_join(threads[idThJoin], NULL);
        if (retval != 0) {
            errno = retval;
            perror("pthread_join failed"); 
            exit(EXIT_FAILURE);
        }
        printf("Finished thread %d.\n", idThJoin);
    }

    printf("\nFinal value of x = %d\n", x);

    retval = sem_destroy(&semMutex);
    if (retval < 0) {
        errno = retval;
    	perror("sem_destroy"); 
        exit(EXIT_FAILURE); 
    }

    return EXIT_SUCCESS;
}
