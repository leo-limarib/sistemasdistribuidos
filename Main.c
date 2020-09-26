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

	Comandos enviandos pelo client:
							MOVE:
							O comando MOVE serve para o player mover o seu personagem.
							Sintaxe: "MOVE;X=4;Y=4"

							LOADMAP:
							O comando LOADMAP serve para o servidor enviar o mapa para
							o jogador.
							Sintaxe: LOADMAP


	Comandos enviados pelo server:
							MAP:
							Comando que envia o mapa todo em uma só mensagem (por isso temos
						  que estabelecer um limite no tamanho de mapa (32x32)).
							Sintaxe: MAP;4x4;1111;0000;0000;0000;END
											 COMANDO:TAMANHODOMAPA;LINHA1;LINHA2;....;ULTIMALINHA

							MOVED:
							Confirma que o comando MOVE do player foi bem sucedido. Também
							contém a nova posição do jogador.
							MOVED;X=4;Y=4

*/

int main(int argc, char* argv[]) {
	if(argc == 2) {
		Map map = loadMap(argv[1]);
		printf("\nMapa:\n");
		renderMap(map);

		//Configura o servidor.
		ServerInfo info = setupServer();
		printf("Socket Id: %d\n", info.sockId);
		connectToClient(info, map);
	} else {
		printf("Erro: Sintaxe errada. \n Utilize: Main \"/caminho/ate/pastadosmapas/\"\n");
		return 0;
	}
}
