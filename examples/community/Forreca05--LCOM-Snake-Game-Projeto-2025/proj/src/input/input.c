#include "input.h"

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include "../controllers/KBC/i8042.h"
#include "../controllers/KBC/keyboard/keyboard.h"
#include "../controllers/KBC/mouse/mouse.h"
#include "../controllers/sp/sp.h"
#include "../game/elements/apple.h"
#include "../game/elements/freesnake.h"
#include "../game/score.h"
#include "../game/UTILS.h"

extern uint8_t scancode, remote_scancode, received_byte;
extern int mouse_x, mouse_y, timer_count;
extern uint8_t byte_index;
int goldenAppleCounter = 0, whiteAppleCounter = 0, darkAppleCounter = 0;

// ------------------------------------------------------------------------------------------------------------------------------

void handleMouseInputGeneric(GameState* gs) {
    mouse_ih();
    mouse_sync_bytes();
    if (byte_index == 3) {
        byte_index = 0;
        mouse_bytes_to_packet();
        update_mouse_position();
    }
    gs->cursorSprite->x = mouse_x;
    gs->cursorSprite->y = mouse_y;
}

// some helper functions

static bool handleAppleCollision(GameState* gs, FreeSnake* snake, Apple* apple, Score* score, int scoreToAdd) {
    if (apple->exists && snake->checkAppleCollision(snake, apple)) {
        apple->applyPowerUp(apple, snake, gs->option);
        score->score += scoreToAdd;
        apple->powerUpTimer = 0;
        return true;
    }
    return false;
}

static void handleAppleSpawnDespawn(Apple* apple, int* appleCounter, int spawnTime, int despawnTime) {
    if (*appleCounter >= spawnTime) {
        if (*appleCounter == spawnTime) {
            apple->exists = true;
        }
        else if (*appleCounter == despawnTime) {
            apple->exists = false;
            apple->x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            apple->y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
            *appleCounter = 0;
        }
    }
}

// ------------------------------------------------------------------------------------------------------------------------------

int handleKeyboardInputSinglePlayerClassic(GameState* gs) {
    kbc_ih();

    if (scancode == BREAK_ESC) return STATE_MENU;

    return STATE_GAME_SINGLEPLAYER_CLASSIC;
}

int handleMouseInputSinglePlayerClassic(GameState* gs) {
    handleMouseInputGeneric(gs);
    return STATE_GAME_SINGLEPLAYER_CLASSIC;
}


int handleTimerInputSinglePlayerClassic(GameState* gs) {
    timer_int_handler();

    if (scancode == KEY_A && gs->snakeClassic->direction != RIGHT) 
        gs->snakeClassic->direction = LEFT;
    else if (scancode == KEY_W && gs->snakeClassic->direction != DOWN)
        gs->snakeClassic->direction = UP;
    else if (scancode == KEY_D && gs->snakeClassic->direction != LEFT) 
        gs->snakeClassic->direction = RIGHT;
    else if (scancode == KEY_S && gs->snakeClassic->direction != UP) 
        gs->snakeClassic->direction = DOWN;

    if (timer_count % 10 == 0) {
        int result = gs->snakeClassic->move(gs->snakeClassic, gs->apple);
        if (result == 0) {
            return STATE_MENU;
        }
        if (result == 1) {
            gs->score->score += 100;
        }
        
        timer_count = 0;

        drawGameState(gs);
    }

    return STATE_GAME_SINGLEPLAYER_CLASSIC;
}

// ------------------------------------------------------------------------------------------------------------------------------

int handleKeyboardInputSinglePlayerIO(GameState* gs) {
    kbc_ih();
    if (scancode == BREAK_ESC) {
        return STATE_MENU;
    }
    return STATE_GAME_SINGLEPLAYER_IO;
}

int handleMouseInputSinglePlayerIO(GameState* gs) {
    handleMouseInputGeneric(gs);
    return STATE_GAME_SINGLEPLAYER_IO;
}

