#include "gamestate.h"
#include "../assets/xpm/all.xpm"

extern int mouse_x, mouse_y;

GameState* createGameState(int option, bool isMultiplayer, bool isMultiplayerLocal, bool isClassic, bool isRemote) {
    GameState* gs = malloc(sizeof(GameState));
    gs->gamebackground = create_sprite(gamebackground_xpm, 0, 0, 0, 0);

    if (!isClassic) {
        gs->normalApple = create_apple(NORMAL, true, isMultiplayer);
        gs->goldenApple = create_apple(GOLDEN, false, isMultiplayer);
        gs->whiteApple = create_apple(WHITE, false, isMultiplayer);
        gs->darkApple = create_apple(DARK, false, isMultiplayer);
        gs->snake = createFreeSnake(isRemote ? 600 : 200, 200);
        gs->snakeClassic = NULL;
        gs->apple = NULL;
    }
    else {
        gs->normalApple = NULL;
        gs->goldenApple = NULL;
        gs->whiteApple = NULL;
        gs->darkApple = NULL;
        gs->snake = NULL;
        gs->snakeClassic = create_snake(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE);
        gs->apple = create_sprite(apple_xpm, (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE, (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE, 0, 0);
    }

    gs->opponentSnake = isMultiplayerLocal ? createFreeSnake(600, 200) : NULL;
    gs->remoteSnake = isMultiplayer ? createFreeSnake(isRemote ? 200 : 600, 200) : NULL;

    gs->cursorSprite = create_sprite(cursor, mouse_x, mouse_y, 0, 0);

    gs->score = createScore(0,0,0);
    gs->opponentScore= isMultiplayerLocal ? createScore(0,SCREEN_WIDTH/2,0):NULL;

    gs->option = option;
    gs->multiplayer = isMultiplayer;
    gs->isRemote = isRemote;

    return gs;
}

void destroyGameState(GameState* gs) {    
    gs->gamebackground->destroy(gs->gamebackground);
    if (gs->normalApple) gs->normalApple->destroy(gs->normalApple);
    if (gs->goldenApple) gs->goldenApple->destroy(gs->goldenApple);
    if (gs->whiteApple) gs->whiteApple->destroy(gs->whiteApple);
    if (gs->darkApple) gs->darkApple->destroy(gs->darkApple);
    if (gs->snake) gs->snake->destroy(gs->snake);
    if (gs->snakeClassic) gs->snakeClassic->destroy(gs->snakeClassic);
    if (gs->apple) gs->apple->destroy(gs->apple);
    if (gs->opponentSnake) gs->opponentSnake->destroy(gs->opponentSnake);
    if (gs->remoteSnake) gs->remoteSnake->destroy(gs->remoteSnake);
    if (gs->cursorSprite) gs->cursorSprite->destroy(gs->cursorSprite);
    if (gs->score) gs->score->destroy(gs->score);
    if (gs->opponentScore) gs->opponentScore->destroy(gs->opponentScore);
    
    free(gs);
}

void drawGameState(GameState* gs) {
    clear_screen();
    gs->gamebackground->draw(gs->gamebackground);
    if (gs->normalApple) gs->normalApple->drawIfExists(gs->normalApple);
    if (gs->goldenApple) gs->goldenApple->drawIfExists(gs->goldenApple);
    if (gs->whiteApple) gs->whiteApple->drawIfExists(gs->whiteApple);
    if (gs->darkApple) gs->darkApple->drawIfExists(gs->darkApple);
    if (gs->snake) gs->snake->draw(gs->snake);
    if (gs->snakeClassic) gs->snakeClassic->draw(gs->snakeClassic);
    if (gs->remoteSnake) gs->remoteSnake->draw(gs->remoteSnake);
    if (gs->apple) gs->apple->draw(gs->apple);
    if (gs->opponentSnake) gs->opponentSnake->draw(gs->opponentSnake);
    if (gs->cursorSprite) gs->cursorSprite->draw(gs->cursorSprite);
    if (gs->score) gs->score->draw(gs->score);
    if (gs->opponentScore) gs->opponentScore->draw(gs->opponentScore);
    draw();
}
