#ifndef SCORE_H
#define SCORE H

#define FILENAME "scores.txt"
#define MAX_LEN 20
#define MAX_SCORES 5

typedef struct {
  int score;
  char pseudo[MAX_LEN];
} Score;

int save_score(Score *score);
int get_scores(Score scores[MAX_SCORES]);

#endif