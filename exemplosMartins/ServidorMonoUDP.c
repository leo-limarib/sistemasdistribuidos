/*
 * 			ServidorMonoUDP.c
 *
 * Este programa servidor foi desenvolvido para receber mensagens de uma aplicacao cliente UDP
 *
 * Funcao:     Enviar e receber mensagens compostas de caracteres
 * Plataforma: Linux (Unix), ou Windows com CygWin
 * Compilar:   gcc -Wall ServidorMonoUDP.c -o ServidorMonoUDP
 * Uso:        ./ServidorMonoUDP
 *
 * Autor:      Jose Martins Junior
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>

#define SIZE 100            //Tamanho maximo do buffer de caracteres
#define SERVER_PORT 4567    //Porta do servidor
#define true 1

int main(int argc, char *argv[]) {
    int sockId, recvBytes;
    unsigned int servLen;
    struct sockaddr_in server;
    char buf[SIZE];

/*
 *******************************************************************************
               Abrindo um socket do tipo datagram (UDP)
 *******************************************************************************
 */
    if ((sockId = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       printf("Datagram socket nao pode ser aberto\n");
       return(1);
    }
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando os atributos da estrutura do socket
 *******************************************************************************
 */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Fazendo o bind para o datagram socket aberto
 *******************************************************************************
 */
    if (bind(sockId, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("O bind para o datagram socket falhou\n");
		close(sockId);
		return(1);
    }
/*----------------------------------------------------------------------------*/

    servLen = sizeof(server);

    while(true) {
/*
 *******************************************************************************
               Recebendo e retornando as mensagens do cliente
 *******************************************************************************
 */
        recvBytes = recvfrom(sockId, buf, SIZE, 0, (struct sockaddr *)&server, &servLen);
        printf("Recebeu %d caracteres: %s\n",recvBytes,buf);
        sendto(sockId, buf, sizeof(buf), 0, (struct sockaddr *)&server, servLen);
/*----------------------------------------------------------------------------*/
    }
    close(sockId);
    return(0);
}

