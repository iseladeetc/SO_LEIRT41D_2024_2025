#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define PIPE_RD 0
#define PIPE_WR 1

#define MAX_BUF 6

int main ()
{
    printf("Parent process starts (pid = %d; ppid=%d)...\n",
                getpid(), getppid());

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

        printf("receiving from parent...\n");

        char buf[MAX_BUF + 1];
        int nBytesRd;

        if ( close(pipefd[PIPE_WR]) < 0) {
            perror("Closing pipe write descriptor");
            exit(EXIT_FAILURE);           
        }
        
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

        printf("Child Process terminating width pid = %d; ppid = %d\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
    }

    
    char msg2send[] = "The quick brown fox jumps over the lazy dog";
    int  msg2sendsz = strlen(msg2send);

    if ( close(pipefd[PIPE_RD]) < 0) {
        perror("Closing pipe read descriptor");
        exit(EXIT_FAILURE);            
    }

    int nBytesWr = write(pipefd[PIPE_WR], msg2send, msg2sendsz);

    if ( nBytesWr < 0) {
        perror("Writing to pipe");
        exit(EXIT_FAILURE);   
    }

    if (nBytesWr != msg2sendsz) {
        printf("Write only send %d\n", nBytesWr);
    }

    if ( close(pipefd[PIPE_WR]) < 0) {
        perror("Closing pipe write descriptor");
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
