#include "snake.h"
#include "../../assets/xpm/all.xpm"

static void destroy_snake(Snake* self) {
    if (!self) return;
    self->sprite->destroy(self->sprite);
    for (int i = 0; i < self->length; i++) {
        self->snakebody[i]->destroy(self->snakebody[i]);
    }
}

static void draw_snake(Snake* self) {
    if (!self) return;
    self->sprite->x = self->x;
    self->sprite->y = self->y;
    self->sprite->draw(self->sprite);
    for (int i = 0; i < self->length; i++) {
        self->snakebody[i]->draw(self->snakebody[i]);
    }
}

static int move_snake(Snake* self, Sprite* apple) {
    int new_x = self->x;
    int new_y = self->y;
    
    switch (self->direction) {
        case LEFT:  new_x -= self->xspeed; break;
        case UP:    new_y -= self->yspeed; break;
        case DOWN:  new_y += self->yspeed; break;
        case RIGHT: new_x += self->xspeed; break;
        default: break;
    }

    if (new_x < 0)
        new_x = SCREEN_WIDTH - CELL_SIZE;
    else if (new_x >= SCREEN_WIDTH)
        new_x = 0;
    if (new_y < 0)
        new_y = SCREEN_HEIGHT - CELL_SIZE;
    else if (new_y >= SCREEN_HEIGHT)
        new_y = 0;

    for (int i = 0; i < self->length; i++) {
        if (self->snakebody[i]->x == new_x && self->snakebody[i]->y == new_y) {
            return 0;
        }
    }

    int tail_x = self->snakebody[self->length - 1]->x;
    int tail_y = self->snakebody[self->length - 1]->y;

    for (int i = self->length - 1; i > 0; i--) {
        self->snakebody[i]->x = self->snakebody[i - 1]->x;
        self->snakebody[i]->y = self->snakebody[i - 1]->y;
    }

    self->x = new_x;
    self->y = new_y;
    self->snakebody[0] = self->sprite;

    if (new_x == apple->x && new_y == apple->y) {
        int old_apple_x = apple->x;
        int old_apple_y = apple->y;
        do {
            apple->x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            apple->y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
        } while (apple->x == old_apple_x && apple->y == old_apple_y);

        if (self->length < MAX_SNAKE_LEN) {
            self->snakebody[self->length] = create_sprite(white, tail_x, tail_y, self->xspeed, self->yspeed);
            self->length++;
        }
        return 1;
    }
    return 2;
}






Snake* create_snake(int x, int y, int xspeed, int yspeed) {
    Snake* snake = malloc(sizeof(Snake));
    snake->sprite = create_sprite(snake_xpm, x, y, xspeed, yspeed);

    snake->x = x;
    snake->y = y;
    snake->xspeed = xspeed;
    snake->yspeed = yspeed;
    snake->length = 1;
    snake->snakebody[0] = snake->sprite;
    snake->direction = RIGHT;

    snake->draw = draw_snake;
    snake->destroy = destroy_snake;
    snake->move = move_snake;

    return snake;
}
