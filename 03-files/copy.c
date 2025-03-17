#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



void handleError (char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
	exit(EXIT_FAILURE);
}


void handleSystemError (char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]) 
{
    int           fsrc;
    int           fdst;
    unsigned long bufferDim;
    char *        buf;
    
    // validar argumentos do programa
    // fcopy <src file> <dst file> <dim buffer>
    if (argc != 4) 
       handleError("Use: fcopy <src file> <dst file> <dim buffer>");

    printf("Source     : %s\n", argv[1]);
    printf("Destination: %s\n", argv[2]);
    printf("Buffer     : %s\n", argv[3]); 
        
    bufferDim = atol(argv[3]);
    if ( bufferDim <= 0 ) handleError("Bad buffer size");
    
    buf = malloc(bufferDim);
    if ( buf == NULL) handleError("Buffer allocation error");
    
    
    if ( (fsrc = open(argv[1], O_RDONLY)) < 0 ) 
        handleSystemError("Open source File");

    if ( (fdst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0 ) 
        handleSystemError("Open destination File");

    int cnt = 0;
    
    size_t nBytesRd;
    while ( (nBytesRd = read(fsrc, buf, bufferDim)) > 0 ) {
		size_t nBytesWr = write(fdst, buf, nBytesRd);
        if(nBytesWr < 0) handleSystemError("Write Error");
		if (nBytesWr < nBytesRd) printf("Write didn't write everything\n");
		cnt++;    
	}
	
	printf("Read calls %d\n", cnt + 1);
	
	if ( nBytesRd < 0 ) handleSystemError("Read error"); 
	
	if ( close(fsrc) < 0) handleSystemError("Closing source file"); 
	if ( close(fdst) < 0) handleSystemError("Closing destination file");
	
	return 0;
}
