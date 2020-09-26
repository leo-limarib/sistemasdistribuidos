#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char command[9] = "MOVE;1;2";

  //Primeiro strtok;
  char *firstToken = strtok(command, ";");
  printf("FIRST TOKEN = %s\n", firstToken);

  //Segundo strtok
  char *x = strtok(NULL, ";");
  printf("SECOND TOKEN = %s\n", x);

  //Terceiro strtok
  char *y = strtok(NULL, ";");
  printf("SECOND TOKEN = %s\n", y);
}
