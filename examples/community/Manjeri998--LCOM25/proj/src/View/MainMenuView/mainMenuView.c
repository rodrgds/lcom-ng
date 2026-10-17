#include "mainMenuView.h"

void loadMainMenuSprites() {
    mm_background = generate_Sprite((xpm_map_t) MainMenuBackGround);
    play = generate_Sprite((xpm_map_t) MainMenuPlay);
    instructions = generate_Sprite((xpm_map_t) MainMenuInstructions);
    exit_ = generate_Sprite((xpm_map_t) MainMenuExit);
}

void setMainMenuBackgorund() {
    loadBackground(mm_background);
    drawSpriteToBackground(play, PLAY_X, PLAY_Y);
    drawSpriteToBackground(instructions, INST_X, INST_Y);
    drawSpriteToBackground(exit_, EXIT_X, EXIT_Y);
}

void drawSMine(SMine mine) {
    draw_Sprite(mine.sprite, mine.pos.x, mine.pos.y);
}
