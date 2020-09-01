#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maploader.h"

int main() {
	Map map = loadMap("/home/leonardo/sistemasdistribuidos/maps/testmap.map");
	printf("%d %d\n", map.height, map.width);
	map.playerPos = getPlayerPosInMap(map.tiles, map.height, map.width);
	printf("Player Position: (%d, %d)", map.playerPos[0], map.playerPos[1]);
	renderMap(map.tiles, map.height, map.width);
}
