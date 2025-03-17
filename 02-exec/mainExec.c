#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



int main ()
{
    printf("Parent process starts (pid = %d; ppid=%d)...\n",
                getpid(), getppid());

    pid_t retfork = fork();

    if (retfork == -1) {
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    if (retfork == 0) {
        // novo processo
        printf("Child Process with pid = %d; ppid = %d\n", getpid(), getppid());

        // executar um programa
        //execlp("ls", "ls", "-l", NULL);
        execlp("geany", "geany", NULL);
        perror("calling exec");
        exit(EXIT_FAILURE);
    }

    // processo pai
    printf("My pid = %d and create a process %d\n", getpid(), retfork);


    // Sincronizar com a terminação do processo filho obtendo:
    //     1 - a forma como terminou o processo filho
    //     2 - valor de terminação
    int status;
    pid_t pid = waitpid(retfork, &status, 0);
    if (pid == -1) {
        perror("Calling waitpid");
        exit(EXIT_FAILURE);
    }

    printf("Process pid = %d", pid);
    if ( WIFEXITED(status) ) {
        printf(" has terminated normally with exit value %d", WEXITSTATUS(status));
    }
    else if ( WIFSIGNALED(status) ) {
        printf(" has terminated by signal %d", WTERMSIG(status));
    }
    printf("\n");

    printf("process %d terminating\n", getpid());
    return 0;
}
