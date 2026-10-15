#include "apple.h"
#include "../../assets/xpm/all.xpm"

extern int option;

static void drawAppleIfExists(Apple* self) {
    if (self->exists) {
        self->sprite->x = self->x;
        self->sprite->y = self->y;
        self->sprite->draw(self->sprite);
    }
}

static void destroyApple(Apple* self) {
    if (!self) return;
    self->sprite->destroy(self->sprite);
}

static void applyPowerUp(Apple* self, FreeSnake* snake, int option) {
    switch (self->type) {
        case NORMAL:
            break;
        case GOLDEN:
            snake->speed *= 1.5;
            break;
        case WHITE:
            snake->r *= 2;
            break;
        case DARK:
            if (option == 1) {
                snake->speed = -snake->speed;
            } else if (option == 2) {
                snake->speed = -snake->speed;
            }
            break;
    }
}

static void disablePowerUp(Apple* self, FreeSnake* snake, int option) {
    switch (self->type) {
        case NORMAL:
            break;
        case GOLDEN:
            snake->speed /= 1.5;
            break;
        case WHITE:
            snake->r /= 2;
            break;
        case DARK:
            if (option == 1) {
                snake->speed = -snake->speed;
            } else if (option == 2) {
                snake->speed = -snake->speed;
            }
            break;
    }
}

static void checkPowerUpTimer(Apple* self, FreeSnake* snake, int option) {
    if (self->powerUpTimer < 180) self->powerUpTimer++;
    else if (self->powerUpTimer == 180) {
        self->disablePowerUp(self, snake, option);
        self->powerUpTimer = INT16_MAX;
    }
}


Apple* create_apple(AppleType type, bool exists, bool isMultiplayer) {
    Apple* apple = malloc(sizeof(Apple));

    int x, y;

    // tive de fazer isto para sincronizar o multiplayer, a ideia é ser temporario mas vamos ver
    if (isMultiplayer) {
        switch (type) {
            case NORMAL:
                x = 100;
                y = 100;
                break;
            case GOLDEN:
                x = 200;
                y = 200;
                break;
            case WHITE:
                x = 300;
                y = 300;
                break;
            case DARK:
                x = 400;
                y = 400;
                break;
        }
    } else {
        x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
        y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
    }

    apple->type = type;
    switch (apple->type) {
        case NORMAL:
            apple->sprite = create_sprite(apple_xpm, x, y, 0, 0);
            break;
        case GOLDEN:
            apple->sprite = create_sprite(goldenapple_xpm, x, y, 0, 0);
            break;
        case WHITE:
            apple->sprite = create_sprite(whiteapple_xpm, x, y, 0, 0);
            break;
        case DARK:
            apple->sprite = create_sprite(darkapple, x, y, 0, 0);
            break;
    }

    apple->exists = exists;
    apple->x = x;
    apple->y = y;
    apple->powerUpTimer = INT16_MAX;

    apple->destroy = destroyApple;
    apple->applyPowerUp = applyPowerUp;
    apple->disablePowerUp = disablePowerUp;
    apple->checkPowerUpTimer = checkPowerUpTimer;
    apple->drawIfExists = drawAppleIfExists;

    return apple;
}
