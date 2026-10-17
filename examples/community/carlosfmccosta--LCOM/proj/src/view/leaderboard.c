/**
 * @file leaderboard.c
 * @brief Implementation of leaderboard management.
 *
 * This file contains the implementation of the functions declared in leaderboard.h.
 */
#include "leaderboard.h"
#include <stdlib.h>

int num_players = 0;

int get_info_lb(const char *filename, leaderboard_t *entries) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: %d (%s)\n", errno, strerror(errno));
    return 1;
  }

  num_players = 0;

  char name_buf[MAX_NAME_CHARS];
  int points;

  while (num_players < MAX_TOP && fscanf(file, "%d %s", &points, name_buf) == 2) {
    entries[num_players].points = points;
    entries[num_players].name = malloc(strlen(name_buf) + 1);
    if (entries[num_players].name) {
      strcpy(entries[num_players].name, name_buf);
      num_players++;
    }
  }

  fclose(file);
  return 0;
}

int compare_desc(const void *a, const void *b) {
  leaderboard_t *l1 = (leaderboard_t *) a;
  leaderboard_t *l2 = (leaderboard_t *) b;
  return l2->points - l1->points;
}

void update_lb(int score, const char *name) {
    entries[num_players].points = score;
    entries[num_players].name = malloc(strlen(name) + 1);
    strcpy(entries[num_players].name, name);
    num_players++;

  // Sort all entries in descending order
  qsort(entries, num_players, sizeof(leaderboard_t), compare_desc);

  // Trim
  if (num_players > MAX_TOP) {
    free(entries[MAX_TOP].name);
    num_players = MAX_TOP;
  }
}

void save_lb(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    printf("Error writing leaderboard\n");
    return;
  }

  for (int i = 0; i < num_players; i++) {
    fprintf(file, "%d %s\n", entries[i].points, entries[i].name);
  }

  fclose(file);
}

void free_lb() {
  for (int i = 0; i < num_players; i++) {
    free(entries[i].name);
  }
}

void main_leaderboard(int score, const char *name) {
  if (get_info_lb("/home/lcom/labs/proj/src/assets/leaderboard.txt", entries))
    return;
  update_lb(score, name);
  save_lb("/home/lcom/labs/proj/src/assets/leaderboard.txt");
  free_lb();
}
