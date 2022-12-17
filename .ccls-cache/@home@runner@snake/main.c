#include <stdio.h>

// Afficher board
// Afficher serpent L=3
// Avancer serpent
// Diriger serpent
// Collision serpent
// Afficher fruit
// Grandir serpent
// Generer fruit
// Afficher game over
// Afficher menu

#define BOARD_SIZE 15
#define BOARD_WALL 'X'

void print_board(int size, char wall);

int main(void) {
  print_board(BOARD_SIZE, BOARD_WALL);
  return 0;
}

void print_board(int size, char wall) {
  float ratio = 1.5; // line/column spacement ratio to have a square board
  int width = (int)size * 1.5;
  // Top line
  for (int i = 0; i < width + 1; i++) {
    printf("%c", wall);
  }
  printf("%c\n", wall);

  // Sides
  for (int i = 0; i < size - 2; i++) { // size-(TopLine+BottomLine)
    printf("%c %*c\n", wall, width, wall);
  }

  // Bottom line
  for (int i = 0; i < width + 1; i++) {
    printf("%c", wall);
  }
  printf("%c\n", wall);
  return;
}