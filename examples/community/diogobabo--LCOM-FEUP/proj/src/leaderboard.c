#include "leaderboard.h"

extern enum STATE GameState;
extern uint8_t *BoardMenu;
extern xpm_image_t imgBoard;

extern uint8_t *font;
extern xpm_image_t imgFont;

extern Character letters[40];

static void read_scores(int scores[5]) {
    for (int i = 0; i < 5; i++) scores[i] = 0;
    FILE *file = fopen("/home/lcom/labs/g04/proj/src/leaderboard.txt", "r");
    if (!file) return;
    for (int i = 0; i < 5; i++) {
        int rank, value;
        if (fscanf(file, "%d - %d", &rank, &value) != 2) break;
        scores[i] = value;
    }
    fclose(file);
}

void leaderboard(int Score) {
    int scores[5];
    read_scores(scores);
    for (int i = 0; i < 5; i++) {
        if (Score > scores[i]) {
            int displaced = scores[i];
            scores[i] = Score;
            Score = displaced;
        }
    }
    FILE *file = fopen("/home/lcom/labs/g04/proj/src/leaderboard.txt", "w");
    if (!file) return;
    for (int i = 0; i < 5; i++) fprintf(file, "%d - %d\n", i + 1, scores[i]);
    fclose(file);
}

void BoardTimerHandler() {
}

void BoardHandlerKBC(enum KEY k) {
  switch (k)
  {
  case ESC:
    GameState = MENU;
    break;
  default:
    break;
  }
}

void drawDefBoard() {
  draw_pix_map(0,0,BoardMenu,imgBoard);
  drawBoard();
}

void drawBoard() {
    int scores[5];
    read_scores(scores);
    int startPosX = 576;
    int startPosY = 400;
    for (int j = 0; j < 5; j++) {
        char score[12];
        snprintf(score, sizeof score, "%d", scores[j]);
        for (int i = 0; i < (int) strlen(score); i++) {
            draw_letter_map(startPosX - ((int)(score[i] - '0')*37) + i * 20, startPosY, font, imgFont, letters[(int)(score[i] - '0') + 26].x, letters[(int)(score[i] - '0') + 26].y);
        }
        startPosY += 48;
    }
}
