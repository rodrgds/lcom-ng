#include "save_score.h"
#include "../../devices/rtc/rtc.h"



void save_name_score(char *name, int score) {
    scoreboard_entry scores[10];
    scoreboard_entry temp_score;
    scoreboard_entry new_score;
    
    if (name[0] == '\0') {
        strcpy(name, "user");
    }
    while (read_rtc_time()) {
        ;
    }
    strncpy(new_score.name, name, 9);
    new_score.name[9] = '\0';
    new_score.score = score;
    new_score.seconds = rtc_info.seconds;
    new_score.minutes = rtc_info.minutes;
    new_score.hours = rtc_info.hours;
    new_score.day = rtc_info.day;
    new_score.month = rtc_info.month;
    new_score.year = rtc_info.year;

    int placed = 0;
    int c = 0;

    FILE *file = fopen("/home/lcom/labs/proj/src/scoreboard.txt", "r");
    if (file != NULL) {
        while (c < 10 && fscanf(file, "%9s %d %hhu:%hhu:%hhu - %hhu/%hhu/%hhu\n",
                                 temp_score.name, &temp_score.score,
                                 &temp_score.hours, &temp_score.minutes, &temp_score.seconds,
                                 &temp_score.day, &temp_score.month, &temp_score.year) == 8) {
            if (placed == 0 && temp_score.score < score) {
                scores[c] = new_score;
                c++;
                placed = 1;
            }
            scores[c] = temp_score;
            c++;
        }
        fclose(file);
    }

    if (placed == 0 && c < 10) {
        scores[c] = new_score;
        c++;
    }

    file = fopen("/home/lcom/labs/proj/src/scoreboard.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < c; i++) {
        fprintf(file, "%s %d %02d:%02d:%02d-%02d/%02d/%02d\n",
                scores[i].name, scores[i].score,
                scores[i].hours, scores[i].minutes, scores[i].seconds,
                scores[i].day, scores[i].month, scores[i].year);
    }
    fclose(file);
}

void read_scores(char names[][10], int scores[], char dates[][18], int size) {
    FILE *file = fopen("/home/lcom/labs/proj/src/scoreboard.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    int c = 0;
    while (c < size && fscanf(file, "%9s %d %17s", names[c], &scores[c], dates[c]) == 3) {
        c++;
    }

    fclose(file);
}

