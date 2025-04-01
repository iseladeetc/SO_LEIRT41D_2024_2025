#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>


#include "erroraux.h"
#include "common.h"


#define MAX_BUFFER  128


int main(int argc, char * argv[]) 
{
    unsigned int       serverPort = DEFAULT_PORT;

    printf("Programa de exemplo de sockets TCP (server) ...\n\n");
    
    if (argc == 2) {
        serverPort = atoi(argv[1]);
    }
    else if (argc != 1) {
        printf("Argumentos invalidos.\nUse: %s <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);	 
    } 

    if ( (serverPort < 1) || (serverPort > 65536) ) {
        printf("O porto deve estar entre 1..65536\n");
        exit(EXIT_FAILURE);	 
    }
    
    printf("O Servidor vai registar-se no porto %d\n", serverPort);
    

    
    // necessário para no caso de erro na escrita ser devolvido erro em
    // vez de o processo ser parado sevido à recepção do sinal SIGPIPE
    signal(SIGPIPE, SIG_IGN);

   
    // Criar sockect TCP
    int  sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        fatalErrorSystem("Erro ao pedir o descritor");


    // Registar endereco local de modo a que os clientes nos possam contactar 
    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(serverPort);

    if ( bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
        fatalErrorSystem("Erro ao efectuar o bind");


    // Activar socket com fila de espera de dimensao 5
    if (listen( sockfd, 5) < 0 )
        fatalErrorSystem("Erro no listen");


    for (int run = 1; run; ) {
        printf("Espero ligacao...\n");

        // Espera Contacto de um cliente
        struct sockaddr_in client_addr;
        socklen_t dim_client = sizeof(client_addr);
        int newSockfd = accept(sockfd, (struct sockaddr *)(&client_addr), &dim_client);
        if ( newSockfd < 0 )
            fatalErrorSystem("Erro ao efectuar o accept");
        
        printf("Estabeleci uma ligacao...\n");
         
        int  n_bytes;
        char buffer[MAX_BUFFER + 1]; // reserva um byte extra para o char de terminação

        while ( (n_bytes = read(newSockfd, buffer, MAX_BUFFER)) > 0 ) {
            
            buffer[n_bytes] = '\0'; // Forca a terminacao a string
            printf("(%d) Recebi -> %s\n", getpid(), buffer );
            

            if (strcmp(buffer, "..\n") == 0) { // terminar ligação
                run = 0;
                break;
            }

            if ( write(newSockfd, buffer, n_bytes) < 0 )
                fatalErrorSystem("Erro ao escrever no socket");

        }
        
        if ( n_bytes < 0 ) fatalErrorSystem("Erro ao ler do socket");

        close(newSockfd);
    }

    close(sockfd);

    return 0;
}
