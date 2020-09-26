#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cores no console.
// \033[0;31m -> red
// \033[0;32m -> green
// \033[0;33m -> yellow

typedef struct {
	int height, width, ended;
	char **tiles;
	int* playerPos;
} Map;

void renderMap(Map map);
Map loadMap(char *mapFilename);
int* getPlayerPosInMap(Map map);
Map movePlayer(Map map, int x, int y);

/************************************************
 *  Função responsável por renderizar o mapa no console.
 *  @param map.tiles Array com os tiles do mapa.
 *  @param height Altura do mapa.
 *  @param width Largura do mapa
 ***********************************************/
void renderMap(Map map) {
	for(int i=0;i<map.height;i++) {
		for(int j=0;j<map.width;j++) {
      if(map.tiles[i][j] == '1') {
        printf("\033[0;31m"); //Set the text to the color red
        printf("%c", map.tiles[i][j]);
        printf("\033[0m");
      }
      else if (map.tiles[i][j] == '2') {
        printf("\033[01;33m"); //Set the text to the color red
        printf("%c", map.tiles[i][j]);
        printf("\033[0m");
      }
      else if (map.tiles[i][j] == '3') {
        printf("\033[01;36m"); //Set the text to the color red
        printf("%c", map.tiles[i][j]);
        printf("\033[0m");
      }
			else if (map.tiles[i][j] == '4') {
        printf("\033[0;32m"); //Set the text to the color red
        printf("%c", map.tiles[i][j]);
        printf("\033[0m");
      }
      else printf("%c", map.tiles[i][j]);
    }
    printf("\n");
	}
}

/************************************************
 *  Função responsável por carregar um mapa.
 *  @param mapFilename String contendo o caminho até o arquivo .map.
 *  @return Uma struct Map com height, width e **tiles.
 ***********************************************/
Map loadMap(char *mapFilename) {
	printf("Loading map %s\n", mapFilename);
	Map map;
	FILE *mapFile = fopen(mapFilename, "r");

	//Get the map size
	fscanf(mapFile, "%d", &map.height);
	fscanf(mapFile, "%d", &map.width);
	map.tiles = malloc(map.height * sizeof(char*));

	if(mapFile != NULL) {
		char line[map.width+2];
		for(int i=-1;fgets(line, sizeof(line), mapFile);i+=1) {
			map.tiles[i] = malloc(sizeof(line) + 1);
			strcpy(map.tiles[i], line);
		}
		map.playerPos = getPlayerPosInMap(map);
		map.ended = 0;
		return map;
	} else {
		printf("Impossível abrir o mapa especificado.");
	}
}

/************************************************
 *  Função responsável por pegar a posição do player nos tiles.
 *  @param tiles Array 2d contendo as informações dos tiles.
 *  @param height A altura do mapa
 *  @param width A largura do mapa
 *  @return Retorna um array[2] com a posição [0] = x e [1] = y do player.
 ***********************************************/
int* getPlayerPosInMap(Map map) {
	for(int i=0;i<map.height;i++) {
		for(int j=0;j<map.width;j++) {
			if(map.tiles[i][j] == '2') {
				int *position = malloc(3 * sizeof(int*));
				position[0] = i;
				position[1] = j;
				return position;
			}
		}
	}
	return NULL;
}

Map movePlayer(Map map, int x, int y) {
	printf("MOVER PARA (%d, %d)\n", x, y);
	if((x < map.width) && (y < map.height) && (map.ended == 0)) {
		int xDist = abs(y - map.playerPos[0]);
		int yDist = abs(x - map.playerPos[1]);
		if((xDist + yDist) == 1) {
			if((map.tiles[y][x] == '0') || (map.tiles[y][x] == 'X')) {
				map.tiles[map.playerPos[0]][map.playerPos[1]] = 'X';
				map.tiles[y][x] = '2';
				map.playerPos[0] = y;
				map.playerPos[1] = x;
				renderMap(map);
			} else if (map.tiles[y][x] == '3') {
				map.tiles[map.playerPos[0]][map.playerPos[1]] = 'X';
				map.tiles[y][x] = '4';
				map.playerPos[0] = y;
				map.playerPos[1] = x;
				map.ended = 1;
				renderMap(map);
			}
		}
	}
	return map;
}
