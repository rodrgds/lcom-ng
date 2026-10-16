#include "MouseInterpreter.h"

/* The original cursor asset is 50x52 and is drawn from its top-left corner. */
enum { CURSOR_WIDTH = 50, CURSOR_HEIGHT = 52 };

void mouseStateInit(MouseState *state, int x, int y) {
    if (state == NULL) {
        return;
    }

    state->x = x;
    state->y = y;

    state->p1.attack = false;
    state->p1.attack_pressed = false;

    state->p2.attack = false;
    state->p2.attack_pressed = false;
}

void mouseInterpretPacket(MouseState *state, struct packet *packet, int screen_w, int screen_h) {
    if (state == NULL || packet == NULL) {
        return;
    }

    /*
     * Mouse delta_y is positive when moving up,
     * but screen y increases downwards.
     */
    state->x += packet->delta_x;
    state->y -= packet->delta_y;

    int max_x = screen_w > CURSOR_WIDTH ? screen_w - CURSOR_WIDTH : 0;
    int max_y = screen_h > CURSOR_HEIGHT ? screen_h - CURSOR_HEIGHT : 0;
    if (state->x < 0) state->x = 0;
    if (state->x > max_x) state->x = max_x;
    if (state->y < 0) state->y = 0;
    if (state->y > max_y) state->y = max_y;

    /*
     * Left click -> Player 1 attack
     */
    if (packet->lb && !state->p1.attack_pressed) {
        state->p1.attack = true;
    }

    state->p1.attack_pressed = packet->lb;

    /*
     * Right click -> Player 2 attack
     */
    if (packet->rb && !state->p2.attack_pressed) {
        state->p2.attack = true;
    }

    state->p2.attack_pressed = packet->rb;
}

void mouseClearEvents(MouseState *state) {
    if (state == NULL) {
        return;
    }

    /*
     * Clear one-frame events only.
     * Keep attack_pressed because that represents holding the button.
     */
    state->p1.attack = false;
    state->p2.attack = false;
}
