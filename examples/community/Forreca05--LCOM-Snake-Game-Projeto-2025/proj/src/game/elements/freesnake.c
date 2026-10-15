#include "freesnake.h"
#include "../../assets/xpm/all.xpm"

#define TURN_RATE (M_PI / 30)
#define COLLISION_RADIUS 50

extern int mouse_x;
extern int mouse_y;

static void destroyFreeSnake(FreeSnake* self) {
    if (!self) return;
    self->sprite->destroy(self->sprite);
    for (int i = 0; i < self->bodyLength; i++) {
        self->body[i]->destroy(self->body[i]);
    }
}

static bool checkAppleCollision(FreeSnake* self, Apple* apple) {
    int dx = apple->x - self->x;
    int dy = apple->y - self->y;
    if (sqrt(dx*dx + dy*dy) < COLLISION_RADIUS) {
        apple->x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
        apple->y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;

        self->body[self->bodyLength] = create_sprite(white, self->x, self->y, 0, 0);
        self->bodyLength++;
        if (apple->type != NORMAL) {
            apple->exists = false; 
        }
        return true;
    }
    return false;
}

static bool moveFreeSnake(FreeSnake* self, int option) {
    if (option == 1 && abs(mouse_x - self->x) < self->speed && abs(mouse_y - self->y) < self->speed) {
        return false;
    }
    switch (option) {
        case 1:
            self->angle = atan2(mouse_y - self->y, mouse_x - self->x);
            break;
        case 2:
            if (self->direction == LEFT) self->angle -= TURN_RATE;
            if (self->direction == RIGHT) self->angle += TURN_RATE;
            break;
    }


    self->vx = self->speed * cos(self->angle);
    self->vy = self->speed * sin(self->angle);

    for (int i = self->bodyLength - 1; i > 0; i--) {
        self->body[i]->x = self->body[i - 1]->x;
        self->body[i]->y = self->body[i - 1]->y;
    }

    int new_x = self->x + self->vx;
    int new_y = self->y + self->vy;

    if (new_x <= 0 || new_x >= SCREEN_WIDTH || new_y <= 0 || new_y >= SCREEN_HEIGHT) return false;

    self->x = new_x;
    self->y = new_y;

    return true;
}

static void drawFreeSnake(FreeSnake* self) {
    self->sprite->x = self->x;
    self->sprite->y = self->y;

    double r = self->r;
    const int borderThickness = 3;
    const uint32_t circleColor = 0x00BB00;
    const uint32_t borderColor  = 0x000000;

    for (int i = 0; i < self->bodyLength; i++) {
        int cx = self->body[i]->x + r;
        int cy = self->body[i]->y + r;

        for (int x = cx - r; x <= cx + r; x++) {
            for (int y = cy - r; y <= cy + r; y++) {
                int dx = x - cx, dy = y - cy;
                if (dx*dx + dy*dy <= r*r) {
                    vg_draw_pixel(x, y, circleColor + i * 40);
                }
            }
        }
    }

    for (int i = 0; i < self->bodyLength; i++) {
        int cx = self->body[i]->x + r;
        int cy = self->body[i]->y + r;

        for (int x = cx - r; x <= cx + r; x++) {
            for (int y = cy - r; y <= cy + r; y++) {
                int dx = x - cx, dy = y - cy;
                int dist2 = dx*dx + dy*dy;
                if (dist2 >= (r - borderThickness)*(r - borderThickness) && dist2 <= r*r) {
                    bool covered = false;
                    for (int j = 0; j < self->bodyLength; j++) {
                        if (j == i) continue;
                        int jcx = self->body[j]->x + r;
                        int jcy = self->body[j]->y + r;
                        int ddx = x - jcx, ddy = y - jcy;
                        // this looks a bit confusing but here we are checking if each pixel (x, y) is inside any other circle
                        // if it is, we mark it as covered in order to not draw the border there, to make for a continuous snake
                        if (ddx*ddx + ddy*ddy <= r*r) {
                            covered = true;
                            break;
                        }
                    }
                    if (!covered) {
                        vg_draw_pixel(x, y, borderColor);
                    }
                }
            }
        }
    }

    // self->sprite->x = self->x;
    // self->sprite->y = self->y;
    // self->sprite->draw(self->sprite);
}

bool checkSnakeCollision(FreeSnake* self, FreeSnake* other) {
    for (int i = 0; i < self->bodyLength; i++) {
        int dx = self->body[i]->x - other->x;
        int dy = self->body[i]->y - other->y;
        if (sqrt(dx*dx + dy*dy) < COLLISION_RADIUS) {
            return true;
        }
    }

    for (int i = 0; i < other->bodyLength; i++) {
        int dx = other->body[i]->x - self->x;
        int dy = other->body[i]->y - self->y;
        if (sqrt(dx*dx + dy*dy) < COLLISION_RADIUS) {
            return true;
        }
    }

    return false;
}

FreeSnake* createFreeSnake(int x, int y) {
    FreeSnake* freeSnake = malloc(sizeof(FreeSnake));
    freeSnake->sprite = create_sprite(white, x, y, 0, 0);

    freeSnake->body[0] = freeSnake->sprite;
    freeSnake->bodyLength = 1;
    freeSnake->angle = 0;
    freeSnake->speed = 5;
    freeSnake->vx = freeSnake->speed;
    freeSnake->vy = 0;
    freeSnake->x = x;
    freeSnake->y = y;
    freeSnake->r = 20;

    freeSnake->draw = drawFreeSnake;
    freeSnake->checkAppleCollision = checkAppleCollision;
    freeSnake->checkSnakeCollision = checkSnakeCollision;
    freeSnake->move = moveFreeSnake;
    freeSnake->destroy = destroyFreeSnake;

    for (int i = 0; i < 5; i++) {
        freeSnake->body[freeSnake->bodyLength] = create_sprite(white, freeSnake->x, freeSnake->y, 0, 0);
        freeSnake->bodyLength++;
    }

    return freeSnake;
}

