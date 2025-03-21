#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
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
    if (retfork == -1) {
        perror("Creating Process");
        exit(EXIT_FAILURE);
    }
    if ( retfork == 0) {
        // novo processo
        printf("Child Process with pid = %d; ppid = %d\n", getpid(), getppid());

        // Se apenas o filho executar o process_work o pai termina primeiro
        // o que acontece ao processo filho?
        process_work(DEFAULT_ITER);
        
        printf("Child Process terminating with pid = %d; ppid = %d\n", 
                    getpid(), getppid());
        return 13;
    }

    // Processo ? -> processo pai
    printf("My pid = %d e create a process %d\n", getpid(), retfork);

    // Se apenas o pai executar o process_work o filho termina primeiro
    // o que acontece ao processo filho?
    //process_work(DEFAULT_ITER);

    int status;
    pid_t pid = wait(&status);
    if (pid == -1) {
        perror("calling wait");
        exit(EXIT_FAILURE);
    }
    printf("Process %d", pid);
    if (WIFEXITED(status)) {
        printf(" has terminated normally with exit value %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        printf(" has terminated by signal %d\n", WTERMSIG(status));
    }


    printf("process %d terminating\n", getpid());

    return 0;
}
