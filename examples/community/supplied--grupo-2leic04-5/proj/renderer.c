// INCLUDES
#include "../include/renderer.h"
#include <stdbool.h>
#include <stdint.h>
#include "../include/game_state.h"
#include "../include/player.h"
#include "../include/config.h"
#include "../include/bullet.h"
#include "../include/enemy.h"
#include "../lab1/rtc.h"
#include "../lab5/video.h"
#include "../include/player_sprites.h"
#include "../include/enemy_sprites.h"

// MEDIA
#include "../media/sprites/player/bullet.xpm"
#include "../media/sprites/analog/enemy_bullet.xpm"

// Backgrounds
#include "../media/menu_bg.xpm"
#include "../media/game_over.xpm"
#include "../media/victory.xpm"

#include "../media/levels/level1_bg.xpm"
#include "../media/levels/level2_bg.xpm"
#include "../media/levels/level3_bg.xpm"
#include "../media/levels/level4_bg.xpm"

#include "../media/levels/speech/level1_speech.xpm"
#include "../media/levels/speech/level2_speech.xpm"
#include "../media/levels/speech/level3_speech.xpm"
#include "../media/levels/speech/level4_speech.xpm"

// Botões
#include "../media/buttons/button_play.xpm"
#include "../media/buttons/button_continue.xpm"
#include "../media/buttons/button_quit.xpm"
#include "../media/buttons/button_restart.xpm"

#define VIDEO_MODE 0x115 // 800×600, 24 bpp

#define BG_COLOR 0x000000
#define ARROW_COLOR 0xFF00FF //roxa
#define SELECT_COLOR 0xFFFFFF
#define FALLBACK_BUTTON_COLOR 0x202020

static rtc_date victory_date = {
    .day = 5,
    .month = 6,
    .year = 26
};

static bool victory_date_valid = true;

typedef struct {
    uint8_t *map;
    xpm_image_t img;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} ButtonSprite;

static uint8_t *player_frames[PLAYER_NUM_FRAMES];
static xpm_image_t player_img_info;

static uint8_t *player_frozen_sprite = NULL;
static xpm_image_t player_frozen_img_info;
 
static uint8_t *bullet_sprite = NULL;
static xpm_image_t bullet_img_info;

static uint8_t *enemy_bullet_sprite = NULL;
static xpm_image_t enemy_bullet_img_info;

static xpm_image_t menu_bg_img;
static uint8_t *menu_bg_colors = NULL;

static xpm_image_t game_over_img;
static uint8_t *game_over_colors = NULL;
 
static xpm_image_t victory_img;
static uint8_t *victory_colors = NULL;

static xpm_image_t level1_bg_img;
static uint8_t *level1_bg_colors = NULL;
 
static xpm_image_t level2_bg_img;
static uint8_t *level2_bg_colors = NULL;
 
static xpm_image_t level3_bg_img;
static uint8_t *level3_bg_colors = NULL;

static xpm_image_t level4_bg_img;
static uint8_t *level4_bg_colors = NULL;

static xpm_image_t speech1_img;
static uint8_t *speech1_colors = NULL;
 
static xpm_image_t speech2_img;
static uint8_t *speech2_colors = NULL;
 
static xpm_image_t speech3_img;
static uint8_t *speech3_colors = NULL;
 
static xpm_image_t speech4_img;
static uint8_t *speech4_colors = NULL;

static ButtonSprite play_button;
static ButtonSprite continue_button;
static ButtonSprite quit_button;
static ButtonSprite restart_button;

static void clear_screen(void);
static void renderer_draw_victory_date_overlay(void);
static void renderer_draw_7seg_digit(int x, int y, uint8_t digit);
static void renderer_draw_7seg_hseg(int x, int y, int w);
static void renderer_draw_7seg_vseg(int x, int y, int h);
static void renderer_draw_7seg_hyphen(int x, int y);

