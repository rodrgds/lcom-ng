#include "../include/gameState.h"
#include "../include/arena.h"
#include "../include/button.h"
#include "../include/data.h"
#include "../include/data_structures/vector.h"
#include "../include/monster/monsterGen.h"
#include "../include/monster/path.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"

#include "../../lab5/spriteXMP.h"
#include "../resources/gameOverMenu/gameOver.h"
#include "../resources/howToPlay/howToPlay.h"
#include "../resources/icons/icons.h"
#include "../resources/levelSelector/levelSelector.h"
#include "../resources/mainMenu/mainMenu.h"
#include "../resources/pauseMenu/pauseMenu.h"
#include "../resources/winMenu/winMenu.h"

#include "../resources/maps/map1/map1.h"
#include "../resources/maps/map2/map2.h"

#include "../resources/maps/map3/map3.h"

#include <stdlib.h>

/** @file gameState.c
 *  @brief Estados do jogo: menus, selector de nivel, pausa, game over, vitoria e partida em arena.
 */

static bool pressedToPlay = false;
static bool pressedNewGame = false;
static bool pressedExit = false;
static bool pressedGoBack = false;
static bool pressedPause = false;
static bool pressedHowToPlay = false;
static int currentLevel = 0;

/** @brief Callback seta: nivel anterior no selector (wrap).
 *  @param[in] self Botao (nao usado).
 */
static void leftButtonFn(Button* self) {
    currentLevel--;
    if (currentLevel < 0) currentLevel = MAX_LVL - 1;
}

/** @brief Callback seta: nivel seguinte no selector (wrap).
 *  @param[in] self Botao (nao usado).
 */
static void rightButtonFn(Button* self) {
    currentLevel++;
    if (currentLevel >= MAX_LVL) {
        currentLevel = 0;
    }
}

/** @brief Marca que o jogador pressionou jogar no selector.
 *  @param[in] self Botao (nao usado).
 */
static void selectButtonFn(Button* self) {
    pressedToPlay = true;
}

/** @brief Sai do menu de pausa (resume).
 *  @param[in] self Botao (nao usado).
 */
static void resumeFn(Button* self) {
    pressedPause = false;
}

/** @brief Abre ecra "how to play" a partir da pausa.
 *  @param[in] self Botao (nao usado).
 */
static void helpFn(Button* self) {
    pressedHowToPlay = true;
}

/** @brief Volta ao menu principal (flag global).
 *  @param[in] self Botao (nao usado).
 */
static void goBackMainMenuFn(Button* self) {
    pressedGoBack = true;
}

/** @brief Pedido de sair do programa.
 *  @param[in] self Botao (nao usado).
 */
static void exitGameFn(Button* self) {
    pressedExit = true;
}

/** @brief Inicia novo jogo a partir do menu.
 *  @param[in] self Botao (nao usado).
 */
static void newGameFn(Button* self) {
    pressedNewGame = true;
}

struct GameState {
    void (*act)(GameState* state, Terminal* terminal, float deltaTime);
    void (*free)(GameState* state);
    GameState* (*transition)(GameState* state, Terminal* terminal);
    Cursor* cursor;

    // GameLoop
    Arena* arena;

    // Main menu
    SpriteXPM* logo;
    SpriteXPM* howToPlay;
    Button* newGameButton;
    Button* closeGameButton;

    // Pause Menu
    SpriteXPM* pauseMenu;
    Button* resumeButton;
    Button* helpButton;
    Button* goBackMainMenuButton;

    // Game Over Menu
    SpriteXPM* gameOver;
    Button* restartButton;
    // uses go back main menu button

    // Level selector
    SpriteXPM** levels;
    SpriteXPM* levelSelectLogo;
    Button* leftSwitchButton;
    Button* rightSwitchButton;
    Button* playButton;

    // Win menu
    SpriteXPM* winMenu;
};

static void resetBools() {
    pressedExit = false;
    pressedGoBack = false;
    pressedNewGame = false;
    pressedPause = false;
    pressedHowToPlay = false;
    pressedToPlay = false;
}

