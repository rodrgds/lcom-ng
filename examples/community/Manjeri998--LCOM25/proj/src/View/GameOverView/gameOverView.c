#include "gameOverView.h"

void loadGameOverSprites() {
    gover_background = generate_Sprite((xpm_map_t) GameOverBackground);
    retry = generate_Sprite((xpm_map_t) GameOverRetry);
    exit_ = generate_Sprite((xpm_map_t) GameOverExit);
}


void setGameOverBackgorund() {
    loadBackground(gover_background);
    drawSpriteToBackground(retry, RETRY_X, RETRY_Y);
    drawSpriteToBackground(exit_, OVER_EXIT_X, OVER_EXIT_Y);
}
