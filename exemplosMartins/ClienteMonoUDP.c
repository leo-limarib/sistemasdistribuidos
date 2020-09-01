/*
 * 			ClienteMonoUDP.c
 *
 * Este programa cliente foi desenvolvido para enviar mensagens para uma aplicacao servidora UDP
 *
 * Funcao:     Enviar e receber mensagens compostas de caracteres
 * Plataforma: Linux (Unix), ou Windows com CygWin
 * Compilar:   gcc -Wall ClienteMonoUDP.c -o ClienteMonoUDP
 * Uso:        ./ClienteMonoUDP Endereco_do_servidor_ou_broadcast
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
    int sockId, servLen;
    int on = 1;
    struct sockaddr_in client, server;
    char buf[SIZE];
    struct hostent *hp;

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
               Definindo opcao de envio broadcast no datagram socket
 *******************************************************************************
 */
    if(setsockopt(sockId, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on)) < 0) {
       printf("Opcao de broadcast nao pode ser atribuida ao datagram socket\n");
       close(sockId);
       return(1);
    }
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando os atributos da estrutura de socket servidora
 *******************************************************************************
 */
    if (argc < 2) {
       server.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    } else {
       hp = gethostbyname(argv[1]);
       if (((char *) hp) == NULL) {
           printf("Host Invalido: %s\n", argv[1]);
           return(1);
       }
       else memcpy((char*)&server.sin_addr, (char*)hp->h_addr, hp->h_length);
    }
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando os atributos da estrutura de socket cliente
 *******************************************************************************
 */
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(0);
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Fazendo o bind para o datagram socket aberto
 *******************************************************************************
 */
    if (bind(sockId, (struct sockaddr *)&client, sizeof(client)) < 0) {
		printf("O bind para o datagram socket falhou\n");
		close(sockId);
		return(1);
    }
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Enviando e recebendo as mensagens do servidor
 *******************************************************************************
 */
    printf("Digite a mensagem: ");
    scanf("%s",buf);
    servLen = sizeof(server);
    sendto(sockId, buf, sizeof(buf), 0, (struct sockaddr *)&server, servLen);
//    recvfrom(sockId, buf, SIZE, 0, (struct sockaddr *)&server, &servLen);
//    printf("Mensagem retornada: %s\n", buf);

/*----------------------------------------------------------------------------*/
    close(sockId);
    return(0);
}

