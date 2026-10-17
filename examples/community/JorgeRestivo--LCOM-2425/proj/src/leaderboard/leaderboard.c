#include "leaderboard.h"



void read_leaderboard(Game game){
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        printf("Error opening leaderboard file.\n");
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        fscanf(file, "%d", &game.leaderboard[i]);
    }
    
    fclose(file);
}

int check_score(Game game){
    int cur = game.score, temp = -1;
    for (int i = 0; i < 5; i++) {
        if (game.score > game.leaderboard[i]) {
            temp = game.leaderboard[i];
            game.leaderboard[i] = cur;
            cur = temp;
        }
    }
    
    game.score = 0;
    
    if (temp != -1) {
        return 0;
    } else {
        return 1;
    }
}

void write_leaderboard(Game game){
    FILE *file = fopen("leaderboard.txt", "w");
    if (file == NULL) {
        printf("Error opening leaderboard file.\n");
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        fprintf(file, "%d\n", game.leaderboard[i]);
    }
    
    fclose(file);
}
