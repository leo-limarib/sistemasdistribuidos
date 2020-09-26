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
#define MAX_FLOW_SIZE 1024  //Tamanho maximo do buffer de caracteres

int sockId, recvBytes, bindRet, getRet, sentBytes, connId, serverPort;
unsigned int servLen, cliLen;
char buf[MAX_FLOW_SIZE];
struct sockaddr_in server;

typedef struct {
	struct sockaddr_in server, client;
	int sockId, state;
} ServerInfo;

ServerInfo info;

void connectToClient(ServerInfo connInfo, Map map) {
	printf("%d", map.height);
}

ServerInfo setupServer() {
	// Abre um socket do tipo stream (TCP)
	sockId = socket(AF_INET, SOCK_STREAM, 0);
	if (sockId < 0) {
		printf("Stream socket nao pode ser aberto\n");
		info.state = 0;
		return info;
	}

	//Setando os atributos da estrutura do socket.
	server.sin_family	= AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port	= htons(serverPort);

	//Fazendo o bind para o stream socket aberto.
	bindRet = bind(sockId, (struct sockaddr *)&server, sizeof(server));
	if (bindRet < 0) {
		printf("O bind para o stream socket falhou\n");
		close(sockId);
		info.state = 0;
		return info;
	}

	//Obtendo o nome do socket.
	servLen = sizeof(server);
	getRet	= getsockname(sockId, (struct sockaddr *)&server, &servLen);
	if (getRet < 0) {
		printf("Nao foi possivel obter o nome do socket\n");
		close(sockId);
		info.state = 0;
		return info;
	}

	info.server = server;
	info.sockId = sockId;
	info.state = 1;
	return info;
}
