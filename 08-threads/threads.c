#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>


#define DEFAULT_ITER    1e7
#define DEFAULT_THREADS 2

void process_work (long niter) 
{
    for (long i = 0; i < niter; i++) {
        sqrt (rand ());
    }
}

void *process_work_thread (void *arg)
{
    long *pniter= (long *)arg;
    long niter = *pniter;

    process_work (niter);
    return NULL;
}

int main (int argc, char *argv[])
{
    long niter   = DEFAULT_ITER;
    int nthreads = DEFAULT_THREADS;

    if (argc == 1) {
        printf ("Usage: %s [niter] [nthreads]\n", argv[0]);
    }
    if (argc > 1) {
        niter = atol (argv[1]);
        if (niter < 1) {
            fprintf (stderr, "niter must be >= 1\n");
            exit (EXIT_FAILURE);
        }
    }
    if (argc > 2) {
        nthreads = atoi (argv[2]);
        if (nthreads < 1) {
            fprintf (stderr, "nthreads must be >= 1\n");
            exit (EXIT_FAILURE);
        }
    }

    printf ("niter = %ld, nthreads = %d\n", niter, nthreads);


    pthread_t threads[nthreads];
    for (int i = 0; i < nthreads; i++) {
        int err = pthread_create (&threads[i], NULL, process_work_thread, &niter);
        if (err != 0) {
            errno = err;
            perror("pthread_create");
            exit (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join (threads[i], NULL);
    }

    return EXIT_SUCCESS;
}