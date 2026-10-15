#include "player.h"

Player *createPlayer(int x, int y, int lives, int score, Sprite *sprite) {
    Player *player = (Player *)malloc(sizeof(Player));
    if (player == NULL) {
        return NULL; // Memory allocation failed
    }
    player->x = x;
    player->y = y;
    player->lives = lives;
    player->score = score;
    player->sprite = sprite;
    return player;
}

void destroyPlayer(Player *player) {
    if (player != NULL) {
        if (player->sprite != NULL) {
            destroy_sprite(player->sprite);
        }
        free(player);
    }
}

void drawPlayer(Player *player) {
    if (player != NULL && player->sprite != NULL) {
        draw_sprite(player->sprite, player->x, player->y);
    }
}

void playerMove(Player *player, int delta_x) {
    if (player == NULL) {
        return;
    }
    player->x += delta_x;

    // Ensure the player stays within the bounds of the screen
    if (player->x < 0) player->x = 0;
    if (player->x > 500 - player->sprite->width) player->x = 500 - player->sprite->width;
}

int damagePlayer(Player *player) {
    if (player != NULL) {
        player->lives--;
        if (player->lives <= 0) {
            return 1;
        }
    }
    return 0;
}
