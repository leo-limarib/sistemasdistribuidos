#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "maps.h"
#include "server_setup.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
/*
	Padrão para as mensagens.
	As mensagens devem seguir o seguinte padrão.
	MOVE;x=1;y=2
	ou
	LOADMAP

	Delimitadores = ;
	E para declarar valor variavel=valor

	Comandos recebidos do client:
							MOVE:
							O comando MOVE serve para o player mover o seu personagem.
							Sintaxe: "MOVE;X=4;Y=4"

							LOADMAP:
							O comando LOADMAP serve para o servidor enviar o mapa inicial
							para o jogador. Quando recebido, o servidor enviará a mensagem
							"MAPSIZE;H=4;W=4" contendo o tamanho do mapa, após isso, enviará
							o mapa linha por linha, e quando acabar, enviará a mensagem "LOADED"
							Sintaxe: "LOADMAP"


	Comandos enviados para o client:
							MAPSIZE:
							O comando serve para o servidor informar o tamanho do mapa para
							o player, antes de enviá-lo.
							Sintaxe: "MAPSIZE;H=4;W=4"

							MAPCHUNK:
							O comando serve para enviar uma linha do mapa para o player.
							Sintaxe: "MAPCHUNK;00000000"

							MAPLOADED:
							Serve para informar o player de que o mapa já foi todo enviado.
							Sintaxe: "MAPLOADED"

							SUCCESS:
							Significa que a mensagem que o client enviou para nós foi recebida
							com sucesso.

							INVALID:
							Significa que a mensagem que o client enviou contém um comando
							inválido.

*/

int main(int argc, char* argv[]) {
	if(argc == 2) {
		loadMap(argv[1]);
		printf("\nMapa:\n");
		renderMap();
	} else {
		printf("Erro: Sintaxe errada. \n Utilize: Main \"/caminho/ate/pastadosmapas/\"\n");
		return 0;
	}

	//Configura o servidor.
	ServerInfo info = setupServer();
	printf("Socket Id: %d\n", info.sockId);
	char buf[1000];

	//Variaveis que preciso do setupServer() para o loop.
	// sockId, server
	listen(info.sockId, QUEUE_LENGTH);
	printf("Porta do servidor: %d\n",ntohs(info.server.sin_port));

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
	close(info.sockId);
}
