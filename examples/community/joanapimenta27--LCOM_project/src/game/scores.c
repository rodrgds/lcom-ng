#include "scores.h"
#include <sys/stat.h>
#include <unistd.h>

int num_scores = 0;

extern rtc_info time_info;

void load_scoreboard(const char *player_name, int level, int score, int year, int month, int day) {
  if (num_scores < MAX_SCORES) {
      strcpy(scoreboard[num_scores].player_name, player_name);
      scoreboard[num_scores].level = level;
      scoreboard[num_scores].score = score;
      scoreboard[num_scores].year = year;
      scoreboard[num_scores].month = month;
      scoreboard[num_scores].day = day;
      num_scores++;
      sort_scores();
  }
}

void insert_score(const char *player_name, int level, int score){
  scoreboard[MAX_SCORES_PER_LEVEL * level -1].score = score;
  scoreboard[MAX_SCORES_PER_LEVEL * level -1].level = level;
  strcpy(scoreboard[MAX_SCORES_PER_LEVEL * level -1].player_name, player_name);
  rtc_update_time();
  scoreboard[MAX_SCORES_PER_LEVEL * level -1].year = time_info.year;
  scoreboard[MAX_SCORES_PER_LEVEL * level -1].month = time_info.month;
  scoreboard[MAX_SCORES_PER_LEVEL * level -1].day = time_info.day;
  sort_scores();
}

void sort_scores() {
    // Sorting implementation
    ScoreEntry temp;
    for (int j = 0; j < MAX_SCORES_PER_LEVEL; j++){
      for (int i = 1; i < MAX_SCORES; i++) {
        if (scoreboard[i].level != 0){
          if (scoreboard[i].level < scoreboard[i-1].level) {
            temp = scoreboard[i];        //Verify if struct copy like this *********************
            scoreboard[i] = scoreboard[i-1];
            scoreboard[i-1] = temp;
          }
          else if ((scoreboard[i].level == scoreboard[i-1].level) && (scoreboard[i].score > scoreboard[i-1].score)) {
            temp = scoreboard[i];
            scoreboard[i] = scoreboard[i-1];
            scoreboard[i-1] = temp;
          }
        }
      }
    }
}

void save_scores(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < MAX_SCORES; i++) {
      if (scoreboard[i].level != 0){
        fprintf(file, "%s %d %d %d %d %d\n", scoreboard[i].player_name, scoreboard[i].level, scoreboard[i].score, scoreboard[i].year, scoreboard[i].month, scoreboard[i].day);
      }
    }

    fclose(file);
}


void load_scores(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }
    char player_name[MAX_NAME_LENGTH];
    int level;
    int score;
    int year;
    int month;
    int day;
    while (fscanf(file, "%s %d %d %d %d %d", player_name, &level, &score, &year, &month, &day) == 6) {
      load_scoreboard(player_name, level, score, year, month, day);
    }

    fclose(file);

    /*for (int i = 0; i < MAX_SCORES; i++){
      printf("%s %d %d %d %d %d\n", scoreboard[i].player_name, scoreboard[i].level, scoreboard[i].score, scoreboard[i].year, score,scoreboard[i].month, scoreboard[i].day);
    }
    */
}


//if player have more points than one of the three scores
bool verify_if_go_to_scoreboard(int level, int score){
  if (scoreboard[MAX_SCORES_PER_LEVEL * level -1].score < score) return true; //if scoreboard is sorted by level (up) and scores (down)
  else return false;
}
