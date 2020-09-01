/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define QUEUE_LENGTH 5      //Tamanho maximo da fila de conexoes de clientes
#define MAX_FLOW_SIZE 1000  //Tamanho maximo do buffer de caracteres
#define true 1

int main(int argc, char *argv[]) {
	int sockId, recvBytes, bindRet, getRet, sentBytes, connId, serverPort;
	unsigned int servLen, cliLen;
	char buf[MAX_FLOW_SIZE];
	struct sockaddr_in server, client;

/*
 *******************************************************************************
               Abrindo um socket do tipo stream (TCP)
 *******************************************************************************
 */
	sockId = socket(AF_INET, SOCK_STREAM, 0);
	if (sockId < 0) {
		printf("Stream socket nao pode ser aberto\n");
		return(1);
	}
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando os atributos da estrutura do socket
 *******************************************************************************
 */
	server.sin_family	= AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	if (argc == 2) {
		serverPort	= atoi(argv[1]);
		if (serverPort <= 0) {
			printf("Porta Invalida: %s\n", argv[1]);
			close(sockId);
			return(1);
		}
		else server.sin_port	= htons(serverPort);
	}
	else server.sin_port = 0;
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Fazendo o bind para o stream socket aberto
 *******************************************************************************
 */
	bindRet = bind(sockId, (struct sockaddr *)&server, sizeof(server));
	if (bindRet < 0) {
		printf("O bind para o stream socket falhou\n");
		close(sockId);
		return(1);
	}
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Obtendo o nome do socket
 *******************************************************************************
 */
	servLen = sizeof(server);
	getRet	= getsockname(sockId, (struct sockaddr *)&server, &servLen);
	if (getRet < 0) {
		printf("Nao foi possivel obter o nome do socket\n");
		close(sockId);
		return(1);
	}
/*----------------------------------------------------------------------------*/

	printf("Porta do servidor: %d\n",ntohs(server.sin_port));

/*
 *******************************************************************************
               Colocando o servidor em modo listening
 *******************************************************************************
 */
	listen(sockId, QUEUE_LENGTH);
/*----------------------------------------------------------------------------*/

	do {

/*
 *******************************************************************************
               Habilitando o servidor de receber conexoes
 *******************************************************************************
 */
		cliLen = sizeof(client);
                connId = accept(sockId, (struct sockaddr *)&client, &cliLen);
		if (connId < 0)
			printf("O socket nao pode aceitar conexoes\n");
/*----------------------------------------------------------------------------*/

		else do {

/*
 *******************************************************************************
               Recebendo as mensagens do cliente
 *******************************************************************************
 */
				memset(buf, 0, sizeof(buf));
				recvBytes = recv(connId, buf, MAX_FLOW_SIZE, 0);
				if (recvBytes <= 0) {
					if (recvBytes < 0)
						printf("Ocorreu um erro na aplicacao\n");
					else
						printf("Encerrando a conexao do cliente\n");
				}
/*----------------------------------------------------------------------------*/

				else {

/*
 *******************************************************************************
               Enviando as mensagens para o cliente
 *******************************************************************************
 */
					sentBytes = send(connId, buf, strlen(buf), 0);
					if (sentBytes < 0) {
						printf("A conexao foi perdida\n");
						recvBytes = 0;
					}
					else {
						printf("Mensagem enviada: [%s]\n",buf);
					}
/*----------------------------------------------------------------------------*/

				}
			} while (recvBytes > 0);
		close(connId);
	} while (true);
	close(sockId);
	return(0);
}