static GameState* gameStateNew(void (*act)(GameState* state, Terminal* terminal, float deltaTimem), void (*free)(GameState* state), GameState* (*transition)(GameState* state, Terminal* terminal)) {
    resetBools();
    GameState* res = malloc(sizeof(GameState));
    res->act = act;
    res->free = free;
    res->transition = transition;

    return res;
}

void gameStateUpdate(GameState* state, Terminal* terminal, float deltaTime) {
    cursorUpdateCoordinates(getCursor());
    state->act(state, terminal, deltaTime);
    cursorUpdate(getCursor(), terminal, deltaTime);
}

void gameStateFree(GameState* state) {
    state->free(state);
}

GameState* gameStateTransition(GameState* state, Terminal* terminal) {
    return state->transition(state, terminal);
}

// Game Over
static void GameOverButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->restartButton), mousePos)) {
        if (leftClick) buttonClick(state->restartButton);
        buttonHover(state->restartButton);

    } else {
        buttonReset(state->restartButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->goBackMainMenuButton), mousePos)) {
        if (leftClick) buttonClick(state->goBackMainMenuButton);
        buttonHover(state->goBackMainMenuButton);

    } else {
        buttonReset(state->goBackMainMenuButton);
    }
}

static void gameOverAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->gameOver, terminal, GAME_OVER_POSITION.x, GAME_OVER_POSITION.y, false);
    buttonDraw(state->restartButton, terminal, deltaTime, false);
    buttonDraw(state->goBackMainMenuButton, terminal, deltaTime, false);
    GameOverButtonsHandler(state);
}

static void gameOverFree(GameState* state) {
    free(state->gameOver);
    buttonFree(state->restartButton);
    buttonFree(state->goBackMainMenuButton);
    free(state);
}

static GameState* gameOverTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    // Wait for the animation to finish first - if there is one
    if (buttonIsFinished(state->restartButton) && pressedNewGame) {
        newState = runningGameNew(terminal);
        resetBools();

    } else if (buttonIsFinished(state->goBackMainMenuButton) && pressedGoBack) {
        newState = mainMenuNew(terminal);
        resetBools();
    }

    return newState;
}

GameState* gameOverMenuNew(Terminal* terminal) {
    resetBools();

    GameState* res = gameStateNew(gameOverAct, gameOverFree, gameOverTransition);
    SpriteXPM** restartButtons = malloc(sizeof(SpriteXPM*) * GAME_OVER_RESTART_FRAMES);
    SpriteXPM** closeGameButtons = malloc(sizeof(SpriteXPM*) * GAME_OVER_EXIT_FRAMES);
    res->gameOver = newSpriteXPM((xpm_map_t)gameOverXPM[GAME_OVER_LOGO], XPM_8_8_8, 0, 0);

    // Restart Button
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)gameOverXPM[GAME_OVER_RESTART_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)gameOverXPM[GAME_OVER_RESTART_PRESSED], XPM_8_8_8, 0, 0);
    restartButtons[0] = buttonNormal;
    restartButtons[1] = buttonPressed;
    res->restartButton = buttonNew(NEW_GAME_BUTTON_POS.x, NEW_GAME_BUTTON_POS.y, NEW_GAME_BUTTON_SIZE.x, NEW_GAME_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, restartButtons, GAME_OVER_RESTART_FRAMES, NORMAL), newGameFn, basicButtonHoverFn);

    // Exit Button
    buttonNormal = newSpriteXPM((xpm_map_t)gameOverXPM[GAME_OVER_EXIT_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)gameOverXPM[GAME_OVER_EXIT_PRESSED], XPM_8_8_8, 0, 0);
    closeGameButtons[0] = buttonNormal;
    closeGameButtons[1] = buttonPressed;
    res->goBackMainMenuButton = buttonNew(EXIT_BUTTON_POS.x, EXIT_BUTTON_POS.y, EXIT_BUTTON_SIZE.x, EXIT_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, closeGameButtons, GAME_OVER_EXIT_FRAMES, NORMAL), goBackMainMenuFn, basicButtonHoverFn);

    clearEmptyBuffer(terminal);
    return res;
}