void renderer_draw_sprite_red_tint(xpm_image_t img, uint8_t *map, int x, int y) {
    if (map == NULL) return;

    int bytes_per_pixel = img.size / (img.width * img.height);
    uint32_t transparent = xpm_transparency_color(img.type);

    for (int row = 0; row < img.height; row++) {
        for (int col = 0; col < img.width; col++) {

            if (x + col < 0 || x + col >= SCREEN_W || y + row < 0 || y + row >= SCREEN_H)
                continue;

            int idx = (row * img.width + col) * bytes_per_pixel;

            uint32_t color = 0;
            for (int b = 0; b < bytes_per_pixel; b++) {
                color |= (uint32_t)(map[idx + b]) << (8 * b);
            }

            if (color == transparent) continue; // ignora pixels transparentes (None no XPM)

            uint8_t r  = (color >> 16) & 0xFF;
            uint8_t g  = (color >>  8) & 0xFF;
            uint8_t bv =  color & 0xFF;

            r  = (uint8_t)(r  * 0.7 + 255 * 0.3);
            g  = (uint8_t)(g  * 0.7);
            bv = (uint8_t)(bv * 0.7);

            my_vg_draw_pixel((uint16_t)(x + col), (uint16_t)(y + row),
                             ((uint32_t)r << 16) | ((uint32_t)g << 8) | bv);
        }
    }
}

static void renderer_update_button_bounds(ButtonSprite *button) {
    int bytes_per_pixel = button->img.size / (button->img.width * button->img.height);
    uint32_t transparent = xpm_transparency_color(button->img.type);

    button->min_x = button->img.width;
    button->min_y = button->img.height;
    button->max_x = -1;
    button->max_y = -1;

    for (int y = 0; y < button->img.height; y++) {
        for (int x = 0; x < button->img.width; x++) {
            int index = (y * button->img.width + x) * bytes_per_pixel;
            uint32_t color = 0;

            for (int b = 0; b < bytes_per_pixel; b++) {
                color |= (button->map[index + b]) << (8 * b);
            }

            if (color == transparent) continue;

            if (x < button->min_x) button->min_x = x;
            if (y < button->min_y) button->min_y = y;
            if (x > button->max_x) button->max_x = x;
            if (y > button->max_y) button->max_y = y;
        }
    }

    if (button->max_x < button->min_x || button->max_y < button->min_y) {
        button->min_x = 0;
        button->min_y = 0;
        button->max_x = button->img.width - 1;
        button->max_y = button->img.height - 1;
    }
}

static int renderer_load_button(ButtonSprite *button, xpm_map_t xpm) {
    button->map = xpm_load(xpm, XPM_8_8_8, &button->img);
    if (button->map == NULL) {
        button->img.width = 260;
        button->img.height = 70;
        button->min_x = 0;
        button->min_y = 0;
        button->max_x = button->img.width - 1;
        button->max_y = button->img.height - 1;
        return 1;
    }

    renderer_update_button_bounds(button);
    return 0;
}

// carrega os mapas dos sprites do jogador, retorna 0 se tiver sucesso ou 1 se falhar
static int renderer_load_player_frames(void) {
    // segurança para garantir que o número de sprites corresponde ao n de frames definidos
    if (PLAYER_SPRITE_COUNT != PLAYER_NUM_FRAMES) return 1;

    for (int i = 0; i < PLAYER_NUM_FRAMES; i++) {
        player_frames[i] = xpm_load(player_sprite_maps[i], XPM_8_8_8, &player_img_info);
        if (player_frames[i] == NULL) return 1;
    }

    player_frozen_sprite = xpm_load((xpm_map_t)player_frozen_xpm, XPM_8_8_8, &player_frozen_img_info);
    if (player_frozen_sprite == NULL) return 1;

    return 0;
}

