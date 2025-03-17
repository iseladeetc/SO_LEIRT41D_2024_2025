/*
 * Para testar pode criar um ficheiro de 10 Mbytes através do comando:
 *    dd if=/dev/zero bs=1M count=10 of=fileSrc
 * 
 * Pode executar o programa indicando o ficheiro fonte e a dimensão 
 * do buffer a ser utilizado. A obtenção do tempo de execução pode 
 * ser feita usando o comando time:
 *    time ./fcopy fileSrc fileDst 128
 * 
 * Para determinar o número de chamadas de sistemas realizadas durante 
 * a execução de um programa use o seguinte comando:
 *    strace ./fcopy fileSrc fileDst 128
 * 
 * Realize vários ensaios variando a dimensão do buffer utilizado.
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