int handleTimerInputSinglePlayerIO(GameState* gs) {

    int goldenAppleSpawnTime = 140 + rand() % 100;
    int goldenAppleDeSpawnTime = goldenAppleSpawnTime + 50 + rand() % 100;
    int whiteAppleSpawnTime = 140 + rand() % 100;
    int whiteAppleDeSpawnTime = whiteAppleSpawnTime + 50 + rand() % 100;
    int darkAppleSpawnTime = 140 + rand() % 100;
    int darkAppleDeSpawnTime = darkAppleSpawnTime + 50 + rand() % 100;

    timer_int_handler();

    if (scancode == KEY_A) 
        gs->snake->direction = LEFT;
    else if (scancode == KEY_D) 
        gs->snake->direction = RIGHT;
    else 
        gs->snake->direction = NO;

    if (timer_count % 2 == 0) {
        if (!gs->snake->move(gs->snake, gs->option)) return STATE_MENU;

        gs->goldenApple->checkPowerUpTimer(gs->goldenApple, gs->snake, gs->option);
        gs->whiteApple->checkPowerUpTimer(gs->whiteApple, gs->snake, gs->option);
        gs->darkApple->checkPowerUpTimer(gs->darkApple, gs->snake, gs->option);

        handleAppleCollision(gs, gs->snake, gs->normalApple,gs->score, 100);
        handleAppleCollision(gs, gs->snake, gs->goldenApple,gs->score, 200);
        handleAppleCollision(gs, gs->snake, gs->whiteApple, gs->score,300);
        handleAppleCollision(gs, gs->snake, gs->darkApple,  gs->score,400);

        handleAppleSpawnDespawn(gs->goldenApple, &goldenAppleCounter, goldenAppleSpawnTime, goldenAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->whiteApple, &whiteAppleCounter, whiteAppleSpawnTime, whiteAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->darkApple, &darkAppleCounter, darkAppleSpawnTime, darkAppleDeSpawnTime);
        
        gs->snake->speed += 0.01;
        timer_count = 0;
        drawGameState(gs);
    }
    goldenAppleCounter++;
    whiteAppleCounter++;
    darkAppleCounter++;

    return STATE_GAME_SINGLEPLAYER_IO;
}

// ------------------------------------------------------------------------------------------------------------------------------

int handleKeyboardInputMultiPlayer(GameState* gs) {
    kbc_ih();
    sp_send_byte(scancode);
    if (scancode == BREAK_ESC) {
        return STATE_MENU;
    }
    return STATE_GAME_MULTIPLAYER_REMOTE;
}

int handleMouseInputMultiPlayer(GameState* gs) {
    handleMouseInputGeneric(gs);
    return STATE_GAME_MULTIPLAYER_REMOTE;
}

int handleTimerInputMultiPlayer(GameState* gs) {
    int goldenAppleSpawnTime = 150;
    int goldenAppleDeSpawnTime = 270;

    int whiteAppleSpawnTime = 180;
    int whiteAppleDeSpawnTime = 260;

    int darkAppleSpawnTime = 200;
    int darkAppleDeSpawnTime = 300;

    timer_int_handler();

    if (scancode == KEY_A) {
        gs->snake->direction = LEFT;
    }
    else if (scancode == KEY_D) {
        gs->snake->direction = RIGHT;
    }
    else {
        gs->snake->direction = NO;
    }

    if (remote_scancode == KEY_A) {
        gs->remoteSnake->direction = LEFT;
    }
    else if (remote_scancode == KEY_D) {
        gs->remoteSnake->direction = RIGHT;
    }
    else {
        gs->remoteSnake->direction = NO;
    }

    if (timer_count % 2 == 0) {
        gs->snake->move(gs->snake, 2);
        gs->remoteSnake->move(gs->remoteSnake, 2);

        gs->goldenApple->checkPowerUpTimer(gs->goldenApple, gs->snake, 2);
        gs->goldenApple->checkPowerUpTimer(gs->goldenApple, gs->remoteSnake, 2);

        gs->whiteApple->checkPowerUpTimer(gs->whiteApple, gs->snake, 2);
        gs->whiteApple->checkPowerUpTimer(gs->whiteApple, gs->remoteSnake, 2);

        gs->darkApple->checkPowerUpTimer(gs->darkApple, gs->snake, 2);
        gs->darkApple->checkPowerUpTimer(gs->darkApple, gs->remoteSnake, 2);

        handleAppleCollision(gs, gs->snake, gs->normalApple,gs->score, 100);
        handleAppleCollision(gs, gs->snake, gs->goldenApple,gs->score, 200);
        handleAppleCollision(gs, gs->snake, gs->whiteApple, gs->score,300);
        handleAppleCollision(gs, gs->snake, gs->darkApple,  gs->score,400);

        handleAppleCollision(gs, gs->remoteSnake, gs->normalApple,gs->opponentScore,  100);
        handleAppleCollision(gs, gs->remoteSnake, gs->goldenApple,gs->opponentScore,  200);
        handleAppleCollision(gs, gs->remoteSnake, gs->whiteApple, gs->opponentScore, 300);
        handleAppleCollision(gs, gs->remoteSnake, gs->darkApple,  gs->opponentScore, 400);

        handleAppleSpawnDespawn(gs->goldenApple, &goldenAppleCounter, goldenAppleSpawnTime, goldenAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->whiteApple, &whiteAppleCounter, whiteAppleSpawnTime, whiteAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->darkApple, &darkAppleCounter, darkAppleSpawnTime, darkAppleDeSpawnTime);

        drawGameState(gs);

        goldenAppleCounter++;
        whiteAppleCounter++;
        darkAppleCounter++;
        timer_count = 0;
    }
    return STATE_GAME_MULTIPLAYER_REMOTE;
}