// Level selector
static void levelSelectorButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->leftSwitchButton), mousePos)) {
        if (leftClick) {
            buttonClick(state->leftSwitchButton);
            buttonReset(state->leftSwitchButton);
        }
        buttonHover(state->leftSwitchButton);

    } else {
        buttonReset(state->leftSwitchButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->rightSwitchButton), mousePos)) {
        if (leftClick) {
            buttonClick(state->rightSwitchButton);
            buttonReset(state->rightSwitchButton);
        }

        buttonHover(state->rightSwitchButton);

    } else {
        buttonReset(state->rightSwitchButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->playButton), mousePos)) {
        if (leftClick) {
            buttonClick(state->playButton);
            buttonReset(state->playButton);
        }

        buttonHover(state->playButton);

    } else {
        buttonReset(state->playButton);
    }
}

static void levelSelectorAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->levelSelectLogo, terminal, LEVEL_SELECTOR_POS.x, LEVEL_SELECTOR_POS.y, false);
    drawSpriteXPM(state->levels[currentLevel], terminal, LEVEL_DISPLAY_POS.x, LEVEL_DISPLAY_POS.y, false);
    buttonDraw(state->leftSwitchButton, terminal, deltaTime, false);
    buttonDraw(state->rightSwitchButton, terminal, deltaTime, false);
    buttonDraw(state->playButton, terminal, deltaTime, false);
    levelSelectorButtonsHandler(state);
}

static void levelSelectorFree(GameState* state) {
    freeSpriteXPM(state->levelSelectLogo);
    for (int i = 0; i < MAX_LVL; i++) freeSpriteXPM(state->levels[i]);
    free(state->levels);
    buttonFree(state->leftSwitchButton);
    buttonFree(state->rightSwitchButton);
    buttonFree(state->playButton);
    free(state);
}

static GameState* levelSelectorTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;

    if (buttonIsFinished(state->playButton) && pressedToPlay) {
        newState = runningGameNew(terminal);
        resetBools();

    } else if (getKeyDown(getInputHandler(), KEYCODE_ESQ)) {
        newState = mainMenuNew(terminal);
        resetBools();
    }

    return newState;
}

static GameState* levelSelectorNew(Terminal* terminal) {
    GameState* state = gameStateNew(levelSelectorAct, levelSelectorFree, levelSelectorTransition);
    currentLevel = 0;
    state->levels = malloc(sizeof(SpriteXPM*) * MAX_LVL);

    state->levels[0] = newSpriteXPM((xpm_map_t)map1XPM[SMALL_IMAGE_INDEX], XPM_8_8_8, 0, 0);
    state->levels[1] = newSpriteXPM((xpm_map_t)map2XPM[SMALL_IMAGE_INDEX], XPM_8_8_8, 0, 0);

    state->levels[2] = newSpriteXPM((xpm_map_t)smallMap3, XPM_8_8_8, 0, 0);

    state->levelSelectLogo = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_LOGO], XPM_8_8_8, 0, 0);

    SpriteXPM** leftButtons = malloc(sizeof(SpriteXPM*) * 2);
    SpriteXPM** rightButtons = malloc(sizeof(SpriteXPM*) * 2);
    SpriteXPM** playButtons = malloc(sizeof(SpriteXPM*) * 2);

    // Left Button
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_BTN_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_BTN_PRESSED], XPM_8_8_8, 0, 0);
    leftButtons[0] = buttonNormal;
    leftButtons[1] = buttonPressed;
    state->leftSwitchButton = buttonNew(LEFT_SWITCH_BUTTON_POS.x, LEFT_SWITCH_BUTTON_POS.y, RED_BUTTON_SIZE.x, RED_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, leftButtons, 2, NORMAL), leftButtonFn, basicButtonHoverFn);

    // Right Button
    buttonNormal = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_BTN_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_BTN_PRESSED], XPM_8_8_8, 0, 0);
    rightButtons[0] = buttonNormal;
    rightButtons[1] = buttonPressed;
    state->rightSwitchButton = buttonNew(RIGHT_SWITCH_BUTTON_POS.x, RIGHT_SWITCH_BUTTON_POS.y, RED_BUTTON_SIZE.x, RED_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, rightButtons, 2, NORMAL), rightButtonFn, basicButtonHoverFn);

    // Play Button
    buttonNormal = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_PLAY_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)levelSelectorXPM[LVL_SELECT_PLAY_PRESSED], XPM_8_8_8, 0, 0);
    playButtons[0] = buttonNormal;
    playButtons[1] = buttonPressed;
    state->playButton = buttonNew(PLAY_BUTTON_POS.x, PLAY_BUTTON_POS.y, PLAY_BUTTON_SIZE.x, PLAY_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, playButtons, 2, NORMAL), selectButtonFn, basicButtonHoverFn);

    return state;
}

