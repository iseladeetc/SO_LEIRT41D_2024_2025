#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "erroraux.h"
#include "common.h"


#define MAX_BUFFER  128


int main(int argc, char * argv[]) 
{
    char              *serverName = DEFAULT_HOST;
    unsigned int       serverPort = DEFAULT_PORT;
      
    printf("Programa de exemplo de sockets TCP (cliente) ...\n\n");
    
    if (argc == 3) {
        serverName = argv[1];
        serverPort = atoi(argv[2]);
    }
    else if (argc != 1) {
        printf("Argumentos invalidos.\nUse: %s <host> <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);	 
    } 

  
    // necessário para no caso de erro na escrita ser devolvido erro em
    // vez de o processo ser parado sevido à recepção do sinal SIGPIPE
    signal(SIGPIPE, SIG_IGN);


    // Determina endereco IP do servidor
    struct hostent *phe;
    in_addr_t       serverAddress;

    if ((phe = gethostbyname(serverName)) != NULL) {
        memcpy(&serverAddress, phe->h_addr_list[0], phe->h_length);
    } 
    else if ( (serverAddress = inet_addr(serverName)) == -1) {
        fatalErrorSystem("Impossivel determinar endereco IP da maquina");
    }
        
    if ( (serverPort < 1) || (serverPort > 65536) ) {
        printf("O porto deve estar entre 1..65536\n");
        exit(EXIT_FAILURE);	 
    }
  

    // Abrir um socket TCP (an Internet Stream socket)
    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fatalErrorSystem("Erro ao pedir o descritor");
    }
        
  
    // Preencher a estrutura serv_addr com o endereco do servidor que pretendemos contactar
    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = serverAddress;
    serv_addr.sin_port        = htons(serverPort);
    
    printf("O cliente vai ligar-se ao servidor na maquina %s:%d\n", serverName, serverPort);
    printf("IP: %s\n", inet_ntoa(serv_addr.sin_addr));
        
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
