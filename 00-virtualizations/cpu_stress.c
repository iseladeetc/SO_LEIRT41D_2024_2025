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
    process_work(DEFAULT_ITER);
    return 0;
}