#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>


const int NUM_THREADS    = 2;
const int NUM_ITERATIONS = 50000000;


// Global variables
int x = 0;


// used in 1st attempt to resolve the critical session
//  bool resourceBusy = false;


void *th_inc_func (void *args)
{
    for (int i = 0; i < NUM_ITERATIONS; i++) {

        // Begin of critical section
        // 1st attempt to resolve the critical session based on a 
        // boolean variable that indicates whether the resource is 
        // being used or not        
        // while (resourceBusy ==true)
        //     ;
        // resourceBusy = true;

        ++x;

        // End of critical section
        // this approach is correct? why?
        // resourceBusy = false;
    }

    return NULL;
}


int main (int argc, char* argv[])
{
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

    return EXIT_SUCCESS;
}
