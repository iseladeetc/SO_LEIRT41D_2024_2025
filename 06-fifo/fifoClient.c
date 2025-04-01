#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include "common.h"



int main ()
{
    int fdFifo = open(FIFO_NAME, O_WRONLY);
    if ( fdFifo == -1 ) {
       perror("Opening fifo");
       exit(EXIT_FAILURE);
    }

    printf("Sending message...\n");

    char msg2send[] = "The quick brown fox jumps over the lazy dog";
    int  msg2sendsz = strlen(msg2send);

    int nBytesWr = write(fdFifo, msg2send, msg2sendsz);

    if ( nBytesWr < 0) {
        perror("Writing to fifo");
        exit(EXIT_FAILURE);   
    }

    if (nBytesWr != msg2sendsz) {
        printf("Write only send %d\n", nBytesWr);
    }

    if (close(fdFifo) < 0) {
        perror("Closing fifo");
        exit(EXIT_FAILURE);           
    }

    return 0;
}