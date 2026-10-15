#include <lcom/lcf.h>

#include "model/resources/resources.h"

static Resources resources;
static AnimSprite *player_anims[4];

/* enemy */
static const char **(enemy_a_xpms)[ENEMY_ANIM_FRAMES] = {enemy_a_1_xpm, enemy_a_2_xpm, enemy_a_3_xpm, enemy_a_4_xpm};

/* player */
static const char **player_down_xpms[PLAYER_ANIM_FRAMES] = {player_down_1_xpm, player_down_2_xpm, player_down_3_xpm, player_down_4_xpm};
static const char **player_up_xpms[PLAYER_ANIM_FRAMES] = {player_up_1_xpm, player_up_2_xpm, player_up_3_xpm, player_up_4_xpm};
static const char **player_left_xpms[PLAYER_ANIM_FRAMES] = {player_left_1_xpm, player_left_2_xpm, player_left_3_xpm, player_left_4_xpm};
static const char **player_right_xpms[PLAYER_ANIM_FRAMES] = {player_right_1_xpm, player_right_2_xpm, player_right_3_xpm, player_right_4_xpm};

/* bomb */
static const char **bomb_xpms[BOMB_ANIM_FRAMES] = {bomb_1_xpm, bomb_2_xpm, bomb_3_xpm, bomb_4_xpm};

/* explosion */
static const char **explosion_center_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_center_1_xpm, explosion_center_2_xpm, explosion_center_3_xpm, explosion_center_4_xpm};

static const char **explosion_horiz_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_arm_h_1_xpm, explosion_arm_h_2_xpm, explosion_arm_h_3_xpm, explosion_arm_h_4_xpm};
static const char **explosion_vert_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_arm_v_1_xpm, explosion_arm_v_2_xpm, explosion_arm_v_3_xpm, explosion_arm_v_4_xpm};

static const char **explosion_hand_up_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_hand_up_1_xpm, explosion_hand_up_2_xpm, explosion_hand_up_3_xpm, explosion_hand_up_4_xpm};
static const char **explosion_hand_down_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_hand_down_1_xpm, explosion_hand_down_2_xpm, explosion_hand_down_3_xpm, explosion_hand_down_4_xpm};
static const char **explosion_hand_left_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_hand_left_1_xpm, explosion_hand_left_2_xpm, explosion_hand_left_3_xpm, explosion_hand_left_4_xpm};
static const char **explosion_hand_right_xpms[EXPLOSION_ANIM_FRAMES] = {explosion_hand_right_1_xpm, explosion_hand_right_2_xpm, explosion_hand_right_3_xpm, explosion_hand_right_4_xpm};

