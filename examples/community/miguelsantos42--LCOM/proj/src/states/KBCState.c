#include "src/include/states/KBCState.h"
#include "src/include/states/MenuState.h"
#include "src/include/Macros.h"
#include "src/include/states/GameState.h"
#include "src/include/states/SettingsState.h"
#include "src/include/states/CharacterState.h"

enum ImportantKeys{
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyAction,
    KeyEscape,
    KeyNone
};

struct KeyboardState{
    uint8_t keysPressed;
    uint8_t keysState;
    bool movementKeyPressed;
    bool (*updateState)(struct KeyboardState*, const KeyboardInput*);
    void (*nextFrame)(struct KeyboardState*);
    void (*delete)(struct KeyboardState*);
};

typedef struct KeyboardState KeyboardState;

static void (deleteKeyboardState)(struct KeyboardState *state){
    if(state) free(state);
}

static bool (updateKeyboardState)(struct KeyboardState *state, const KeyboardInput *input){
    printf("Updating Keyboard State\n");
    if(!state || !input) return false;
    const char *key = input->getKey(input);
    enum ImportantKeys mappedKey = KeyNone;
    bool released = input->wasReleased(input);
    if(strcmp(key,"LetterW") == 0 || strcmp(key,"ArrowUp") == 0) mappedKey = KeyUp;
    else if(strcmp(key,"LetterS") == 0 || strcmp(key,"ArrowDown") == 0) mappedKey = KeyDown;
    else if(strcmp(key,"LetterA") == 0 || strcmp(key,"ArrowLeft") == 0) mappedKey = KeyLeft;
    else if(strcmp(key,"LetterD") == 0 || strcmp(key,"ArrowRight") == 0) mappedKey = KeyRight;
    else if(strcmp(key,"Space") == 0 || strcmp(key,"Enter") == 0) mappedKey = KeyAction;
    else if(strcmp(key,"Escape") == 0) mappedKey = KeyEscape;
    printf("Key pressed: %d\n", mappedKey);
    if(mappedKey!=KeyNone){
        if(released) state->keysState &= ~BIT(mappedKey);
        else{
            if(mappedKey == KeyEscape){
                if(gameState == GameRunning || gameState == GameInstructions || gameState == GameSettings) {
                    gameState = GameMenu;
                    return false;
                } else { return true; }
            };
            if(!(state->keysPressed&BIT(mappedKey))||gameState == GameMenu){
                state->keysPressed |= BIT(mappedKey); 
                if(playableCharacterState) {
                    switch (mappedKey)
                    {
                    case KeyUp:
                        if(gameState == GameMenu){
                            menuState->setUp(menuState);
                            break;
                        }
                        if(gameState == GameSettings){
                            settingsState->setUp(settingsState);
                            break;
                        }
                        if(state->movementKeyPressed) break;
                        playableCharacterState->move(playableCharacterState, ScreenUp);
                        state->movementKeyPressed = true;
                        break;
                    case KeyDown:
                        if(gameState == GameMenu){
                            menuState->setDown(menuState);
                            break;
                        }
                        if(gameState == GameSettings){
                            settingsState->setDown(settingsState);
                            break;
                        }
                        if(state->movementKeyPressed) break;
                        playableCharacterState->move(playableCharacterState, ScreenDown);
                        state->movementKeyPressed = true;
                        break;
                    case KeyRight:
                        if(state->movementKeyPressed) break;
                        playableCharacterState->move(playableCharacterState, ScreenRight);
                        state->movementKeyPressed = true;
                        break;
                    case KeyLeft:
                        if(state->movementKeyPressed) break;
                        playableCharacterState->move(playableCharacterState, ScreenLeft);
                        state->movementKeyPressed = true;
                        break;
                    case KeyAction:
                        if(gameState == GameMenu){
                            menuState->select(menuState);
                            break;
                        }
                        if(gameState == GameSettings){
                            settingsState->select(settingsState);
                            break;
                        }
                        shootBullet(playableCharacterState->getDirection(playableCharacterState));
                    default:
                        break;
                    }
                }
            }
            state->keysState |= BIT(mappedKey);
        }
    }
    return false;
}

static void (nextKeyboardFrame)(struct KeyboardState *state){
    if(!state) return;
    static uint8_t internalCounter = 0;
    if(++internalCounter%10!=0) return;
    internalCounter = 0;
    state->keysPressed = state->keysState;
    state->movementKeyPressed = false;
    if(playableCharacterState){
        if(state->keysPressed&BIT(KeyUp)) {
            state->movementKeyPressed = true;
            playableCharacterState->move(playableCharacterState, ScreenUp);
        }else if(state->keysPressed&BIT(KeyDown)) {
            state->movementKeyPressed = true;
            playableCharacterState->move(playableCharacterState, ScreenDown);
        }else if(state->keysPressed&BIT(KeyRight)) {
            state->movementKeyPressed = true;
            playableCharacterState->move(playableCharacterState, ScreenRight);
        }else if(state->keysPressed&BIT(KeyLeft)) {
            state->movementKeyPressed = true;
            playableCharacterState->move(playableCharacterState, ScreenLeft);
        }
    }
}

static KeyboardState *(keyboardStateConstructor)(){
    KeyboardState *state = malloc(sizeof(KeyboardState));
    if(!state) return NULL;
    state->movementKeyPressed = false;
    state->keysPressed = 0;
    state->keysState = 0;
    state->updateState = updateKeyboardState;
    state->nextFrame = nextKeyboardFrame;
    state->delete = deleteKeyboardState;
    return state;
}

