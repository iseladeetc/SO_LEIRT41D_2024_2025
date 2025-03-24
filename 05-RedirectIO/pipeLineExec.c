#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PIPE_RD 0
#define PIPE_WR 1


int main ()
{
    printf("Pipe line executing 2 commands example (pid=%d): cat /etc/passwd | wc\n", getpid());

    
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("Calling pipe");
        exit(EXIT_FAILURE);   
    }

    pid_t retfork1 = fork();

    if (retfork1 == -1) {
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    if (retfork1 == 0) {  // ------ 1º processo filho ------
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
        execlp("cat", "cat", "/etc/passwd", NULL);
        perror("calling exec cat");
        exit(EXIT_FAILURE);
    }

    // Continuação do processo pai ...
    pid_t retfork2 = fork();

    if (retfork2 == -1) {
        perror("Calling fork");
        exit(EXIT_FAILURE);
    }

    if (retfork2 == 0) {  // ------ 2º processo filho ------
        // novo processo
        printf("Child Process with pid = %d; ppid = %d\n", getpid(), getppid());

        // duplicar descritor fd no descritor stdout
		dup2(pipefd[PIPE_RD], STDIN_FILENO);

        if ( close(pipefd[PIPE_WR]) < 0) {
            perror("Closing pipe write descriptor");
            exit(EXIT_FAILURE);           
        }

        if ( close(pipefd[PIPE_RD]) < 0) {
            perror("Closing pipe read descriptor");
            exit(EXIT_FAILURE);           
        }

        // executar um programa 
        execlp("wc", "wc", NULL);
        perror("calling exec wc");
        exit(EXIT_FAILURE);
    }
    
    // Continuação do processo pai ...
    
    if ( close(pipefd[PIPE_WR]) < 0) {
        perror("Closing pipe write descriptor");
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

    // wait for 1st child
    pid_t pid;
    pid = waitpid(retfork1, &status, 0);
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

    // wait for 2nd child
    pid = waitpid(retfork2, &status, 0);
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
