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
#define MAP_HEADER_SIZE 16			//Tamanho fixo do cabeçalho da mensagem MAP
#define MAX_ATTEMPTS 50

int sockId, recvBytes, bindRet, getRet, sentBytes, connId, serverPort;
unsigned int servLen, cliLen;
char buf[MAX_FLOW_SIZE];
struct sockaddr_in server;

typedef struct {
	struct sockaddr_in server, client;
	int sockId, state;
} ServerInfo;

ServerInfo info;

/*
//Variaveis que preciso do setupServer() para o loop.
// sockId, server
listen(info.sockId, QUEUE_LENGTH);
printf("Porta do servidor: %d\n",ntohs(info.server.sin_port));
char buf[1200];

do {
	//Habilita o servidor para receber conexões.
	cliLen = sizeof(info.client);
							connId = accept(info.sockId, (struct sockaddr *)&info.client, &cliLen);
	if (connId < 0)
		printf("O socket nao pode aceitar conexoes\n");
	else do { // --
			//Recebe mensagens do cliente.
			memset(buf, 0, sizeof(buf));
			recvBytes = recv(connId, buf, MAX_FLOW_SIZE, 0);
			if (recvBytes <= 0) {
				if (recvBytes < 0)
					printf("Ocorreu um erro na aplicacao\n");
				else
					printf("Encerrando a conexao do cliente\n");
			}
			else {
				//Envia mensagens para o cliente.
				sentBytes = send(connId, buf, strlen(buf), 0);
				if (sentBytes < 0) {
					printf("A conexao foi perdida\n");
					recvBytes = 0;
				}
				else {
					printf("Mensagem enviada: [%s]\n",buf);
				}
			}
		} while (recvBytes > 0); // --
	close(connId);	//FECHA A PORRA DA CONEXÃO
} while (true);
close(info.sockId);]
*/

char* mapToString(Map map) {
	int msgSize = (map.height*map.width) + map.height-1;
	msgSize += MAP_HEADER_SIZE;
	char* msg = malloc(msgSize * sizeof(char));
	sprintf(msg, "MAP;%dx%d", map.height, map.width);

	for(int i=0;i<map.height;i++) {
		char* line = malloc((map.width + 1) * sizeof(char));
		sprintf(line, ";%s", map.tiles[i]);
		strtok(line, "\n");
		strcat(msg, line);
	}
	return msg;
}

/*
	Realiza a conexão com o cliente, envia o mapa e começa a "partida".
*/
void connectToClient(ServerInfo connInfo, Map map) {
	//Testing
	mapToString(map);

	//Começa a ouvir na porta info.server.sin_port.
	listen(info.sockId, QUEUE_LENGTH);
	printf("Porta do servidor: %d\n",ntohs(info.server.sin_port));
	char buf[1200];
	int attempts = 0;

	do {
		//Habilita o servidor para receber conexões.
		cliLen = sizeof(info.client);
		connId = accept(info.sockId, (struct sockaddr *)&info.client, &cliLen);
		if (connId < 0)
			printf("O socket nao pode aceitar conexoes\n");
		else do { // --
				//Recebe mensagens do cliente.
				memset(buf, 0, sizeof(buf));
				recvBytes = recv(connId, buf, MAX_FLOW_SIZE, 0);
				if (recvBytes <= 0) {
					if (recvBytes < 0)
						printf("Ocorreu um erro na aplicacao\n");
					else
						printf("Encerrando a conexao do cliente\n");
				}
				else {
					// ------- Lista de comandos sem parâmetros ------- //
					if(strcmp(buf, "LOADMAP") == 0) {
						//PARSEAR O MAPA PARA STRING E ENVIÁ-LO AO CLIENT
						char *mapStr = mapToString(map);
						sentBytes = send(connId, mapStr, strlen(mapStr), 0);
					}  else {
						//Dar split no char ";", se reconhecermos o comando, faz, senão
						//o comando é inválido.
						char* command = strtok(buf, ";");

						//Comandos que possuem parâmetros
						if(strcmp(command, "MOVE") == 0) {
							//Tentar mover, se funcionar, retornar SUCCESS;X;Y, se não, FAIL:100
							//para quando o movimento foi impedido por uma parede ou limite do
							//mapa, ou retornar FAIL:200 se o número de tentativas máx foi
							//alcançado
							char *x = strtok(NULL, ";");
							char *y = strtok(NULL, ";");

							if((x != NULL) && (y != NULL) && (attempts < MAX_ATTEMPTS)) {
								map = movePlayer(map, atoi(x), atoi(y));
								attempts += 1;
								if(map.ended == 0) {
									sentBytes = send(connId, "SUCCESS", 8, 0);
								} else {
									sentBytes = send(connId, "COMPLETED", 10, 0);
									recvBytes = 0;
								}
							} else {
								sentBytes = send(connId, "ERROR:404", 10, 0);
							}
						} else {
							sentBytes = send(connId, "ERROR:404", 9, 0);
						}
					}
					// -------------------------------- //
				}
			} while (recvBytes > 0); // --
		close(connId);	//FECHA A PORRA DA CONEXÃO
	} while (true);
	close(info.sockId);
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
