#include "hud.h"

void draw_number(int number, int x, int y) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", number);
    
    int length = strlen(buffer);
    for (int i = 0; i < length; i++) {
        char digit = buffer[i];
        if (digit >= '0' && digit <= '9') {
            draw_sprite(get_digit_sprite(digit - '0'), x + i * (number_0->width + 3), y);
        }
    }
}

void drawHud(Player *player) {
    if (player == NULL) {
        return;
    }

    draw_sprite(lives_text, 510, 590 - lives_text->height);

    for (int i = 0; i < 3; i++) {
        if (i < player->lives) {
            draw_sprite(heart_life, (520 + lives_text->width) + i * (heart_life->width + 5), 590 - heart_life->height);
        } else {
            draw_sprite(heart_nolife, (520 + lives_text->width) + i * (heart_nolife->width + 5), 590 - heart_nolife->height);
        }
    }

    draw_sprite(score_text, 510, 10);
    draw_number(player->score, 520 + score_text->width, 10);
}

void drawWave(int wave) {
    draw_sprite(wave_text, 510, 40);

    draw_number(wave, 520 + wave_text->width, 40);
}
