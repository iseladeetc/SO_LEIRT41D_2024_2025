/*
 * Ver ficheiro lab.txt com indicações de ensaios
 */
#include <stdio.h>
#include <stdlib.h>


void handleError (char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
	exit(EXIT_FAILURE);
}


int main (int argc, char *argv[]) 
{
    FILE *        fsrc;
    FILE *        fdst;
    unsigned long bufferDim;
    char *        buf;
    
    // validar argumentos do programa
    // fcopy <src file> <dst file> <dim buffer>
    if (argc != 4) 
    handleError("Use: fcopy <src file> <dst file> <dim buffer>");

    printf("Source file     : %s\n", argv[1]);
    printf("Destination file: %s\n", argv[2]);
    printf("Buffer size     : %s\n", argv[3]); 
        
    bufferDim = atol(argv[3]);
    if ( bufferDim <= 0 ) handleError("Bad buffer size");
    
    buf = malloc(bufferDim);
    if ( buf == NULL ) handleError("Buffer allocation error");
    
    
    if ( (fsrc = fopen(argv[1], "r")) == NULL) handleError("Open source File");

    if ( (fdst = fopen(argv[2], "w")) == NULL) handleError("Open destination File");

    size_t nBytesRd;

    while ( (nBytesRd = fread(buf, 1, bufferDim, fsrc)) > 0 ) {
		size_t nBytesWr = fwrite(buf, 1, nBytesRd, fdst);
		if (nBytesWr < nBytesRd) handleError("Write error");    
	}
	
	if ( ferror(fsrc) ) handleError("Read error"); 
	
	fclose(fsrc); 
	fclose(fdst);
    
	return 0;
}