int(create_resources)(void) {
  for (uint8_t i = 0; i < BOMB_ANIM_FRAMES; i++) {
    resources.bomb_sprites[i] = create_sprite(bomb_xpms[i]);
    if (!resources.bomb_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load bomb anim frame %u", i);
      return 1;
    }
  }

  for (uint8_t i = 0; i < EXPLOSION_ANIM_FRAMES; i++) {
    resources.explosion_center_sprites[i] = create_sprite(explosion_center_xpms[i]);
    resources.explosion_horiz_sprites[i] = create_sprite(explosion_horiz_xpms[i]);
    resources.explosion_vert_sprites[i] = create_sprite(explosion_vert_xpms[i]);
    resources.explosion_hand_up_sprites[i] = create_sprite(explosion_hand_up_xpms[i]);
    resources.explosion_hand_down_sprites[i] = create_sprite(explosion_hand_down_xpms[i]);
    resources.explosion_hand_left_sprites[i] = create_sprite(explosion_hand_left_xpms[i]);
    resources.explosion_hand_right_sprites[i] = create_sprite(explosion_hand_right_xpms[i]);

    if (!resources.explosion_center_sprites[i] || !resources.explosion_horiz_sprites[i] ||
        !resources.explosion_vert_sprites[i] || !resources.explosion_hand_up_sprites[i] ||
        !resources.explosion_hand_down_sprites[i] || !resources.explosion_hand_left_sprites[i] ||
        !resources.explosion_hand_right_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load explosion sprite frame %u", i);
      return 1;
    }
  }

  resources.brick_sprites[0] = create_sprite(brick_0_xpm);
  resources.brick_sprites[1] = create_sprite(brick_1_xpm);
  resources.brick_sprites[2] = create_sprite(brick_2_xpm);
  for (uint8_t i = 0; i < 3; i++) {
    if (resources.brick_sprites[i] == NULL) {
      fprintf(stderr, "create_resources: failed to load brick sprite.");
      return 1;
    }
  }

  resources.door_sprite = create_sprite(door_xpm);
  if (resources.door_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create door sprite.");
    return 1;
  }

  for (uint8_t i = 0; i < ENEMY_ANIM_FRAMES; i++) {
    resources.enemy_a_sprites[i] = create_sprite(enemy_a_xpms[i]);
    if (!resources.enemy_a_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load enemy_%d", i);
      return 1;
    }
  }

  resources.handopen_sprite = create_sprite(handopen_xpm);
  if (resources.handopen_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create handopen sprite.");
    return 1;
  }

  resources.handpointing_sprite = create_sprite(handpointing_xpm);
  if (resources.handpointing_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create handpointing sprite.");
    return 1;
  }

  resources.heart_sprite = create_sprite(heart_xpm);
  if (resources.heart_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create heart sprite.");
    return 1;
  }

  resources.lose_sprite = create_sprite(lose_xpm);
  if (resources.lose_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create lose sprite.");
    return 1;
  }

  resources.menu_sprite[0] = create_sprite(menu_default_xpm);
  resources.menu_sprite[1] = create_sprite(menu_start_xpm);
  resources.menu_sprite[2] = create_sprite(menu_exit_xpm);
  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_sprite[i] == NULL) {
      fprintf(stderr, "create_resources: failed to load menu sprite.");
      return 1;
    }
  }

  resources.menu_pause[0] = create_sprite(menu_pause_default_xpm);
  resources.menu_pause[1] = create_sprite(menu_pause_continue_xpm);
  resources.menu_pause[2] = create_sprite(menu_pause_exit_xpm);
  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_pause[i] == NULL) {
      fprintf(stderr, "create_resources: failed to load pause menu sprite.");
      return 1;
    }
  }

  for (uint8_t i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    resources.player_down_sprites[i] = create_sprite(player_down_xpms[i]);
    resources.player_up_sprites[i] = create_sprite(player_up_xpms[i]);
    resources.player_left_sprites[i] = create_sprite(player_left_xpms[i]);
    resources.player_right_sprites[i] = create_sprite(player_right_xpms[i]);
    if (!resources.player_down_sprites[i] || !resources.player_up_sprites[i] || !resources.player_left_sprites[i] || !resources.player_right_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load player anim frame %u", i);
      return 1;
    }
  }
  player_anims[UP] = create_animSprite(resources.player_up_sprites, PLAYER_ANIM_FRAMES, PLAYER_ANIM_SPEED, false);
  player_anims[RIGHT] = create_animSprite(resources.player_right_sprites, PLAYER_ANIM_FRAMES, PLAYER_ANIM_SPEED, false);
  player_anims[DOWN] = create_animSprite(resources.player_down_sprites, PLAYER_ANIM_FRAMES, PLAYER_ANIM_SPEED, false);
  player_anims[LEFT] = create_animSprite(resources.player_left_sprites, PLAYER_ANIM_FRAMES, PLAYER_ANIM_SPEED, false);

  resources.powerup_sprite = create_sprite(powerup_xpm);
  if (resources.powerup_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create powerup sprite.");
    return 1;
  }

  resources.numbers_sprite = create_sprite(numbers_xpm);
  if (resources.numbers_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create numbers sprite.");
    return 1;
  }

  resources.alphabet_sprite = create_sprite(alphabet_xpm);
  if (resources.alphabet_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create alphabet sprite.");
    return 1;
  }

  resources.wall_sprite = create_sprite(wall_xpm);
  if (resources.wall_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create wall sprite.");
    return 1;
  }

  resources.win_sprite = create_sprite(win_xpm);
  if (resources.win_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create win sprite.");
    return 1;
  }

  return 0;
}