// Win Menu
static void winButtonHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->goBackMainMenuButton), mousePos)) {
        if (leftClick) buttonClick(state->goBackMainMenuButton);
        buttonHover(state->goBackMainMenuButton);

    } else {
        buttonReset(state->goBackMainMenuButton);
    }
}

static void winMenuAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->winMenu, terminal, WIN_INFO_POSITION.x, WIN_INFO_POSITION.y, false);
    buttonDraw(state->goBackMainMenuButton, terminal, deltaTime, false);
    winButtonHandler(state);
}

static void winMenuFree(GameState* state) {
    freeSpriteXPM(state->winMenu);
    buttonFree(state->goBackMainMenuButton);
    free(state);
}

static GameState* winMenuTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;

    if (buttonIsFinished(state->goBackMainMenuButton) && pressedGoBack) {
        newState = mainMenuNew(terminal);
        resetBools();
    }

    return newState;
}

static GameState* winMenuNew(Terminal* terminal) {
    GameState* state = gameStateNew(winMenuAct, winMenuFree, winMenuTransition);
    state->winMenu = newSpriteXPM((xpm_map_t)winMenuXPM[WIN_BACKGROUND], XPM_8_8_8, 0, 0);
    SpriteXPM** goBackButton = malloc(sizeof(SpriteXPM*) * 2);

    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)winMenuXPM[WIN_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)winMenuXPM[WIN_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    goBackButton[0] = buttonNormal;
    goBackButton[1] = buttonPressed;
    state->goBackMainMenuButton = buttonNew(WIN_BUTTON_POSITION.x, WIN_BUTTON_POSITION.y, GO_BACK_POS.x, GO_BACK_POS.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, goBackButton, 2, NORMAL), goBackMainMenuFn, basicButtonHoverFn);

	clearEmptyBuffer(terminal);
    return state;
}

// GameLoop
static void runningGameFree(GameState* state) {
    buttonFree(state->resumeButton);
    buttonFree(state->goBackMainMenuButton);
    buttonFree(state->helpButton);
    freeSpriteXPM(state->pauseMenu);
    freeSpriteXPM(state->howToPlay);
    arenaFree(state->arena);

    free(state);
}

static GameState* runningGameTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    if (getKeyDown(getInputHandler(), KEYCODE_ESQ)) {
        pressedPause = true;
    }

    ArenaOutcome out = arenaGetOutcome(state->arena);
    if (out != ARENA_OUTCOME_PLAYING) {
        resetBools();
        if (out == ARENA_OUTCOME_WIN) {
            newState = winMenuNew(terminal);
        } else {
            newState = gameOverMenuNew(terminal);
        }

    } else if (buttonIsFinished(state->goBackMainMenuButton) && pressedGoBack) {
        resetBools();
        newState = mainMenuNew(terminal);
    }
    return newState;
}

static void pausedMenuButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->resumeButton), mousePos)) {
        if (leftClick) buttonClick(state->resumeButton);
        buttonHover(state->resumeButton);

    } else {
        buttonReset(state->resumeButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->helpButton), mousePos)) {
        if (leftClick) buttonClick(state->helpButton);
        buttonHover(state->helpButton);

    } else {
        buttonReset(state->helpButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->goBackMainMenuButton), mousePos)) {
        if (leftClick) buttonClick(state->goBackMainMenuButton);
        buttonHover(state->goBackMainMenuButton);

    } else {
        buttonReset(state->goBackMainMenuButton);
    }
}

static void howToPlayHandleMenu(GameState* state, Terminal* terminal) {
    if (getKeyDown(getInputHandler(), KEYCODE_ESQ)) {
        pressedHowToPlay = false;
    }
}

