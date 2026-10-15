#include "KeyboardInterpreter.h"

/**
 * @brief Zeroes all directional flags of a PlayerInput.
 * @param playerInput PlayerInput to initialise.
 */
static void playerInputInit(PlayerInput *playerInput) {
    playerInput->down = false;
    playerInput->up = false;
    playerInput->left = false;
    playerInput->right = false;
}

void keyboardStateInit(KeyboardState *keyboardState) {
    if (keyboardState == NULL) return;

    playerInputInit(&keyboardState->p1);
    playerInputInit(&keyboardState->p2);

    keyboardState->exit = false;
    keyboardState->shop = false;
}

void keyboardInterpretScancode(KeyboardState *state, uint8_t *bytes, uint8_t size) {
    if (state == NULL || bytes == NULL || size == 0) {
        return;
    }

    /*
     * 1-byte scancodes:
     * Player 1 movement + ESC
     */
    if (size == 1) {
        switch (bytes[0]) {
            case W_MAKE:
                state->p1.up = true;
                break;

            case W_BREAK:
                state->p1.up = false;
                break;

            case A_MAKE:
                state->p1.left = true;
                break;

            case A_BREAK:
                state->p1.left = false;
                break;

            case S_MAKE:
                state->p1.down = true;
                break;

            case S_BREAK:
                state->p1.down = false;
                break;

            case D_MAKE:
                state->p1.right = true;
                break;

            case D_BREAK:
                state->p1.right = false;
                break;

            case ESC_BREAK:
                state->exit = true;
                break;
            case P_MAKE:
                state->shop = true;
                break;
            default:
                break;
        }

        return;
    }

    /*
     * 2-byte scancodes:
     * Player 2 arrow keys
     */
    if (size == 2 && bytes[0] == EXTENDED) {
        switch (bytes[1]) {
            case UP_MAKE:
                state->p2.up = true;
                break;

            case UP_BREAK:
                state->p2.up = false;
                break;

            case LEFT_MAKE:
                state->p2.left = true;
                break;

            case LEFT_BREAK:
                state->p2.left = false;
                break;

            case RIGHT_MAKE:
                state->p2.right = true;
                break;

            case RIGHT_BREAK:
                state->p2.right = false;
                break;

            case DOWN_MAKE:
                state->p2.down = true;
                break;

            case DOWN_BREAK:
                state->p2.down = false;
                break;

            default:
                break;
        }
    }
}
