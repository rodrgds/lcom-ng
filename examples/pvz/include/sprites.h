#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "pvz_platform.h"
#include "constants.h"

// Represents a set of pre-loaded animation frames
typedef struct {
    uint8_t **pixmaps;    // Array of pointers to pre-loaded raw pixel data
    uint16_t width;       // Width of the first frame; variable frames expose getters
    uint16_t height;      // Height of the first frame; variable frames expose getters
    uint16_t num_frames;  // Number of animation frames
} AnimSprite;

// Getters for each plant's idle animation sprite
AnimSprite* get_sunflower_idle_sprite(void);
AnimSprite* get_peashooter_idle_sprite(void);
AnimSprite* get_peashooter_shooting_sprite(void);
uint16_t get_peashooter_shooting_frame_width(int frame);
uint16_t get_peashooter_shooting_frame_height(int frame);
AnimSprite* get_repeater_idle_sprite(void);
AnimSprite* get_repeater_shooting_sprite(void);
uint16_t get_repeater_shooting_frame_width(int frame);
uint16_t get_repeater_shooting_frame_height(int frame);
AnimSprite* get_cabbage_idle_sprite(void);
AnimSprite* get_cabbage_shooting_sprite(void);
AnimSprite* get_cabbage_card_sprite(void);
uint16_t get_cabbage_idle_frame_width(int frame);
uint16_t get_cabbage_idle_frame_height(int frame);
uint16_t get_cabbage_shooting_frame_width(int frame);
uint16_t get_cabbage_shooting_frame_height(int frame);
AnimSprite* get_wall_nut_idle_sprite(void);
AnimSprite* get_potato_mine_idle_sprite(void);
AnimSprite* get_potato_mine_pop_sprite(void);
AnimSprite* get_potato_mine_explosion_sprite(void);
uint16_t get_potato_mine_idle_frame_width(int frame);
uint16_t get_potato_mine_idle_frame_height(int frame);
uint16_t get_potato_mine_pop_frame_width(int frame);
uint16_t get_potato_mine_pop_frame_height(int frame);
uint16_t get_potato_mine_explosion_frame_width(int frame);
uint16_t get_potato_mine_explosion_frame_height(int frame);
AnimSprite* get_spikeweed_idle_sprite(void);
AnimSprite* get_spikeweed_attack_sprite(void);
AnimSprite* get_spikeweed_card_sprite(void);
uint16_t get_spikeweed_idle_frame_width(int frame);
uint16_t get_spikeweed_idle_frame_height(int frame);
uint16_t get_spikeweed_attack_frame_width(int frame);
uint16_t get_spikeweed_attack_frame_height(int frame);

AnimSprite* get_regular_walking_sprite(void);
AnimSprite* get_regular_eating_sprite(void);
AnimSprite* get_conehead_walking_sprite(void);
AnimSprite* get_conehead_eating_sprite(void);
AnimSprite* get_pole_vaulting_walking_sprite(void);
AnimSprite* get_pole_vaulting_jump_sprite(void);
AnimSprite* get_pole_vaulting_nopole_sprite(void);
AnimSprite* get_pole_vaulting_eating_sprite(void);
uint16_t get_pole_vaulting_walking_frame_width(int frame);
uint16_t get_pole_vaulting_walking_frame_height(int frame);
uint16_t get_pole_vaulting_jump_frame_width(int frame);
uint16_t get_pole_vaulting_jump_frame_height(int frame);
uint16_t get_pole_vaulting_nopole_frame_width(int frame);
uint16_t get_pole_vaulting_nopole_frame_height(int frame);
uint16_t get_pole_vaulting_eating_frame_width(int frame);
uint16_t get_pole_vaulting_eating_frame_height(int frame);

AnimSprite* get_map_sprite(void);
AnimSprite* get_cursor_sprite(void);
AnimSprite* get_bullet_sprite(void);
uint16_t get_bullet_frame_width(int frame);
uint16_t get_bullet_frame_height(int frame);
AnimSprite* get_cabbage_projectile_sprite(void);
uint16_t get_cabbage_projectile_frame_width(int frame);
uint16_t get_cabbage_projectile_frame_height(int frame);
AnimSprite* get_sun_sprite(void);
AnimSprite* get_hud_sprite(void);
AnimSprite* get_shovel_sprite(void);
AnimSprite* get_only_shovel_sprite(void);
AnimSprite* get_lawnmower_sprite(void);

// Individual loaders/freers (called by load_all_sprites / free_all_sprites in renderer.c)
int load_sunflower_sprites(void);
void free_sunflower_sprites(void);

int load_peashooter_sprites(void);
void free_peashooter_sprites(void);

int load_repeater_sprites(void);
void free_repeater_sprites(void);

int load_cabbage_sprites(void);
void free_cabbage_sprites(void);

int load_wall_nut_sprites(void);
void free_wall_nut_sprites(void);

int load_potato_mine_sprites(void);
void free_potato_mine_sprites(void);

int load_spikeweed_sprites(void);
void free_spikeweed_sprites(void);

int load_regular_walking_sprites(void);
void free_regular_walking_sprites(void);
int load_regular_eating_sprites(void);
void free_regular_eating_sprites(void);

int load_cone_zombie_sprites(void);
void free_cone_zombie_sprites(void);
int load_pole_vaulting_zombie_sprites(void);
void free_pole_vaulting_zombie_sprites(void);

int load_map_sprite(void);
void free_map_sprite(void);

int load_cursor_sprite(void);
void free_cursor_sprite(void);

int load_bullet_sprite(void);
void free_bullet_sprite(void);

int load_cabbage_projectile_sprite();
void free_cabbage_projectile_sprite();

int load_sun_sprite(void);
void free_sun_sprite(void);

int load_hud_sprite(void);
void free_hud_sprite(void);

int load_shovel_sprite(void);
void free_shovel_sprite(void);

int load_lawnmower_sprite(void);
void free_lawnmower_sprite(void);

int load_seed_packet_sprite(void);
void free_seed_packet_sprite(void);
AnimSprite* get_seed_packet_sprite(void);

#endif /* _SPRITES_H_ */
