#ifndef __GUIDRAWER_H
#define __GUIDRAWER_H

#include "../device_controllers/video/video.h"

#include <lcom/lcf.h>

typedef struct {
    uint16_t height, width;
    uint32_t *colors;
} Sprite;

Sprite* guiDrawer_get_button_sprite(int text_index);
Sprite* get_cursor_sprite(); 
Sprite* guiDrawer_get_start_button_sprite(); 
Sprite* guiDrawer_get_quit_button_sprite();
Sprite* guiDrawer_get_minigotchi_sprite();
Sprite* guiDrawer_get_minigotchi_cuddle_sprite();
Sprite* guiDrawer_get_minigotchi_normal_sprite();
Sprite* guiDrawer_get_hotbar_sprite();
Sprite* guiDrawer_get_hotbar_select_sprite();
Sprite* guiDrawer_get_day_window();
Sprite* guiDrawer_get_evening_window();
Sprite* guiDrawer_get_night_window();
Sprite* guiDrawer_get_guest_button_sprite();
Sprite* guiDrawer_get_host_button_sprite();
Sprite* guiDrawer_get_ttt_field();
Sprite* guiDrawer_get_ttt_wait();
Sprite* guiDrawer_get_ttt_o();
Sprite* guiDrawer_get_ttt_x();
Sprite* guiDrawer_get_bar_sprite(int bar_type, int level);
Sprite* guiDrawer_get_minigamesIcon_sprite();
Sprite* guiDrawer_get_apple_sprite();
Sprite* guiDrawer_get_item_sprite(int id);
Sprite* guiDrawer_get_rock_sprite();
Sprite* guiDrawer_get_paper_sprite();
Sprite* guiDrawer_get_scissors_sprite();
Sprite* guiDrawer_get_rpsWin_sprite();
Sprite* guiDrawer_get_rpsTie_sprite();
Sprite* guiDrawer_get_rpsLose_sprite();
Sprite* guiDrawer_get_botChose_sprite();
Sprite* guiDrawer_get_exitGameIcon_sprite();
Sprite* guiDrawer_get_ttt_button_sprite();
Sprite* guiDrawer_get_rps_button_sprite();
Sprite* guiDrawer_get_exitIconSmall_button_sprite();

Sprite* guiDrawer_get_happiness_level_sprite_from_index(uint8_t index);
void setup_happiness_sprites();
void cleanup_happiness_sprites();

// TicTacToe
void setup_ttt_sprites();
void cleanup_ttt_sprites();

// Happiness bar
Sprite* guiDrawer_get_hunger_level_sprite_from_index(uint8_t index);
void setup_hunger_sprites();
void cleanup_hunger_sprites();

// Hunger bar
Sprite* get_text_sprite_from_index(uint8_t index);
void setup_text_sprite();
void cleanup_text_sprite();

// Buffer getters
char** guiDrawer_get_backgroundBuffer();

// Buffer setters
void guiDrawer_set_backgroundBuffer(char** backgroundBuffer);

Sprite *create_sprite_xpm(xpm_map_t sprite);
void setup_sprites();
int drawSprite(Sprite *sprite, int x, int y);

// Sprite getters
uint16_t sprite_get_width(Sprite* sprite);
uint16_t sprite_get_height(Sprite* sprite);

void switchBackground(uint8_t bg);

// Wrappers
void wrapper_draw_background(uint8_t bufferIndex);

#endif /* __GUIDRAWER_H */