int renderer_init(void) {
    if (my_map_vram(VIDEO_MODE) != 0) return 1;
    if (my_vg_init(VIDEO_MODE)  != 0) return 1;
    if (my_vg_enable_double_buffer() != 0) return 1;

    renderer_load_button(&play_button, (xpm_map_t)button_play_xpm);
    renderer_load_button(&continue_button, (xpm_map_t)button_continue_xpm);
    renderer_load_button(&quit_button, (xpm_map_t)button_quit_xpm);
    renderer_load_button(&restart_button, (xpm_map_t)button_restart_xpm);

    // tenta carregar os backgrounds, se falhar, o jogo ainda funciona, mas com fundo preto
    menu_bg_colors = xpm_load((xpm_map_t)menu_bg, XPM_8_8_8, &menu_bg_img);
    game_over_colors = xpm_load((xpm_map_t)game_over, XPM_8_8_8, &game_over_img);
    victory_colors = xpm_load((xpm_map_t)victory, XPM_8_8_8, &victory_img);
    level1_bg_colors = xpm_load((xpm_map_t)level1_bg, XPM_8_8_8, &level1_bg_img);
    level2_bg_colors = xpm_load((xpm_map_t)level2_bg, XPM_8_8_8, &level2_bg_img);
    level3_bg_colors = xpm_load((xpm_map_t)level3_bg, XPM_8_8_8, &level3_bg_img);
    level4_bg_colors = xpm_load((xpm_map_t)level4_bg, XPM_8_8_8, &level4_bg_img);
    speech1_colors = xpm_load((xpm_map_t)level1_speech, XPM_8_8_8, &speech1_img);
    speech2_colors = xpm_load((xpm_map_t)level2_speech, XPM_8_8_8, &speech2_img);
    speech3_colors = xpm_load((xpm_map_t)level3_speech, XPM_8_8_8, &speech3_img);
    speech4_colors = xpm_load((xpm_map_t)level4_speech, XPM_8_8_8, &speech4_img);

    // carrega os sprites do jogador, mas nao impede o menu de abrir se algum XPM novo falhar
    renderer_load_player_frames();

    // carrega os sprites dos inimigos, mas nao impede o menu de abrir se algum XPM novo falhar
    enemy_load_sprites();

    bullet_sprite = xpm_load((xpm_map_t)bullet_xpm, XPM_8_8_8, &bullet_img_info);

    enemy_bullet_sprite = xpm_load((xpm_map_t)laser1780427427, XPM_8_8_8, &enemy_bullet_img_info);
 
    player_init(player_frames, player_img_info);
    bullet_init();

    return 0;
}

static void renderer_draw_direction_arrow(Player *p) {
    int arrow_x = p->x + p->img_info.width / 2 - 8;
    int arrow_y;
    uint32_t arrow_color = ARROW_COLOR;

    if (p->facing_v == FACING_UP) {
        arrow_y = p->y - 16;

        my_vg_draw_hline(arrow_x + 8, arrow_y,     1, arrow_color);
        my_vg_draw_hline(arrow_x + 7, arrow_y + 1,  3, arrow_color);
        my_vg_draw_hline(arrow_x + 6, arrow_y + 2,  5, arrow_color);
        my_vg_draw_hline(arrow_x + 5, arrow_y + 3,  7, arrow_color);
        my_vg_draw_hline(arrow_x + 4, arrow_y + 4,  9, arrow_color);
        my_vg_draw_hline(arrow_x + 3, arrow_y + 5, 11, arrow_color);
        my_vg_draw_hline(arrow_x + 2, arrow_y + 6, 13, arrow_color);
        my_vg_draw_hline(arrow_x + 1, arrow_y + 7, 15, arrow_color);
    } 
    
    else if (p->facing_v == FACING_DOWN) {
        arrow_y = p->y + p->img_info.height + 4;

        my_vg_draw_hline(arrow_x + 1, arrow_y,     15, arrow_color);
        my_vg_draw_hline(arrow_x + 2, arrow_y + 1, 13, arrow_color);
        my_vg_draw_hline(arrow_x + 3, arrow_y + 2, 11, arrow_color);
        my_vg_draw_hline(arrow_x + 4, arrow_y + 3,  9, arrow_color);
        my_vg_draw_hline(arrow_x + 5, arrow_y + 4,  7, arrow_color);
        my_vg_draw_hline(arrow_x + 6, arrow_y + 5,  5, arrow_color);
        my_vg_draw_hline(arrow_x + 7, arrow_y + 6,  3, arrow_color);
        my_vg_draw_hline(arrow_x + 8, arrow_y + 7,  1, arrow_color);
    }
}

void renderer_cleanup(void) {
    bullet_cleanup(); // limpa os recursos dos tiros
    clear_screen(); // limpa a tela antes de sair
    my_vg_flip_buffer(); // garante que a tela limpa é mostrada antes de sair
    my_vg_free_double_buffer(); 
    vg_exit();
}
 
static void clear_screen(void) {
    my_vg_draw_rectangle(0, 0, SCREEN_W, SCREEN_H, BG_COLOR);
}

