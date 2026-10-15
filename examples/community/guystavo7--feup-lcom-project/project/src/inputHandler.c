#include "../include/utils/inputHandler.h"
/** @file inputHandler.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include <stdint.h>
#include <stdlib.h>

enum keyState {
    KEYUP,
    KEYDOWN,
    KEYPRESSED
};

struct InputHandler {
    enum keyState keys[88];
};

static InputHandler* newInputHandler() {
    InputHandler* i = malloc(sizeof(InputHandler));
    return i;
}

InputHandler* getInputHandler() {
    if (!isInputHandlerCreated) {
        inputHandler = newInputHandler();
		for (int i = 0; i < 88; i++) inputHandler->keys[i] = KEYUP;
		isInputHandlerCreated = true;
    }

    return inputHandler;
}

void readKey(InputHandler* inputHandler, uint16_t keycode, bool isMake) {
	enum keyState currentState = inputHandler->keys[keycode - KEYCODE_ESQ];

    if (isMake && currentState == KEYUP) {
        inputHandler->keys[keycode - KEYCODE_ESQ] = KEYDOWN;
    } else if (!isMake){
        inputHandler->keys[keycode - KEYCODE_ESQ] = KEYUP;
    }
}

bool getKeyDown(InputHandler* inputHandler, uint16_t keycode) {
	bool isKeyDown = inputHandler->keys[keycode - KEYCODE_ESQ] == KEYDOWN;
	if (isKeyDown) inputHandler->keys[keycode - KEYCODE_ESQ] = KEYPRESSED;

	return isKeyDown;
}

bool getKeyPressed(InputHandler* inputHandler, uint16_t keycode) {
	return inputHandler->keys[keycode - KEYCODE_ESQ] != KEYUP;
}
