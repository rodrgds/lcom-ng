#include "sprite.h"

Sprite * (create_sprite)(xpm_map_t xpm) {
    Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));
    if (sprite == NULL) {
        return NULL; // Memory allocation failed
    }

    xpm_image_t loaded_xpm;
    sprite->colors = (uint32_t*) xpm_load(xpm, XPM_8_8_8_8, &loaded_xpm);
    if (sprite->colors == NULL) {
        free(sprite);
        return NULL;
    }

    sprite->width = loaded_xpm.width;
    sprite->height = loaded_xpm.height;

    return sprite;
}

int (draw_sprite)(Sprite *sprite, uint16_t x, uint16_t y) {
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            uint32_t color = sprite->colors[i * sprite->width + j];
            if (vg_draw_pixel(x + j, y + i, color) != 0) {
                return 1; // Error drawing pixel
            }
        }
    }

    return 0; // Success
}

void (destroy_sprite)(Sprite *sprite) {
    if (sprite != NULL) {
        if (sprite->colors != NULL) {
            free(sprite->colors);
        }
        free(sprite);
    }
}

void (createMenuSprites)() {
    cursor = create_sprite(cursor_xpm);
    logo = create_sprite(logo_xpm);
    play = create_sprite(play_xpm);
    play_highlight = create_sprite(play_highlight_xpm);
    leaderboard = create_sprite(leaderboard_xpm);
    leaderboard_highlight = create_sprite(leaderboard_highlight_xpm);
    quit = create_sprite(exit_xpm);
    quit_highlight = create_sprite(exit_highlight_xpm);
    // Titulo, botões, etc.
}

void (destroyMenuSprites)() {
    destroy_sprite(cursor);
    // Titulo, botões, etc.
}

void (createGameOverSprites)() {
    play = create_sprite(play_xpm);
    play_highlight = create_sprite(play_highlight_xpm);
    quit = create_sprite(exit_xpm);
    quit_highlight = create_sprite(exit_highlight_xpm);
    score_text = create_sprite(score_xpm);
    // Titulo, botões, etc.
}

void (destroyGameOverSprites)() {
    destroy_sprite(play);
    destroy_sprite(play_highlight);
    destroy_sprite(quit);
    destroy_sprite(quit_highlight);
    destroy_sprite(score_text);
    // Titulo, botões, etc.
}

Sprite* get_digit_sprite(char digit) {
    switch (digit) {
        case 0: return number_0;
        case 1: return number_1;
        case 2: return number_2;
        case 3: return number_3;
        case 4: return number_4;
        case 5: return number_5;
        case 6: return number_6;
        case 7: return number_7;
        case 8: return number_8;
        case 9: return number_9;
        default: return NULL;
    }
}

void (createGameSprites)(){
    airship = create_sprite(airship_xpm);
    p_projectile = create_sprite(airship_projectile_xpm);

    alien1 = create_sprite(alien1_xpm);
    alien2 = create_sprite(alien2_xpm);
    alien3 = create_sprite(alien3_xpm);
    alien_explosion = create_sprite(alien_explosion_xpm);
    a_projectile = create_sprite(alien_projectile_xpm);

    barrier_st0 = create_sprite(stage0_xpm);
    barrier_st1 = create_sprite(stage1_xpm);
    barrier_st2 = create_sprite(stage2_xpm);
    barrier_st3 = create_sprite(stage3_xpm);

    heart_life = create_sprite(heart_life_xpm);
    heart_nolife = create_sprite(heart_nolife_xpm);

    number_0 = create_sprite(n0_xpm);
    number_1 = create_sprite(n1_xpm);
    number_2 = create_sprite(n2_xpm);
    number_3 = create_sprite(n3_xpm);
    number_4 = create_sprite(n4_xpm);
    number_5 = create_sprite(n5_xpm);
    number_6 = create_sprite(n6_xpm);
    number_7 = create_sprite(n7_xpm);
    number_8 = create_sprite(n8_xpm);
    number_9 = create_sprite(n9_xpm);

    score_text = create_sprite(score_xpm);
    lives_text = create_sprite(lives_xpm);
    wave_text = create_sprite(wave_xpm);
    // Outros sprites do jogo, inimigos, etc.
}

void (destroyGameSprites)() {
    destroy_sprite(airship);
    destroy_sprite(p_projectile);

    destroy_sprite(alien1);
    destroy_sprite(alien2);
    destroy_sprite(alien3);
    destroy_sprite(alien_explosion);
    destroy_sprite(a_projectile);

    destroy_sprite(barrier_st0);
    destroy_sprite(barrier_st1);
    destroy_sprite(barrier_st2);
    destroy_sprite(barrier_st3);

    destroy_sprite(heart_life);
    destroy_sprite(heart_nolife);

    destroy_sprite(number_0);
    destroy_sprite(number_1);
    destroy_sprite(number_2);
    destroy_sprite(number_3);
    destroy_sprite(number_4);
    destroy_sprite(number_5);
    destroy_sprite(number_6);
    destroy_sprite(number_7);
    destroy_sprite(number_8);
    destroy_sprite(number_9);

    destroy_sprite(score_text);
    destroy_sprite(lives_text);
    destroy_sprite(wave_text);
    // Outros sprites do jogo, inimigos, etc.
}