static void runningGameAct(GameState* state, Terminal* terminal, float deltaTime) {
    if (pressedPause && !pressedHowToPlay) {
        drawSpriteXPM(state->pauseMenu, terminal, PAUSE_MENU_BG_POS.x, PAUSE_MENU_BG_POS.y, false);
        buttonDraw(state->resumeButton, terminal, deltaTime, false);
        buttonDraw(state->helpButton, terminal, deltaTime, false);
        buttonDraw(state->goBackMainMenuButton, terminal, deltaTime, false);
        pausedMenuButtonsHandler(state);

    } else if (pressedHowToPlay) {
        drawSpriteXPM(state->howToPlay, terminal, HOW_TO_PLAY_POSITION.x, HOW_TO_PLAY_POSITION.y, false);
        howToPlayHandleMenu(state, terminal);

    } else {
        arenaUpdate(state->arena, terminal, deltaTime);
    }
}

void pathListFree(void* path) {
    Path* p = *(Path**)path;
    pathFree(p);
}

void monsterGenListFree(void* path) {
    MonsterGen* p = *(MonsterGen**)path;
    monsterGenFree(p);
}

static void push_three_wave_monster_gens(Vector* monsterGenList, Path* p) {
    MonsterGen* mg;
    mg = monsterGenNew(p, pawnMonsterCreator, waveNoAutoSpawnGenTimeFunc);
    vectorPushBack(monsterGenList, (char*)&mg);
    mg = monsterGenNew(p, alienMonsterCreator, waveNoAutoSpawnGenTimeFunc);
    vectorPushBack(monsterGenList, (char*)&mg);
    mg = monsterGenNew(p, spearManMonsterCreator, waveNoAutoSpawnGenTimeFunc);
    vectorPushBack(monsterGenList, (char*)&mg);
}