enum MouseButtons{
    MouseLeft,
    MouseMiddle,
    MouseRight
};

struct MouseStateKBC{
    int16_t x;
    int16_t y;
    enum ScreenDirection direction;
    bool buttonsPressed[3]; 
    bool buttonsState[3];
    void (*updateState)(struct MouseStateKBC*, const MouseInput*);
    void (*nextFrame)(struct MouseStateKBC*);
    void (*delete)(struct MouseStateKBC*);
};

typedef struct MouseStateKBC MouseStateKBC;

static void (deleteMouseState)(struct MouseStateKBC* state){
    if(state) free(state);
}

static void (updateMouseState)(struct MouseStateKBC *state, const MouseInput *input){
    if(!state || !input) return;
    int16_t rawDeltaX = input->getX(input);
    int16_t rawDeltaY = input->getY(input);
    int32_t newX = (int32_t)state->x + rawDeltaX;
    int32_t newY = (int32_t)state->y - rawDeltaY; 
    int32_t maxX = 1024 - 16;
    int32_t maxY = 768 - 16;
    if (newX < 0)       newX = 0;
    else if (newX > maxX) newX = maxX;
    if (newY < 0)       newY = 0;
    else if (newY > maxY) newY = maxY;
    state->x = (uint16_t)newX;
    state->y = (uint16_t)newY;
    const int cx = 1024 / 2;
    const int cy = 768 / 2;
    int dx = (int)newX - cx;
    int dy =  cy    - (int)newY;
    if      (dy >  dx && dy > -dx) state->direction = ScreenUp; 
    else if (dx >  dy && dx > -dy) state->direction = ScreenRight;
    else if (-dy > dx && -dy > -dx) state->direction = ScreenDown; 
    else state->direction = ScreenLeft;

    bool buttons[3] = {input->leftPressed(input), input->middlePressed(input), input->rightPressed(input)};
    for(enum MouseButtons i = MouseLeft; i <= MouseRight; i++){
        if(buttons[i]){
            if(!state->buttonsPressed[i]) {
                state->buttonsPressed[i] = true;
                if(i == 0){
                    if(gameState == GameMenu){
                        menuState->select(menuState);
                        continue;;
                    }
                    if(gameState == GameSettings){
                        settingsState->select(settingsState);
                        continue;;
                    }
                }
            }
            state->buttonsState[i] = true;
        }else state->buttonsState[i] = false;
    }

}

static void (nextMouseFrame)(struct MouseStateKBC *state){
    if(!state) return;
    mouseState->setPosition(mouseState, state->x, state->y);
    if(gameState == GameMenu) menuState->setMouse(menuState,state->x,state->y);
    else if(gameState == GameSettings) settingsState->setMouse(settingsState,state->x,state->y);
    static uint8_t internalCounter = 0;
    if(++internalCounter%10!=0) return;
    internalCounter = 0;

    for(enum MouseButtons i = MouseLeft; i <= MouseRight; i++) {
        state->buttonsPressed[i] = state->buttonsState[i];
        if(i == 0 && state->buttonsState[i]){
            if(gameState == GameMenu){
                menuState->select(menuState);
                continue;
            }
            if(gameState == GameSettings){
                settingsState->select(settingsState);
                continue;
            }
        }
    }
}


static MouseStateKBC *(mouseStateKBCConstructor)(){
    MouseStateKBC *state = malloc(sizeof(MouseStateKBC));
    if(!state) return NULL;
    for(enum MouseButtons i = MouseLeft; i <= MouseRight; i++){
        state->buttonsPressed[i] = 0;
        state->buttonsState[i] = 0;
    }
    state->x = (1024/2)-8;
    state->y = (768/2)-8;
    state->updateState = updateMouseState;
    state->nextFrame = nextMouseFrame;
    state->delete = deleteMouseState;
    return state;
}

struct KBCStateData{
    KeyboardState *keyboardState;
    MouseStateKBC *mouseState;
};

typedef struct KBCStateData KBCStateData;

static void (destroyKBCStateData)(struct KBCStateData *data){
    if(!data) return;
    data->keyboardState->delete(data->keyboardState);
    data->mouseState->delete(data->mouseState);
    free(data);
}

static void (destroyKBCState)(struct KBCState *state){
    if(!state) return;
    destroyKBCStateData(state->data);
    free(state);
}

static bool (updateKBCState)(struct KBCState *state, const KBCInput *input){
    if(!input) return false;
    KBCStateData *data;
    getData(state,data,false);
    if(input->isAux(input)){
        if(data->mouseState) data->mouseState->updateState(data->mouseState, input->getMouseInput(input));
        return false;
    }
    if(data->keyboardState) return data->keyboardState->updateState(data->keyboardState, input->getKeyboardInput(input));
    return false;
}
static void (nextKBCFrame)(struct KBCState *state){
    KBCStateData *data;
    getVoidData(state,data);
    if(data->keyboardState) data->keyboardState->nextFrame(data->keyboardState);
    if(data->mouseState) data->mouseState->nextFrame(data->mouseState);
}

KBCState *(kBCStateConstructor)(){
    KBCStateData *data = malloc(sizeof(KBCStateData));
    if(!data) return NULL;
    data->keyboardState = keyboardStateConstructor();
    data->mouseState = mouseStateKBCConstructor();
    KBCState *state = malloc(sizeof(KBCState));
    state->data = data;
    state->updateState = updateKBCState;
    state->nextFrame = nextKBCFrame;
    state->destroy = destroyKBCState;
    return state;
}