int handleSerialPortInput() {
    uint8_t lsr;
    util_sys_inb(COM1_BASE_ADDR + LSR, &lsr);
    if (lsr & 0x01) {
        
        util_sys_inb(COM1_BASE_ADDR, &remote_scancode);
        printf("received scancode\n");
    }
    return STATE_GAME_MULTIPLAYER;
}

// ------------------------------------------------------------------------------------------------------------------------------


int handleKeyboardInputMultiPlayerLocal(GameState* gs) {
    kbc_ih();
    
    if (scancode == BREAK_ESC) {
        return STATE_MENU;
    }
    return STATE_GAME_MULTIPLAYER_LOCAL;
}
                    
int handleMouseInputMultiPlayerLocal(GameState* gs) {
    handleMouseInputGeneric(gs);
    return STATE_GAME_MULTIPLAYER_LOCAL;
}
                
int handleTimerInputMultiPlayerLocal(GameState* gs) {

    int goldenAppleSpawnTime = 140 + rand() % 100;
    int goldenAppleDeSpawnTime = goldenAppleSpawnTime + 50 + rand() % 100;
    int whiteAppleSpawnTime = 140 + rand() % 100;
    int whiteAppleDeSpawnTime = whiteAppleSpawnTime + 50 + rand() % 100;
    int darkAppleSpawnTime = 140 + rand() % 100;
    int darkAppleDeSpawnTime = darkAppleSpawnTime + 50 + rand() % 100;

    timer_int_handler();

    if (scancode == KEY_A) 
        gs->snake->direction = LEFT;
    else if (scancode == KEY_D) 
        gs->snake->direction = RIGHT;
    else 
        gs->snake->direction = NO;

    gs->cursorSprite->x = mouse_x;
    gs->cursorSprite->y = mouse_y;

    if (timer_count % 3 == 0) {
        if (!gs->snake->move(gs->snake, gs->option)) return STATE_MENU;
        if (!gs->opponentSnake->move(gs->opponentSnake, 1)) return STATE_MENU;

        gs->goldenApple->checkPowerUpTimer(gs->goldenApple, gs->snake, gs->option);
        gs->goldenApple->checkPowerUpTimer(gs->goldenApple, gs->opponentSnake, 1);

        gs->whiteApple->checkPowerUpTimer(gs->whiteApple, gs->snake, gs->option);
        gs->whiteApple->checkPowerUpTimer(gs->whiteApple, gs->opponentSnake, 1);

        gs->darkApple->checkPowerUpTimer(gs->darkApple, gs->snake, gs->option);
        gs->darkApple->checkPowerUpTimer(gs->darkApple, gs->opponentSnake, 1);

        handleAppleCollision(gs, gs->snake, gs->normalApple,gs->score, 100);
        handleAppleCollision(gs, gs->snake, gs->goldenApple,gs->score, 200);
        handleAppleCollision(gs, gs->snake, gs->whiteApple, gs->score, 300);
        handleAppleCollision(gs, gs->snake, gs->darkApple,  gs->score, 400);

        handleAppleCollision(gs, gs->opponentSnake, gs->normalApple,gs->opponentScore, 100);
        handleAppleCollision(gs, gs->opponentSnake, gs->goldenApple,gs->opponentScore, 200);
        handleAppleCollision(gs, gs->opponentSnake, gs->whiteApple, gs->opponentScore, 300);
        handleAppleCollision(gs, gs->opponentSnake, gs->darkApple,  gs->opponentScore, 400);

        handleAppleSpawnDespawn(gs->goldenApple, &goldenAppleCounter, goldenAppleSpawnTime, goldenAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->whiteApple, &whiteAppleCounter, whiteAppleSpawnTime, whiteAppleDeSpawnTime);
        handleAppleSpawnDespawn(gs->darkApple, &darkAppleCounter, darkAppleSpawnTime, darkAppleDeSpawnTime);

        gs->snake->speed += 0.01;
        gs->opponentSnake->speed += 0.01;
        timer_count = 0;
        
        drawGameState(gs);

        if (gs->snake->checkSnakeCollision(gs->snake, gs->opponentSnake)) {
            return STATE_MENU;
        }

    }
    goldenAppleCounter++;
    whiteAppleCounter++;
    darkAppleCounter++;

    return STATE_GAME_MULTIPLAYER_LOCAL;
}
