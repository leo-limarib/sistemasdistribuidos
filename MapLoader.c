#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maploader.h"

int main() {
	Map map = loadMap("/home/leonardo/TrabalhoSistemasDist/maps/testmap.map");
	printf("%d %d\n", map.height, map.width);
	renderMap(map.tiles, map.height, map.width);
}
