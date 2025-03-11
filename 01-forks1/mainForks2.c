#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_ITER    1e9

void process_work (long niter) 
{
    for (long i = 0; i < niter; i++) {
        sqrt (rand ());
    }
}

int main ()
{
    printf("Parent process starts (pid = %d; ppid=%d)...\n",
                getpid(), getppid());

    pid_t retfork = fork();


    printf("process %d terminating\n", getpid());
    return 0;
}
