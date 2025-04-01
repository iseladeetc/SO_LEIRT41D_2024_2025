#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "common.h"

#define RD_BLOCK_SIZE   64


int main ()
{
    printf("Fifo server starts...\n");

    
    int result = mkfifo(FIFO_NAME, 0644);
    if (result == -1) {
       perror("Erro na criacao do fifo");
       return -1;
    }

    int fdFifo = open(FIFO_NAME, O_RDONLY);
    if ( fdFifo == -1 ) {
       perror("Erro na abertura do fifo");
       return -1;
    }
    

    printf("receiving from client...\n");

    char buf[RD_BLOCK_SIZE + 1];
    int nBytesRd;

    
    while ( (nBytesRd = read(fdFifo, buf, RD_BLOCK_SIZE)) > 0) {
        buf[nBytesRd] = '\0';
        printf("receive: %s\n", buf);
    }

    if (nBytesRd < 0) {
        perror("Reading from pipe");
        exit(EXIT_FAILURE);
    }

    if (close(fdFifo) < 0) {
        perror("Closing fifo");
        exit(EXIT_FAILURE);           
    }


    printf("removing fifo file...\n");
    if (unlink(FIFO_NAME) < 0) {
        perror("removing fifo");
        exit(EXIT_FAILURE);           
    }   
 
    return EXIT_SUCCESS;
}
