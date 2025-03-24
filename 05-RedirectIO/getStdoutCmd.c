#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define PIPE_RD 0
#define PIPE_WR 1

#define MAX_BUF 16


int main ()
{
    printf("Execute a command and get its stdout example (pid=%d)\n", getpid());

        
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("Calling pipe");
        exit(EXIT_FAILURE);   
    }

    pid_t retfork = fork();

    if (retfork == -1) {
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    if (retfork == 0) {
        // novo processo
        printf("Child Process with pid = %d; ppid = %d\n", getpid(), getppid());


        // duplicar descritor fd no descritor stdout
		dup2(pipefd[PIPE_WR], STDOUT_FILENO);

        if ( close(pipefd[PIPE_WR]) < 0) {
            perror("Closing pipe write descriptor");
            exit(EXIT_FAILURE);           
        }

        if ( close(pipefd[PIPE_RD]) < 0) {
            perror("Closing pipe read descriptor");
            exit(EXIT_FAILURE);           
        }

        // executar um programa 
        execlp("ls", "ls", "-l", NULL);
        perror("calling exec");
        exit(EXIT_FAILURE);
    }

    // processo pai
    printf("My pid = %d and create a process %d\n", getpid(), retfork);

    if ( close(pipefd[PIPE_WR]) < 0) {
        perror("Closing pipe write descriptor");
        exit(EXIT_FAILURE);           
    }

    char buf[MAX_BUF + 1];
    int nBytesRd;

    // get stdout of child
    while ( (nBytesRd = read(pipefd[PIPE_RD], buf, MAX_BUF)) > 0) {
        buf[nBytesRd] = '\0';
        printf("receive: %s\n", buf);
    }

    if (nBytesRd < 0) {
        perror("Reading from pipe");
        exit(EXIT_FAILURE);
    }    

    if ( close(pipefd[PIPE_RD]) < 0) {
        perror("Closing pipe read descriptor");
        exit(EXIT_FAILURE);           
    }
    

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
