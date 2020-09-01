#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cores no console.
// \033[0;31m -> red
// \033[0;32m -> green
// \033[0;33m -> yellow

typedef struct {
	int height, width;
	char **tiles;
	int* playerPos;
} Map;

void renderMap(char **aMap, int height, int width);
Map loadMap(char *mapFilename);
int* getPlayerPosInMap(char **tiles, int height, int width);

/************************************************
 *  Função responsável por renderizar o mapa no console.
 *  @param aMap Array com os tiles do mapa.
 *  @param height Altura do mapa.
 *  @param width Largura do mapa
 ***********************************************/
void renderMap(char **aMap, int height, int width) {
	printf("\n");
	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
      if(aMap[i][j] == '1') {
        printf("\033[0;31m"); //Set the text to the color red
        printf("%c", aMap[i][j]);
        printf("\033[0m");
      }
      else if (aMap[i][j] == '2') {
        printf("\033[01;33m"); //Set the text to the color red
        printf("%c", aMap[i][j]);
        printf("\033[0m");
      }
      else if (aMap[i][j] == '3') {
        printf("\033[01;36m"); //Set the text to the color red
        printf("%c", aMap[i][j]);
        printf("\033[0m");
      }
      else printf("%c", aMap[i][j]);
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
	Map map;

	printf("Loading map %s", mapFilename);
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
	} else {
		printf("Impossível abrir o mapa especificado.");
		return map;
	}
	return map;
}

/************************************************
 *  Função responsável por pegar a posição do player nos tiles.
 *  @param tiles Array 2d contendo as informações dos tiles.
 *  @param height A altura do mapa
 *  @param width A largura do mapa
 *  @return Retorna um array[2] com a posição [0] = x e [1] = y do player.
 ***********************************************/
int* getPlayerPosInMap(char **tiles, int height, int width) {
	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
			if(tiles[i][j] == '2') {
				int *position = malloc(3 * sizeof(int*));
				position[0] = i;
				position[1] = j;
				return position;
			}
		}
	}
	return NULL;
}
