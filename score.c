#include "score.h"

#include <stdio.h>
#include <stdlib.h>

int save_score(Score *score) {
  // Get best scores and compare if current is better
  Score scores[MAX_SCORES];
  get_scores(scores);

  // Place the score in correct order
  int i = 0;
  while (i < MAX_SCORES) {
    if (score->score >= scores[i].score) {
      scores[i].score = score->score;
      int c = 0;
      while (score->pseudo[c] != '\0') {
        scores[i].pseudo[c] = score->pseudo[c];
        c++;
      }
      scores[i].pseudo[c] = '\0';
      i = MAX_SCORES;
    }
    i++;
  }

  // Rewrite scores to file
  FILE *file = NULL;
  file = fopen(FILENAME, "w");
  if (file == NULL) {
    return 1;
  }

  int c = 0;
  while (scores[c].pseudo[0] != '\0') {
    fprintf(file, "%s\n", scores[c].pseudo);
    fprintf(file, "%d\n", scores[c].score);
    c++;
  }

  fclose(file);
  return 0;
}

int get_scores(Score scores[MAX_SCORES]) {
  for (int i = 0; i < MAX_SCORES; i++) {
    scores[i].score = 0;
    scores[i].pseudo[0] = '\0';
  }

  FILE *file = NULL;
  file = fopen(FILENAME, "a+"); // Create file if does not exist
  if (file == NULL) {
    return 1;
  }

  int i = 0;
  int read = 1;
  while (i < MAX_SCORES && read != -1) {
    read = fscanf(file, "%s\n", scores[i].pseudo);
    read = fscanf(file, "%d\n", &scores[i].score);
    if (read == -1) {
      scores[i].pseudo[0] = '\0';
      scores[i].score = 0;
    }
    i++;
  }
  fclose(file);
  return 0;
}