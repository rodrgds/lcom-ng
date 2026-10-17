#include "src/include/states/states.h"
#include "src/include/states/KBCState.h"
#include "src/include/controllers/video/graphics.h"
#include "src/include/states/MenuState.h"
#include "src/include/states/InstructionsState.h"
#include "src/include/states/GameState.h"
#include "src/include/states/SettingsState.h"

CharacterState *playableCharacterState;
static KBCState *kBCState = NULL;
CharacterState* characters[10000];
uint16_t characterNum;

extern bool game_over;
extern uint8_t bulletCount;
extern uint8_t lifeCount;
extern uint8_t score;

int (makeStates)(){
    kBCState = kBCStateConstructor();
    menuState = menuStateConstructor();
    instructionState = instructionsStateConstructor();
    settingsState = settingsStateConstructor();
    if(!kBCState) return 1;
    if(!menuState) return 1;
    if(!instructionState) return 1;
    if(!settingsState) return 1;
    return 0;
}

void destroyStates(){
    if(kBCState) kBCState->destroy(kBCState);
    if(menuState) menuState->destroy(menuState);
    if(instructionState) instructionState->destroy(instructionState);
    if(settingsState) settingsState->destroy(settingsState);
}

void (nextFrame)(){
    reset_buffer();

    if(game_over && gameState == GameRunning){
        gameState = GameMenu;
        reloadMap(getMapByDifficulty(selectedDifficulty));
    }

    if(kBCState) kBCState->nextFrame(kBCState);
    
    
    if (gameState == GameMenu) {
        if(menuState) {
            menuState->draw(menuState); 
            mouseState->draw(mouseState);
        }
        change_buffer();
        return;
    }

    if (gameState == GameInstructions) {
        if(instructionState) {
            instructionState->draw(instructionState);
            mouseState->draw(mouseState);
        }
        change_buffer();
        return;
    }

    if (gameState == GameSettings) {
        if(settingsState){
            settingsState->draw(settingsState);
            mouseState->draw(mouseState);
        }
        change_buffer();
        return;
    }
    
    for(uint16_t i = characterNum; i >= 1; i--) {
        if(characters[i-1] == NULL) continue;
        characters[i-1]->move(characters[i-1], ScreenNo);
        if(characters[i-1] != NULL) characters[i-1]->draw(characters[i-1]);
    }

        for(uint16_t i = 0; i < characterNum; i++) {
            if(characters[i] == NULL) printf("Error\n");
            allowMovement(characters[i]);
        }

    playableCharacterState->draw(playableCharacterState);

    draw_information();
    change_buffer();
}

void (updateKBCState)(const KBCInput *input){
    if(kBCState) if(kBCState->updateState(kBCState, input)) gameState = GameStopping;
}
