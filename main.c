#include "gestion_clavier.h"
#include "random.h"
#include "score.h"
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
// Self collide
// Afficher game over

// Save best scores
// Afficher menu

#define KUP 'z'
#define KDOWN 's'
#define KLEFT 'q'
#define KRIGHT 'd'
#define KESC 27

#define BOARD_HEIGHT 15
#define BOARD_WIDTH 25
#define BOARD_WALL 'X'
#define SNAKE_HEAD '@'
#define SNAKE_BODY 'o'
#define APPLE '*'

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

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
  Body *head;
} Snake;

typedef struct {
  int x;
  int y;
} Apple;

void init_snake(Snake *snake, int x, int y);
int append_snake(Snake *snake);
int update_snake(Snake *snake);
int update(char board[BOARD_HEIGHT][BOARD_WIDTH], Snake *snake, Apple *apple);
void init_board(char board[BOARD_HEIGHT][BOARD_WIDTH]);
void print_board(char board[BOARD_HEIGHT][BOARD_WIDTH]);
void destroy_snake(Snake *snake);
void print_gameover(int score);

int main(void) {
  Snake snake;
  Apple apple;
  char board[BOARD_HEIGHT][BOARD_WIDTH];
  char key;

  Score score;
  score.score = 0;
  printf("Entrez votre pseudo: \n");
  scanf("%s", score.pseudo);

  init_snake(&snake, 15, 8);
  init_board(board);
  do {
    apple.x = randint(0, BOARD_WIDTH - 3);
    apple.y = randint(0, BOARD_HEIGHT - 3);
  } while (update(board, &snake, &apple) == 2);

  print_board(board);

  do {
    key = pressing_key();
    if (key != NO_KEY) {
      switch (key) {
      case KUP:
        snake.direction = snake.direction != DOWN ? UP : DOWN;
        break;
      case KDOWN:
        snake.direction = snake.direction != UP ? DOWN : UP;
        break;
      case KLEFT:
        snake.direction = snake.direction != RIGHT ? LEFT : RIGHT;
        break;
      case KRIGHT:
        snake.direction = snake.direction != LEFT ? RIGHT : LEFT;
        break;
      }
    }

    if (update_snake(&snake) == 1) { // Self-collide
      print_gameover(score.score);
      // save_score(&score);
      destroy_snake(&snake);
      return 0;
    }

    int err = update(board, &snake, &apple);
    if (err == 1) { // Colide
      print_gameover(score.score);
      save_score(&score);
      destroy_snake(&snake);
      return 0;
    } else if (err == 2) { // Apple
      append_snake(&snake);
      score.score++;
      do {
        apple.x = randint(0, BOARD_WIDTH - 3);
        apple.y = randint(0, BOARD_HEIGHT - 3);
      } while (update(board, &snake, &apple) == 2);
    }
    print_board(board);
  } while (key != KESC);
  return 0;
}

void init_snake(Snake *snake, int x, int y) {
  snake->len = 1;
  snake->direction = RIGHT;
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

void init_board(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (i == 0 || i == BOARD_HEIGHT - 1) {
        board[i][j] = BOARD_WALL;
      } else if (j == 0 || j == BOARD_WIDTH - 1) {
        board[i][j] = BOARD_WALL;
      } else {
        board[i][j] = ' ';
      }
    }
  }
  return;
}

void print_board(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      printf("%c", board[i][j]);
      // EOL
      if (j == BOARD_WIDTH - 1) {
        printf("\n");
      }
    }
  }
  return;
}

int append_snake(Snake *snake) {
  // Append element at the end of the snake
  // New elem
  Body *new_elem = malloc(sizeof(*new_elem));
  if (new_elem == NULL) {
    exit(EXIT_FAILURE);
  }
  new_elem->is_head = 0;
  new_elem->next = NULL;

  // Go to the end and add new_elem
  Body *last_elem = snake->head;
  while (last_elem->next != NULL) {
    last_elem = last_elem->next;
  }
  last_elem->next = new_elem;

  // Calc new_elem coordinates
  if (snake->direction == RIGHT) {
    new_elem->x = last_elem->x - 1;
    new_elem->y = last_elem->y;
  } else if (snake->direction == LEFT) {
    new_elem->x = last_elem->x + 1;
    new_elem->y = last_elem->y;
  } else if (snake->direction == UP) {
    new_elem->x = last_elem->x;
    new_elem->y = last_elem->y + 1;
  } else if (snake->direction == DOWN) {
    new_elem->x = last_elem->x;
    new_elem->y = last_elem->y - 1;
  }
  return 0;
}

int update(char board[BOARD_HEIGHT][BOARD_WIDTH], Snake *snake, Apple *apple) {
  init_board(board);
  // Snake is inside the board. Snake position can vary from 0 to board_size - 3
  Body *elem = snake->head;
  while (elem != NULL) {
    if (elem->x >= 0 && elem->x <= BOARD_WIDTH - 3 && elem->y >= 0 &&
        elem->y <= BOARD_HEIGHT - 3) {
      if (elem->is_head) {
        board[elem->y + 1][elem->x + 1] = SNAKE_HEAD;
      } else {
        board[elem->y + 1][elem->x + 1] = SNAKE_BODY;
      }
      elem = elem->next;
    } else {
      return 1;
    }
  }

  // Check apple position and add apple if empty, else return 2
  if (board[apple->y][apple->x] == ' ') {
    board[apple->y][apple->x] = APPLE;
  } else {
    return 2;
  }
  return 0;
}

int update_snake(Snake *snake) {
  // Browse each element and give it previous element's corrdinates
  Body *elem = snake->head;
  int prev_x = 0;
  int prev_y = 0;
  while (elem != NULL) {
    if (elem->is_head) { // Increment head coordinate and tail collision
      prev_x = elem->x;
      prev_y = elem->y;
      if (snake->direction == RIGHT) {
        elem->x = elem->x + 1;
      } else if (snake->direction == LEFT) {
        elem->x = elem->x - 1;
      } else if (snake->direction == UP) {
        elem->y = elem->y - 1;
      } else if (snake->direction == DOWN) {
        elem->y = elem->y + 1;
      }
      Body *tail_elem = elem->next;
      while (tail_elem != NULL) {
        if (elem->x == tail_elem->x && elem->y == tail_elem->y) {
          return 1;
        }
        tail_elem = tail_elem->next;
      }
    } else {
      int tmp_x = elem->x;
      int tmp_y = elem->y;
      elem->x = prev_x;
      elem->y = prev_y;
      prev_x = tmp_x;
      prev_y = tmp_y;
    }

    elem = elem->next;
  }
  return 0;
}

void print_gameover(int score) {
  printf("\n\n"
         " ██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ "
         "██████  \n"
         "██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██ "
         "  ██ \n"
         "██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   "
         "██████  \n"
         "██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██ "
         "  ██ \n"
         " ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██ "
         "  ██ \n");
  printf("\n\n Your score is: %d \n\n", score);
  return;
}

void destroy_snake(Snake *snake) {
  Body *elem = snake->head;
  while (elem != NULL) {
    Body *next = elem->next;
    free(elem);
    elem = next;
  }
  return;
}