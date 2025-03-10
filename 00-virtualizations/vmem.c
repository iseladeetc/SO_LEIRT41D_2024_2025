#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define DEFAULT_ITER    1e8

void process_work (long niter) 
{
    for (long i = 0; i < niter; i++) {
        sqrt (rand ());
    }
}

int main () 
{
    int *p = malloc(sizeof(*p));
    if (p == NULL) {
        printf("Malloc failed!\n");
        return -1;
    }
    
    printf("[%d] address pointed to by p: %p\n", getpid(), p);

    while (1) {
        process_work(DEFAULT_ITER);
        *p = *p + 1;
        printf("[%d] p: %d\n", getpid(), *p);

    }
    return 0;
}