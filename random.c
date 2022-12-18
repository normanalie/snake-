#include "random.h"

#include <stdlib.h>
#include <time.h>

int randint(int min, int max) {
  srand(time(NULL));
  int result = (rand() % (max + 1 - min)) + min;
  return result;
}