static void createArena(GameState* state, Terminal* terminal) {
    const char* map;
    const char* path;
    const char* lake;
    char** background;
    Vector* paths = vectorCreate(sizeof(Path*), pathListFree);
    Path* p;
    Vector* monsterGenList = vectorCreate(sizeof(MonsterGen*), monsterGenListFree);
    int bananaStart = 10;

    switch (currentLevel) {
        case 0:
            map = map1;
            path = path1;
            lake = lake1;
            background = background1;
            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 33 * 2, -50.0, 352.0, 0.0, 256.0, 32.0, 256.0, 64.0, 256.0, 96.0, 256.0, 128.0, 256.0, 160.0, 256.0, 192.0, 256.0, 224.0, 256.0, 256.0, 256.0, 256.0, 288.0, 288.0, 288.0, 320.0, 288.0, 352.0, 288.0, 384.0, 288.0, 384.0, 256.0, 416.0, 256.0, 416.0, 224.0, 448.0, 224.0, 480.0, 224.0, 512.0, 224.0, 544.0, 224.0, 544.0, 256.0, 576.0, 256.0, 576.0, 288.0, 576.0, 320.0, 608.0, 320.0, 640.0, 320.0, 672.0, 320.0, 704.0, 320.0, 736.0, 320.0, 768.0, 320.0, 810.0, 320.0));
            vectorPushBack(paths, (char*)&p);
            push_three_wave_monster_gens(monsterGenList, p);

            break;
        case 1:
            map = map2;
            path = path2;
            lake = lake2;
            background = background2;
            bananaStart = 25;

            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 35 * 2, -50.0, 352.0, 32.0, 352.0, 64.0, 352.0, 64.0, 320.0, 96.0, 320.0, 96.0, 288.0, 128.0, 288.0, 160.0, 288.0, 192.0, 288.0, 224.0, 288.0, 224.0, 320.0, 224.0, 352.0, 256.0, 352.0, 256.0, 384.0, 256.0, 416.0, 256.0, 448.0, 288.0, 448.0, 320.0, 448.0, 352.0, 448.0, 384.0, 448.0, 416.0, 448.0, 448.0, 448.0, 480.0, 448.0, 512.0, 448.0, 512.0, 480.0, 544.0, 480.0, 576.0, 480.0, 608.0, 480.0, 608.0, 512.0, 640.0, 512.0, 672.0, 512.0, 704.0, 512.0, 736.0, 512.0, 800.0, 512.0, 801.0, 512.0));
            vectorPushBack(paths, (char*)&p);

            push_three_wave_monster_gens(monsterGenList, p);

            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 34 * 2, -50.0, 352.0, 32.0, 352.0, 64.0, 352.0, 64.0, 320.0, 96.0, 320.0, 96.0, 288.0, 128.0, 288.0, 160.0, 288.0, 192.0, 288.0, 224.0, 288.0, 224.0, 256.0, 224.0, 224.0, 256.0, 224.0, 288.0, 224.0, 320.0, 224.0, 352.0, 224.0, 384.0, 224.0, 416.0, 224.0, 448.0, 224.0, 480.0, 224.0, 512.0, 224.0, 544.0, 224.0, 544.0, 192.0, 576.0, 192.0, 576.0, 160.0, 576.0, 128.0, 576.0, 96.0, 608.0, 96.0, 640.0, 96.0, 672.0, 96.0, 704.0, 96.0, 736.0, 96.0, 800.0, 96.0, 801.0, 96.0));
            vectorPushBack(paths, (char*)&p);
            push_three_wave_monster_gens(monsterGenList, p);

            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 18 * 2, -50.0, 160.0, 32.0, 160.0, 64.0, 160.0, 96.0, 160.0, 128.0, 160.0, 160.0, 160.0, 192.0, 160.0, 224.0, 160.0, 224.0, 128.0, 256.0, 128.0, 256.0, 96.0, 288.0, 96.0, 320.0, 96.0, 320.0, 64.0, 320.0, 32.0, 352.0, 32.0, 352.0, -50.0, 352.0, -60.0));
            vectorPushBack(paths, (char*)&p);
            push_three_wave_monster_gens(monsterGenList, p);

            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 31 * 2, -50.0, 160.0, 32.0, 160.0, 64.0, 160.0, 96.0, 160.0, 128.0, 160.0, 160.0, 160.0, 192.0, 160.0, 224.0, 160.0, 224.0, 192.0, 224.0, 224.0, 256.0, 224.0, 288.0, 224.0, 320.0, 224.0, 352.0, 224.0, 384.0, 224.0, 416.0, 224.0, 448.0, 224.0, 480.0, 224.0, 512.0, 224.0, 544.0, 224.0, 544.0, 192.0, 576.0, 192.0, 576.0, 160.0, 576.0, 128.0, 576.0, 96.0, 576.0, 64.0, 544.0, 64.0, 544.0, 32.0, 544.0, 0.0, 512.0, -50.0, 512.0, -60.0));
            vectorPushBack(paths, (char*)&p);
            push_three_wave_monster_gens(monsterGenList, p);
 
            break;
        case 2:
            map = map3;
            path = path3;
            lake = lake3;
            background = background3;
            bananaStart=80;

             
 
             
             MonsterGen* mg;

          

            p = pathNew(4, 
                vectorCreateWithFloatingData(sizeof(float), NULL,
                 48 * 2,
                 -50.0, 480.0, 32.0, 480.0, 64.0, 480.0, 64.0, 448.0, 96.0, 448.0, 96.0, 416.0, 128.0, 416.0, 128.0, 384.0, 160.0, 384.0, 160.0, 352.0, 160.0, 320.0, 192.0, 320.0, 192.0, 288.0, 224.0, 288.0, 224.0, 256.0, 256.0, 256.0, 256.0, 224.0, 256.0, 192.0, 288.0, 192.0, 288.0, 160.0, 320.0, 160.0, 352.0, 160.0, 352.0, 128.0, 384.0, 128.0, 416.0, 128.0, 448.0, 128.0, 480.0, 128.0, 480.0, 160.0, 512.0, 160.0, 544.0, 160.0, 544.0, 192.0, 576.0, 192.0, 576.0, 224.0, 576.0, 256.0, 608.0, 256.0, 608.0, 288.0, 640.0, 288.0, 640.0, 320.0, 672.0, 320.0, 672.0, 352.0, 704.0, 352.0, 704.0, 384.0, 704.0, 416.0, 736.0, 416.0, 736.0, 448.0, 768.0, 448.0, 800.0, 480.0, 801.0, 480.0

                 ));

        
            vectorPushBack(paths, (char*)&p);
            mg = monsterGenNew(p, pawnMonsterCreator, constantSlowGenTimeFunc_1);
            vectorPushBack(monsterGenList, (char*)&mg);
             mg = monsterGenNew(p, alienMonsterCreator, progressiveTimeFunction100_1);
            vectorPushBack(monsterGenList, (char*)&mg);

            mg = monsterGenNew(p, spearManMonsterCreator, progressiveTimeFunction200_1);
            vectorPushBack(monsterGenList, (char*)&mg);

            p = pathNew(4, 
                vectorCreateWithFloatingData(sizeof(float), NULL,
                 48 * 2,
                 -50.0, 128.0, 32.0, 128.0, 64.0, 128.0, 64.0, 160.0, 96.0, 160.0, 96.0, 192.0, 128.0, 192.0, 128.0, 224.0, 160.0, 224.0, 160.0, 256.0, 160.0, 288.0, 192.0, 288.0, 192.0, 320.0, 224.0, 320.0, 224.0, 352.0, 256.0, 352.0, 256.0, 384.0, 256.0, 416.0, 288.0, 416.0, 288.0, 448.0, 320.0, 448.0, 352.0, 448.0, 352.0, 480.0, 384.0, 480.0, 416.0, 480.0, 448.0, 480.0, 480.0, 480.0, 480.0, 448.0, 512.0, 448.0, 544.0, 448.0, 544.0, 416.0, 576.0, 416.0, 576.0, 384.0, 576.0, 352.0, 608.0, 352.0, 608.0, 320.0, 640.0, 320.0, 640.0, 288.0, 672.0, 288.0, 672.0, 256.0, 704.0, 256.0, 704.0, 224.0, 704.0, 192.0, 736.0, 192.0, 736.0, 160.0, 768.0, 160.0, 800.0, 128.0, 801.0, 128.0

                 ));

            vectorPushBack(paths, (char*)&p);
            mg = monsterGenNew(p, pawnMonsterCreator, constantSlowGenTimeFunc_1);
            vectorPushBack(monsterGenList, (char*)&mg);
             mg = monsterGenNew(p, alienMonsterCreator, progressiveTimeFunction100_1);
            vectorPushBack(monsterGenList, (char*)&mg);

            mg = monsterGenNew(p, spearManMonsterCreator, progressiveTimeFunction200_1);
            vectorPushBack(monsterGenList, (char*)&mg);

            break;
        default:
            map = map1;
            path = path1;
            lake = lake1;
            background = background1;
            p = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 33 * 2, -50.0, 352.0, 0.0, 256.0, 32.0, 256.0, 64.0, 256.0, 96.0, 256.0, 128.0, 256.0, 160.0, 256.0, 192.0, 256.0, 224.0, 256.0, 256.0, 256.0, 256.0, 288.0, 288.0, 288.0, 320.0, 288.0, 352.0, 288.0, 384.0, 288.0, 384.0, 256.0, 416.0, 256.0, 416.0, 224.0, 448.0, 224.0, 480.0, 224.0, 512.0, 224.0, 544.0, 224.0, 544.0, 256.0, 576.0, 256.0, 576.0, 288.0, 576.0, 320.0, 608.0, 320.0, 640.0, 320.0, 672.0, 320.0, 704.0, 320.0, 736.0, 320.0, 768.0, 320.0, 810.0, 320.0));
            vectorPushBack(paths, (char*)&p);
            push_three_wave_monster_gens(monsterGenList, p);

            break;
    }

    state->arena = arenaNew(terminal, map, path, lake, background, paths, monsterGenList, bananaStart, currentLevel);
}