static const uint8_t SEG_MASKS[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static void renderer_draw_7seg_hseg(int x, int y, int w) {
    if (w > 0) {
        my_vg_draw_rectangle((uint16_t)x, (uint16_t)y, (uint16_t)w, 4, SELECT_COLOR);
    }
}

static void renderer_draw_7seg_vseg(int x, int y, int h) {
    if (h > 0) {
        my_vg_draw_rectangle((uint16_t)x, (uint16_t)y, 4, (uint16_t)h, SELECT_COLOR);
    }
}

static void renderer_draw_7seg_digit(int x, int y, uint8_t digit) {
    if (digit > 9) return;

    const int w = 18;
    const int h = 30;
    const int t = 4;
    const int inner_w = w - 2 * t;
    const int upper_h = (h - 3 * t) / 2;
    const int lower_h = upper_h;
    const int mid_y = y + (h / 2) - (t / 2);

    uint8_t mask = SEG_MASKS[digit];

    if (mask & 0x01) renderer_draw_7seg_hseg(x + t, y, inner_w);                    // a
    if (mask & 0x02) renderer_draw_7seg_vseg(x + w - t, y + t, upper_h);             // b
    if (mask & 0x04) renderer_draw_7seg_vseg(x + w - t, y + t + upper_h + t, lower_h); // c
    if (mask & 0x08) renderer_draw_7seg_hseg(x + t, y + h - t, inner_w);             // d
    if (mask & 0x10) renderer_draw_7seg_vseg(x, y + t + upper_h + t, lower_h);       // e
    if (mask & 0x20) renderer_draw_7seg_vseg(x, y + t, upper_h);                     // f
    if (mask & 0x40) renderer_draw_7seg_hseg(x + t, mid_y, inner_w);                 // g
}

static void renderer_draw_7seg_hyphen(int x, int y) {
    my_vg_draw_rectangle((uint16_t)x, (uint16_t)(y + 13), 12, 4, SELECT_COLOR);
}

static void renderer_draw_victory_date_overlay(void) {
    if (!victory_date_valid) return;

    const int digit_w = 18;
    const int gap = 6;
    const int symbol_gap = 10;
    const int hyphen_w = 12;
    const int total_w = (8 * digit_w) + (2 * hyphen_w) + (7 * gap) + (2 * symbol_gap);
    const int total_h = 34;
    const int x0 = (SCREEN_W - total_w) / 2;
    const int y0 = SCREEN_H - total_h - 22;

    my_vg_draw_rectangle((uint16_t)(x0 - 10), (uint16_t)(y0 - 10),
                         (uint16_t)(total_w + 20), (uint16_t)(total_h + 20),
                         0x101010);

    int x = x0;

    renderer_draw_7seg_digit(x, y0, victory_date.day / 10); x += digit_w + gap;
    renderer_draw_7seg_digit(x, y0, victory_date.day % 10); x += digit_w + symbol_gap;

    renderer_draw_7seg_hyphen(x, y0); x += hyphen_w + symbol_gap;

    renderer_draw_7seg_digit(x, y0, victory_date.month / 10); x += digit_w + gap;
    renderer_draw_7seg_digit(x, y0, victory_date.month % 10); x += digit_w + symbol_gap;

    renderer_draw_7seg_hyphen(x, y0); x += hyphen_w + symbol_gap;

    renderer_draw_7seg_digit(x, y0, 2); x += digit_w + gap;
    renderer_draw_7seg_digit(x, y0, 0); x += digit_w + gap;
    renderer_draw_7seg_digit(x, y0, victory_date.year / 10); x += digit_w + gap;
    renderer_draw_7seg_digit(x, y0, victory_date.year % 10);
}


// desenha um retângulo de seleção ao redor de um botão, com uma borda de 8 pixels, talvez mudar a margem mais tarde nao sei
static void renderer_draw_selection_rect(int x, int y, int width, int height) {
    my_vg_draw_rectangle((uint16_t)x, (uint16_t)y, (uint16_t)width, 3, SELECT_COLOR);
    my_vg_draw_rectangle((uint16_t)x, (uint16_t)(y + height - 3), (uint16_t)width, 3, SELECT_COLOR);
    my_vg_draw_rectangle((uint16_t)x, (uint16_t)y, 3, (uint16_t)height, SELECT_COLOR);
    my_vg_draw_rectangle((uint16_t)(x + width - 3), (uint16_t)y, 3, (uint16_t)height, SELECT_COLOR);
}

// desenha um botao na tela, com ou sem seleção
static void renderer_draw_button(ButtonSprite *button, int center_y, bool selected) {

    // calcula as dimensoes e posiçao do retangulo opaco a ser desenhado
    int opaque_width = button->max_x - button->min_x + 1;
    int opaque_height = button->max_y - button->min_y + 1;

    // calcula a posiçao para desenhar o sprite de forma a centralizar o retangulo opaco na tela
    int opaque_x = (SCREEN_W - opaque_width) / 2;
    int opaque_y = center_y - opaque_height / 2;

    // calcula a posiçao para desenhar o sprite de forma a alinhar o retangulo opaco com a posiçao calculada anteriormente
    int draw_x = opaque_x - button->min_x;
    int draw_y = opaque_y - button->min_y;

    if (button->map != NULL) {
        renderer_draw_sprite(button->img, button->map, draw_x, draw_y);
    }
    else {
        my_vg_draw_rectangle((uint16_t)opaque_x, (uint16_t)opaque_y,
                             (uint16_t)opaque_width, (uint16_t)opaque_height,
                             FALLBACK_BUTTON_COLOR);
    }

    if (selected) {
        renderer_draw_selection_rect(opaque_x - 8, opaque_y - 8, opaque_width + 16, opaque_height + 16);
    }
}

// desenha o background, se tiver sido carregado com sucesso, senao clear_screen
static void renderer_draw_background(xpm_image_t img, uint8_t *colors) {
    if (colors != NULL) {
        renderer_draw_sprite(img, colors, 0, 0);
    }
    else {
        clear_screen();
    }
}

// desenha os tiros ativos na tela
static void renderer_draw_bullets(void) {
    Bullet *bullets = bullet_get_all();
    int bullet_count = bullet_get_count();

    for (int i = 0; i < bullet_count; i++) {
        if (bullets[i].is_active) {
            if (bullets[i].type == BULLET_PLAYER) {
                renderer_draw_sprite(bullet_img_info, bullet_sprite, bullets[i].x, bullets[i].y);
            } else if (bullets[i].type == BULLET_ENEMY) {
                renderer_draw_sprite(enemy_bullet_img_info, enemy_bullet_sprite, bullets[i].x, bullets[i].y);
            }
        }
    }
}

// desenha a cena do jogo, o background, os tiros e o jogador com a seta de direçao
static void renderer_draw_game_scene(void) {

    switch (game_state_current_level()) {
      case 1:
          renderer_draw_background(level1_bg_img, level1_bg_colors);
          break;
      case 2:
          renderer_draw_background(level2_bg_img, level2_bg_colors);
          break;
      case 3:
          renderer_draw_background(level3_bg_img, level3_bg_colors);
          break;
      case 4:
          renderer_draw_background(level4_bg_img, level4_bg_colors);
          break;
      default:
          clear_screen();
          break;
    }

    Player *p = player_get();
    renderer_draw_bullets();
    draw_enemies(enemy_get_all(), enemy_get_count());

    if (p->freeze_ticks > 0 && player_frozen_sprite != NULL) {
        renderer_draw_sprite(player_frozen_img_info, player_frozen_sprite, p->x, p->y);
    } else if (p->damage_ticks > 0) {
        renderer_draw_sprite_red_tint(player_img_info, player_frames[p->current_frame], p->x, p->y);
    } else {
        renderer_draw_sprite(p->img_info, p->frame_maps[p->current_frame], p->x, p->y);
    }
        renderer_draw_direction_arrow(p);
}

// desenha um overlay de pausa, que é uma grade de linhas horizontais para parecer q baixamos a opacidade da tela, talvez mudar isso mais tarde para um retangulo semitransparente ou algo assim
static void renderer_draw_pause_overlay(void) {
    for (int y = 0; y < SCREEN_H; y += 2) {
        my_vg_draw_hline(0, (uint16_t)y, SCREEN_W, BG_COLOR);
    }

    for (int y = 1; y < SCREEN_H; y += 4) {
        my_vg_draw_hline(0, (uint16_t)y, SCREEN_W, BG_COLOR);
    }
}
 
void renderer_draw_menu(void) {
    renderer_draw_background(menu_bg_img, menu_bg_colors);

    renderer_draw_button(&play_button, 250, game_state_selected_action() == MENU_ACTION_PRIMARY);
    renderer_draw_button(&quit_button, 360, game_state_selected_action() == MENU_ACTION_QUIT);
    my_vg_flip_buffer();
}
 
void renderer_draw_sprite(xpm_image_t img, uint8_t *map, int x, int y) {
 
    if (map == NULL) return; // segurança para evitar crashes se o mapa não carregar
 
    int bytes_per_pixel = img.size / (img.width * img.height);
    uint32_t transparent = xpm_transparency_color(img.type); 
 
    for (int h = 0; h < img.height; h++) {
        for (int w = 0; w < img.width; w++) {
            
            if (x + w < 0 || x + w >= SCREEN_W || y + h < 0 || y + h >= SCREEN_H) {
                continue; // ignora pixels fora da tela
            }
 
            int index = (h * img.width + w) * bytes_per_pixel;
            
            uint32_t color = 0;
            for (int b = 0; b < bytes_per_pixel; b++) {
                color |= (map[index + b]) << (8 * b);
            }
            if (color != transparent) {
                my_vg_draw_pixel((uint16_t)(x + w), (uint16_t)(y + h), color);
            }
        }
    }
}
 
void renderer_draw_game(void) {
    renderer_draw_game_scene();
    my_vg_flip_buffer();
}

// desenha a cena do jogo com o overlay de pausa e as opcoes de pausa, destaca a opcao selecionada
void renderer_draw_paused(void) {
    renderer_draw_game_scene();
    renderer_draw_pause_overlay();

    renderer_draw_button(&continue_button, 180, game_state_selected_action() == MENU_ACTION_PRIMARY);
    renderer_draw_button(&restart_button, 300, game_state_selected_action() == MENU_ACTION_RESTART);
    renderer_draw_button(&quit_button, 420, game_state_selected_action() == MENU_ACTION_QUIT);
    my_vg_flip_buffer();
}

// desenha a tela de vitoria, com opções para reiniciar ou sair
void renderer_draw_victory(void) {
  renderer_draw_background(victory_img, victory_colors);
  renderer_draw_victory_date_overlay();
  my_vg_flip_buffer();
}

// desenha a tela de game over, com opções para reiniciar ou sair
void renderer_draw_game_over(void) {
    renderer_draw_background(game_over_img, game_over_colors);
    my_vg_flip_buffer();
}

// desenha o ecrã de speech do nível, pressionar ENTER avança para o nível
static void renderer_draw_speech(int level) {
    switch (level) {
        case 1: renderer_draw_background(speech1_img, speech1_colors); break;
        case 2: renderer_draw_background(speech2_img, speech2_colors); break;
        case 3: renderer_draw_background(speech3_img, speech3_colors); break;
        case 4: renderer_draw_background(speech4_img, speech4_colors); break;
        default: clear_screen(); break;
    }
    my_vg_flip_buffer();
}

void renderer_draw(void) {
  switch (game_state_get()) {
      case STATE_MENU:
          renderer_draw_menu();
          break;
      case STATE_SPEECH_1:
          renderer_draw_speech(1);
          break;
      case STATE_SPEECH_2:
          renderer_draw_speech(2);
          break;
      case STATE_SPEECH_3:
          renderer_draw_speech(3);
          break;
      case STATE_SPEECH_4:
          renderer_draw_speech(4);
          break;
      case STATE_LEVEL_1:
      case STATE_LEVEL_2:
      case STATE_LEVEL_3:
      case STATE_LEVEL_4:
          renderer_draw_game();
          break;
      case STATE_VICTORY:
          renderer_draw_victory();
          break;
      case STATE_GAME_OVER:
          renderer_draw_game_over();
          break;
      case STATE_PAUSED:
          renderer_draw_paused();
          break;
      default:
          clear_screen();
          my_vg_flip_buffer();
          break;
  }
}
 
int renderer_get_bullet_width(void) {
    return bullet_img_info.width;
}
 
int renderer_get_bullet_height(void) {
    return bullet_img_info.height;
}  

int renderer_get_enemy_bullet_width(void) {
    return enemy_bullet_img_info.width;
}

int renderer_get_enemy_bullet_height(void) {
    return enemy_bullet_img_info.height;
}

void renderer_set_victory_date(const rtc_date *date) {

    if (date == NULL)
        return;

    victory_date = *date;
    victory_date_valid = true;
}