void(destroy_resources)(void) {
  for (uint8_t i = 0; i < BOMB_ANIM_FRAMES; i++) {
    if (resources.bomb_sprites[i]) {
      destroy_sprite(resources.bomb_sprites[i]);
      resources.bomb_sprites[i] = NULL;
    }
  }

  for (uint8_t i = 0; i < EXPLOSION_ANIM_FRAMES; i++) {
    destroy_sprite(resources.explosion_center_sprites[i]);
    destroy_sprite(resources.explosion_horiz_sprites[i]);
    destroy_sprite(resources.explosion_vert_sprites[i]);
    destroy_sprite(resources.explosion_hand_up_sprites[i]);
    destroy_sprite(resources.explosion_hand_down_sprites[i]);
    destroy_sprite(resources.explosion_hand_left_sprites[i]);
    destroy_sprite(resources.explosion_hand_right_sprites[i]);

    resources.explosion_center_sprites[i] = NULL;
    resources.explosion_horiz_sprites[i] = NULL;
    resources.explosion_vert_sprites[i] = NULL;
    resources.explosion_hand_up_sprites[i] = NULL;
    resources.explosion_hand_down_sprites[i] = NULL;
    resources.explosion_hand_left_sprites[i] = NULL;
    resources.explosion_hand_right_sprites[i] = NULL;
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.brick_sprites[i]) {
      destroy_sprite(resources.brick_sprites[i]);
      resources.brick_sprites[i] = NULL;
    }
  }

  if (resources.door_sprite != NULL) {
    destroy_sprite(resources.door_sprite);
    resources.door_sprite = NULL;
  }

  for (uint8_t i = 0; i < ENEMY_ANIM_FRAMES; i++) {
    if (resources.enemy_a_sprites[i]) {
      destroy_sprite(resources.enemy_a_sprites[i]);
      resources.enemy_a_sprites[i] = NULL;
    }
  }

  if (resources.handopen_sprite != NULL) {
    destroy_sprite(resources.handopen_sprite);
    resources.handopen_sprite = NULL;
  }

  if (resources.handpointing_sprite != NULL) {
    destroy_sprite(resources.handpointing_sprite);
    resources.handpointing_sprite = NULL;
  }

  if (resources.heart_sprite != NULL) {
    destroy_sprite(resources.heart_sprite);
    resources.heart_sprite = NULL;
  }

  if (resources.lose_sprite != NULL) {
    destroy_sprite(resources.lose_sprite);
    resources.lose_sprite = NULL;
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_sprite[i]) {
      destroy_sprite(resources.menu_sprite[i]);
      resources.menu_sprite[i] = NULL;
    }
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_pause[i]) {
      destroy_sprite(resources.menu_pause[i]);
      resources.menu_pause[i] = NULL;
    }
  }

  for (uint8_t i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    if (resources.player_down_sprites[i] != NULL) {
      destroy_sprite(resources.player_down_sprites[i]);
      resources.player_down_sprites[i] = NULL;
    }
    if (resources.player_up_sprites[i] != NULL) {
      destroy_sprite(resources.player_up_sprites[i]);
      resources.player_up_sprites[i] = NULL;
    }
    if (resources.player_left_sprites[i] != NULL) {
      destroy_sprite(resources.player_left_sprites[i]);
      resources.player_left_sprites[i] = NULL;
    }
    if (resources.player_right_sprites[i] != NULL) {
      destroy_sprite(resources.player_right_sprites[i]);
      resources.player_right_sprites[i] = NULL;
    }
  }

  if (resources.powerup_sprite != NULL) {
    destroy_sprite(resources.powerup_sprite);
    resources.powerup_sprite = NULL;
  }

  if (resources.numbers_sprite != NULL) {
    destroy_sprite(resources.numbers_sprite);
    resources.numbers_sprite = NULL;
  }

  if (resources.alphabet_sprite != NULL) {
    destroy_sprite(resources.alphabet_sprite);
    resources.alphabet_sprite = NULL;
  }

  if (resources.wall_sprite != NULL) {
    destroy_sprite(resources.wall_sprite);
    resources.wall_sprite = NULL;
  }

  if (resources.lose_sprite != NULL) {
    destroy_sprite(resources.lose_sprite);
    resources.lose_sprite = NULL;
  }

  for (int i = 0; i < 4; i++) {
    if (player_anims[i]) {
      destroy_animSprite(player_anims[i]);
      player_anims[i] = NULL;
    }
  }
}

const Resources *(get_resources) (void) {
  return &resources;
}

AnimSprite *get_player_anim(Direction dir) {
  return player_anims[dir];
}