GameState* runningGameNew(Terminal* terminal) {
    GameState* res = gameStateNew(runningGameAct, runningGameFree, runningGameTransition);

    createArena(res, terminal);
    res->pauseMenu = newSpriteXPM((xpm_map_t)pauseMenuXPM[PAUSE_MENU_INDEX], XPM_8_8_8, 0, 0);
    res->howToPlay = newSpriteXPM((xpm_map_t)howToPlay, XPM_8_8_8, 0, 0);

    // Resume Button
    SpriteXPM** resumeSprites = malloc(sizeof(SpriteXPM*) * 2);
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)iconsXPM[RESUME_NORMAL_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)iconsXPM[RESUME_PRESSED_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    resumeSprites[0] = buttonNormal;
    resumeSprites[1] = buttonPressed;

    res->resumeButton = buttonNew(RESUME_POS.x, RESUME_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, resumeSprites, 2, NORMAL), resumeFn, basicButtonHoverFn);

    // Help button
    SpriteXPM** helpSprites = malloc(sizeof(SpriteXPM*) * 2);
    buttonNormal = newSpriteXPM((xpm_map_t)iconsXPM[HELP_NORMAl_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)iconsXPM[HELP_PRESSED_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    helpSprites[0] = buttonNormal;
    helpSprites[1] = buttonPressed;

    res->helpButton = buttonNew(HELP_POS.x, HELP_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, helpSprites, 2, NORMAL), helpFn, basicButtonHoverFn);

    // Go back button
    SpriteXPM** goBackMainMenuSprites = malloc(sizeof(SpriteXPM*) * 2);
    buttonNormal = newSpriteXPM((xpm_map_t)iconsXPM[GO_BACK_NORMAl_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)iconsXPM[GO_BACK_PRESSED_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    goBackMainMenuSprites[0] = buttonNormal;
    goBackMainMenuSprites[1] = buttonPressed;

    res->goBackMainMenuButton = buttonNew(GO_BACK_POS.x, GO_BACK_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, goBackMainMenuSprites, 2, NORMAL), goBackMainMenuFn, basicButtonHoverFn);

    return res;
}

// Main Menu
static void mainMenuButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->newGameButton), mousePos)) {
        if (leftClick) buttonClick(state->newGameButton);
        buttonHover(state->newGameButton);

    } else {
        buttonReset(state->newGameButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->closeGameButton), mousePos)) {
        if (leftClick) buttonClick(state->closeGameButton);
        buttonHover(state->closeGameButton);

    } else {
        buttonReset(state->closeGameButton);
    }
}

