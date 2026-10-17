#include <lcom/lcf.h>

#include "score.h"
#include <limits.h>



void loadScores(){
    static char dates[5][32];
    int *values[] = {&s.scoreOne, &s.scoreTwo, &s.scoreThree, &s.scoreFour, &s.scoreFive};
    char **targets[] = {&s.scoreOneDate, &s.scoreTwoDate, &s.scoreThreeDate, &s.scoreFourDate, &s.scoreFiveDate};
    s = (Scores){0};
    FILE *best_scores = fopen("/z.txt", "r");
    if (best_scores == NULL) {
        best_scores = fopen("/z.txt", "w");
        if (best_scores != NULL)
            fclose(best_scores);
        return;
    }
    for (unsigned i = 0; i < 5; i++) {
        char line[32];
        if (fgets(line, sizeof line, best_scores) == NULL)
            break;
        char *end;
        long value = strtol(line, &end, 10);
        if (end == line || value < 0 || value > INT_MAX || (*end != ';' && *end != '\n'))
            break;
        if (fgets(dates[i], sizeof dates[i], best_scores) == NULL)
            break;
        *values[i] = (int)value;
        *targets[i] = dates[i];
    }
    fclose(best_scores);
}

void saveScores(){

    FILE *best_scores = fopen("bestScores.txt", "w");

    if(!best_scores) {
        perror("File opening failed");
        return;// EXIT_FAILURE;
    }

    printf("sc 1");
    char score[32];
    if (s.scoreOne != 0){
        printf("sc 2");
        sprintf(score, "%d;\n", s.scoreOne);
        printf("sc 3");
        printf("%s", score);

        
        fputs(score, best_scores);
        printf("sc 4");
        fputs(s.scoreOneDate, best_scores);
        printf("sc 5");
        if (s.scoreTwo != 0){
            sprintf(score, "%d;\n", s.scoreTwo);
            fputs(score, best_scores);
            fputs(s.scoreTwoDate, best_scores);
            if (s.scoreThree != 0){
                sprintf(score, "%d;\n", s.scoreThree);
                fputs(score, best_scores);
                fputs(s.scoreThreeDate, best_scores);
                if (s.scoreFour != 0){
                    sprintf(score, "%d;\n", s.scoreFour);
                    fputs(score, best_scores);
                    fputs(s.scoreFourDate, best_scores);
                    if (s.scoreFive != 0){
                        sprintf(score, "%d;\n", s.scoreFive);
                        fputs(score, best_scores);
                        fputs(s.scoreFiveDate, best_scores);
                    }
                }
            }


        }
    }

    fclose(best_scores);
}

void addNewScore(int score){
    if (score < s.scoreFive)
        return;
    
    char * date = get_current_date_and_time(false);

    printf("1");

    if (s.scoreOne == 0){
        s.scoreOne = score;
        printf("1.5");
        s.scoreOneDate = date;
        printf("1.6");
        return;
    }
    else if (s.scoreTwo == 0){
        s.scoreTwo = score;
        s.scoreTwoDate = date;
        return;
    }
    else if (s.scoreThree == 0){
        s.scoreThree = score;
        s.scoreThreeDate = date;
        return;
    }
    else if (s.scoreFour == 0){
        s.scoreFour = score;
        s.scoreFourDate = date;
        return;
    }
    else if (s.scoreFive == 0){
        s.scoreFive = score;
        s.scoreFiveDate = date;
        return;
    }

    printf("2");


    if (score > s.scoreFive){
        s.scoreFive = score;
        s.scoreFiveDate = date;
        if (score > s.scoreFour){
            s.scoreFive = s.scoreFour;
            s.scoreFiveDate = s.scoreFourDate;
            s.scoreFour = score;
            s.scoreFourDate = date;
            if (score > s.scoreThree){
                s.scoreFour = s.scoreThree;
                s.scoreFourDate = s.scoreThreeDate;
                s.scoreThree = score;
                s.scoreThreeDate = date;
                if (score > s.scoreTwo){
                    s.scoreThree = s.scoreTwo;
                    s.scoreThreeDate = s.scoreTwoDate;
                    s.scoreTwo = score;
                    s.scoreTwoDate = date;
                    if (score > s.scoreOne){
                        s.scoreTwo = s.scoreOne;
                        s.scoreTwoDate = s.scoreOneDate;
                        s.scoreOne = score;
                        s.scoreOneDate = date; 
                    }
                }
            }
        }
    }
    printf("3");


}

