#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "erroraux.h"
#include "common.h"


#define MAX_BUFFER  128


int main(int argc, char * argv[]) 
{
    char  *serverName = DEFAULT_UNIX_SOCKET;

    printf("Programa de exemplo de sockets Domínio Unix (cliente) ...\n\n");

    if (argc == 2) {
        serverName = argv[1];
    }

  
    // necessário para no caso de erro na escrita ser devolvido erro em
    // vez de o processo ser parado sevido à recepção do sinal SIGPIPE
    signal(SIGPIPE, SIG_IGN);



    // Abrir um socket TCP (an Internet Stream socket)
    int sockfd;
    if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao pedir o descritor");
    }
        
  
    // Preencher a estrutura serv_addr com o endereco do servidor que pretendemos contactar
    struct sockaddr_un serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family      = AF_UNIX;
    strcpy(serv_addr.sun_path, serverName);
    
    printf("O cliente vai ligar-se ao servidor no socket UNIX %s\n", serverName);
        
    // Ligar-se ao servidor
    if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
        fatalErrorSystem("Falha no connect");
    }
    
    printf("Ligacao estabelecida...\n");

    char               buffer[MAX_BUFFER+1]; /* reserva um byte extra para o char de terminacao */
    int                n_bytes;

    while ( fgets(buffer, sizeof(buffer), stdin) != NULL ) {
    	
    	if ( strcmp(buffer, ".\n") == 0 ) break;
    	
        if ( write(sockfd, buffer, strlen(buffer)) < 0 ) {
            fatalErrorSystem("Erro ao escrever no socket");
        }
        
        if ( (n_bytes = read(sockfd, buffer, MAX_BUFFER)) < 0 ) {
            fatalErrorSystem("Erro ao ler do socket");
        }

        if ( n_bytes == 0) {
        	printf("Servidor fechou a ligação.\n");
        	break;
        }

        buffer[n_bytes] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    
    close(sockfd);
    
    return EXIT_SUCCESS;
}