static void mainMenuAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->logo, terminal, LOGO_POSITION.x, LOGO_POSITION.y, false);
    buttonDraw(state->newGameButton, terminal, deltaTime, false);
    buttonDraw(state->closeGameButton, terminal, deltaTime, false);
    mainMenuButtonsHandler(state);
}

static void mainMenuFree(GameState* state) {
    free(state->logo);
    buttonFree(state->newGameButton);
    buttonFree(state->closeGameButton);
    free(state);
}

static GameState* mainMenuTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    // Wait for the animation to finish first - if there is one
    if (buttonIsFinished(state->newGameButton) && pressedNewGame) {
        newState = levelSelectorNew(terminal);
        resetBools();

    } else if (buttonIsFinished(state->closeGameButton) && pressedExit) {
        newState = NULL;
        resetBools();
    }

    return newState;
}

GameState* mainMenuNew(Terminal* terminal) {
    pressedNewGame = false;
    pressedExit = false;

    GameState* res = gameStateNew(mainMenuAct, mainMenuFree, mainMenuTransition);
    SpriteXPM** newGameButtons = malloc(sizeof(SpriteXPM*) * NEW_GAME_BUTTON_FRAMES);
    SpriteXPM** closeGameButtons = malloc(sizeof(SpriteXPM*) * EXIT_BUTTON_FRAMES);
    res->logo = newSpriteXPM((xpm_map_t)mainMenuXPM[LOGO], XPM_8_8_8, 0, 0);

    // New Game Button
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    newGameButtons[0] = buttonNormal;
    newGameButtons[1] = buttonPressed;

    res->newGameButton = buttonNew(NEW_GAME_BUTTON_POS.x, NEW_GAME_BUTTON_POS.y, NEW_GAME_BUTTON_SIZE.x, NEW_GAME_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, newGameButtons, NEW_GAME_BUTTON_FRAMES, NORMAL), newGameFn, basicButtonHoverFn);

    // Exit Button
    buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    closeGameButtons[0] = buttonNormal;
    closeGameButtons[1] = buttonPressed;
    res->closeGameButton = buttonNew(EXIT_BUTTON_POS.x, EXIT_BUTTON_POS.y, EXIT_BUTTON_SIZE.x, EXIT_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, closeGameButtons, EXIT_BUTTON_FRAMES, NORMAL), exitGameFn, basicButtonHoverFn);

    clearEmptyBuffer(terminal);
    return res;
}
