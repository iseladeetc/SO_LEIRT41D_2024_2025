#include <stdio.h>
#include <unistd.h>

#include <math.h>
#include <stdlib.h>

#define DEFAULT_ITER    1e9

void process_work (long niter) 
{
    for (long i = 0; i < niter; i++) {
        sqrt (rand ());
    }
}

int main ()
{
    printf("Parent process starts...\n");

    fork();

    printf("My pid = %d\n", getpid());

    process_work(DEFAULT_ITER);

    printf("process %d terminating\n",   
            getpid());
    return 0;
}
