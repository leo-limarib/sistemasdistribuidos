/*
 * 			ClienteMonoTCP.c
 *
 * Este programa cliente foi desenvolvido para conectar uma aplicacao servidora
 *
 * Funcao:     Enviar e receber streams compostos de caracteres
 * Plataforma: Linux (Unix), ou Windows com CygWin
 * Compilar:   gcc -Wall ClienteMonoTCP.c -o ClienteMonoTCP
 * Uso:        ./ClienteMonoTCP Endereco_IP_ou_nome_do_servidor porta_do_servidor
 *
 * Autor:      Jose Martins Junior
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define MAX_FLOW_SIZE 1000  //Tamanho maximo do buffer de caracteres
#define true 1

int main(int argc, char *argv[]) {

	int sockId, serverPort, sentBytes, recvBytes, connId;
	char buf[MAX_FLOW_SIZE];
	struct sockaddr_in client;
	struct hostent *hp;

/*
 *******************************************************************************
               Testando os argumentos passados na chamada
 *******************************************************************************
 */
	if (argc != 3) {
		printf("Incorreto... Usar: %s IP_ou_nome_do_servidor porta_do_servidor\n", argv[0]);
		return(1);
	}
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando e testando o endereco informado (primeiro argumento)
 *******************************************************************************
 */
	hp = gethostbyname(argv[1]);
	if (((char *) hp) == NULL) {
		printf("Host Invalido: %s\n", argv[1]);
		return(1);
	}
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Setando os atributos da estrutura do socket
 *******************************************************************************
 */
	memcpy((char*)&client.sin_addr, (char*)hp->h_addr, hp->h_length);
	client.sin_family = AF_INET;
	serverPort = atoi(argv[2]);
	if (serverPort <= 0) {
		printf("Porta Invalida: %s\n", argv[2]);
		return(1);
	}
	client.sin_port = htons(serverPort);
/*----------------------------------------------------------------------------*/

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
               Conectando o socket ao servidor
 *******************************************************************************
 */
	connId = connect(sockId, (struct sockaddr *)&client, sizeof(client));
	if (connId == -1) {
		printf("A conexao com o servidor %s na porta %s falhou\n", argv[1], argv[2]);
		close(sockId);
		return(1);
	}
/*----------------------------------------------------------------------------*/

	while(true) {
		printf("Digite a mensagem (END para encerrar): ");
		scanf("%s",buf);

		printf("Mensagem digitada: [%s]\n",buf);
		printf("Tamanho do buffer: %zu\n",strlen(buf));

/*
 *******************************************************************************
               Condicao de parada (termino normal)
 *******************************************************************************
 */
		if (!strcmp(buf,"END")) {
			close(sockId);
			printf("Encerrado\n");
			return(0);
		}
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Enviando as mensagens para o servidor
 *******************************************************************************
 */
		sentBytes = send(sockId, buf, strlen(buf), 0);
		if (sentBytes < 0) {
			close(sockId);
			printf("A conexao foi perdida\n");
			return(1);
		}
		memset(buf,0,sizeof(buf));
/*----------------------------------------------------------------------------*/

/*
 *******************************************************************************
               Recebendo as mensagens do servidor
 *******************************************************************************
 */
		recvBytes = recv(sockId, buf, MAX_FLOW_SIZE, 0);
		if (recvBytes < 0) {
			close(sockId);
			printf("Ocorreu um erro na aplicacao\n");
			return(1);
		}
		if (recvBytes == 0) {
			close(sockId);
			printf("A conexao foi encerrada pelo servidor\n");
			return(1);
		}
		printf("O servidor retornou %zu caracteres: [%s]\n", strlen(buf), buf);
		memset(buf,0,sizeof(buf));
/*----------------------------------------------------------------------------*/

	}
}

