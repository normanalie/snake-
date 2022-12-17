#include <stdio.h>
#include <stdlib.h>

// Afficher board OK
// Afficher serpent L=3
// Avancer serpent
// Diriger serpent
// Collision serpent
// Afficher fruit
// Grandir serpent
// Generer fruit
// Afficher game over
// Afficher menu

#define BOARD_HEIGHT 15
#define BOARD_WIDTH 22
#define BOARD_WALL 'X'
#define SNAKE_HEAD '@'
#define SNAKE_BODY '*'

typedef struct Body Body;
struct Body {
  int x;
  int y;
  int is_head;
  Body *next;
};

typedef struct {
  int len;
  int direction;
  int score;
  Body *head;
} Snake;

void init_snake(Snake *snake, int x, int y);
int append_snake(Snake *snake);
void display(int width, int height, char wall, Snake *snake);

int main(void) {
  Snake snake;
  init_snake(&snake, 2, 10);
  display(BOARD_WIDTH, BOARD_HEIGHT, BOARD_WALL, &snake);
  return 0;
}

void init_snake(Snake *snake, int x, int y) {
  snake->len = 1;
  snake->direction = 1;
  snake->score = 0;
  Body *head = malloc(sizeof(*head));
  if (head == NULL) {
    exit(EXIT_FAILURE);
  }
  head->x = x;
  head->y = y;
  head->is_head = 1;
  head->next = NULL;
  snake->head = head;
  return;
}

int append_snake(Snake *snake) { return 0; }

void display(int width, int height, char wall, Snake *snake) {
  for (int i = 0; i <= height; i++) {
    for (int j = 0; j <= width; j++) {
      // WALLS
      if (i == 0 || i == height) {
        printf("%c", wall);
      } else if (j == 0 || j == width) {
        printf("%c", wall);
      } else {
        Body *snake_body = snake->head;
        while (snake_body != NULL) {
          if (snake_body->x == j && snake_body->y == i) {
            printf("%c", snake_body->is_head == 1 ? SNAKE_HEAD : SNAKE_BODY);
          } else {
            printf(" ");
          }
          snake_body = snake_body->next;
        }
      }
      // INSIDE

      // EOL
      if (j == width) {
        printf("\n");
      }
    }
  }
  return;
}