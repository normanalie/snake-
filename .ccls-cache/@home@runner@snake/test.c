#include "gestion_clavier.h"
#include <stdio.h>

#define UP 'z'
#define DOWN 's'
#define LEFT 'q'
#define RIGHT 'd'
#define ESC 27

int main() {
  char key;

  do {
    key = pressing_key();
    if (key == NO_KEY) {
      printf("Aucune touche\n");
    } else {
      printf("La touche %c a été appuyée...\n", key);
    }

  } while (key != ESC);

  printf("##\nle programme est terminé.\n");
  return 0;
}