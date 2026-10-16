#include "../include/utils/cursorHandler.h"
/** @file cursorHandler.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/cursor/cursor.h"

#include <stdlib.h>

enum keyState {
    UP,
    DOWN,
    PRESSED
};

enum MouseState {
    DEFAULT,
    CLICKED,
    DENIED,
	CONSTRUCTION,
};

struct Cursor {
    int x, y;
    int deltaX, deltaY;
    int width, height;

    enum keyState leftButton;
    enum keyState rightButton;
    enum keyState middleButton;

    enum MouseState currentState;
    SpriteXPM** sprites;

    bool constructionMode;
    bool placementHoverValid;
};

static Cursor* cursorNew(int w, int h) {
    Cursor* res = malloc(sizeof(Cursor));

	res->currentState = DEFAULT;
    res->constructionMode = false;
    res->placementHoverValid = true;
    res->sprites = malloc(sizeof(SpriteXPM*) * CURSOR_FRAMES);
    for (int i = 0; i < CURSOR_FRAMES; i++) res->sprites[i] = newSpriteXPM((xpm_map_t)cursorsXPM[i], XPM_8_8_8, 0, 0);

    res->deltaX = 0;
    res->deltaY = 0;

    res->x = w / 2;
    res->y = h / 2;

    res->width = w;
    res->height = h;

    return res;
}

Cursor* getCursor() {
    if (!isCursorCreated) {
        cursor = cursorNew(SCREEN_WIDTH, SCREEN_HEIGHT);
        isCursorCreated = true;
    }

    return cursor;
}

void cursorFree(Cursor* c) {
    for (int i = 0; i < CURSOR_FRAMES; i++) freeSpriteXPM(c->sprites[i]);
    free(c->sprites);
    free(c);
}

void cursorReset(Cursor* c) {
    c->x = 0;
    c->y = 0;
}

void cursorUpdateData(Cursor* c, int dx, int dy, bool leftButton, bool rightButton, bool middleButton) {
    c->deltaX += dx;
    c->deltaY += dy;

    if (leftButton && c->leftButton == UP) {
        c->leftButton = DOWN;
        c->currentState = CLICKED;

    } else if (!leftButton) {
        c->leftButton = UP;
    }

    if (middleButton && c->middleButton == UP) {
        c->middleButton = DOWN;
    } else if (!middleButton) {
        c->middleButton = UP;
    }

    if (rightButton && c->rightButton == UP) {
        c->rightButton = DOWN;
    } else if (!rightButton) {
        c->rightButton = UP;
    }

    if (!leftButton) c->currentState = DEFAULT;
}

void cursorUpdateCoordinates(Cursor* c) {
    c->x += c->deltaX;
    c->y += c->deltaY;

    c->deltaX = 0;
    c->deltaY = 0;

    if (c->x < 0) c->x = 0;
    if (c->x > c->width - 1) c->x = c->width - 1;

	if (c->y < 0) c->y = 0;
	if (c->y > c->height - 1) c->y = c->height - 1;
}

static void cursorApplyVisualState(Cursor* c) {
    if (c->constructionMode) {
        c->currentState = c->placementHoverValid ? CONSTRUCTION : DENIED;
    } else {
        c->currentState = DEFAULT;
    }
}

void cursorSetConstructionMode(Cursor* c, bool enabled) {
    c->constructionMode = enabled;
    if (!enabled) c->placementHoverValid = true;
}

void cursorSetPlacementHoverValid(Cursor* c, bool valid) {
    c->placementHoverValid = valid;
}

static void cursorDraw(Cursor* c, Terminal* terminal, float deltaTime) {
    /* Placement still uses the full-screen input point; only artwork is inset. */
    SpriteXPM* sprite = c->sprites[c->currentState];
    int max_x = c->width - getSpriteWidth(sprite);
    int max_y = c->height - getSpriteHeight(sprite);
    if (max_x < 0) max_x = 0;
    if (max_y < 0) max_y = 0;
    int draw_x = c->x > max_x ? max_x : c->x;
    int draw_y = c->y > max_y ? max_y : c->y;
    drawSpriteXPM(sprite, terminal, draw_x, draw_y, false);
}

void cursorUpdate(Cursor* c, Terminal* terminal, float deltaTime) {
    cursorUpdateCoordinates(c);
    cursorApplyVisualState(c);
    cursorDraw(c, terminal, deltaTime);
}

void cursorGetPixelPosition(Cursor* c, int* px, int* py) {
    *px = c->x;
    *py = c->y;
}

int cursorGetX(Cursor* c) {
    return c->x;
}

int cursorGetY(Cursor* c) {
    return c->y;
}

bool cursorIsLeftButtonPressed(Cursor* c) {
    if (c->leftButton == DOWN) {
        c->leftButton = PRESSED;
    }

    return c->leftButton == PRESSED;
}

bool cursorIsLeftButtonPressedDown(Cursor* c) {
    if (c->leftButton == DOWN) {
        c->leftButton = PRESSED;
        return true;
    }

    return false;
}

bool cursorIsMiddleButtonPressed(Cursor* c) {
    if (c->middleButton == DOWN) {
        c->middleButton = PRESSED;
    }

    return c->middleButton == PRESSED;
}

bool cursorIsMiddleButtonPressedDown(Cursor* c) {
    if (c->middleButton == DOWN) {
        c->middleButton = PRESSED;
        return true;
    }

    return false;
}

bool cursorIsRightButtonPressed(Cursor* c) {
    if (c->rightButton == DOWN) {
        c->rightButton = PRESSED;
    }

    return c->rightButton == PRESSED;
}

bool cursorIsRightButtonPressedDown(Cursor* c) {
    if (c->rightButton == DOWN) {
        c->rightButton = PRESSED;
        return true;
    }

    return false;
}
