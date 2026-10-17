/** 
 * @file animations.c
 * @brief Source code file for all the logic related to the animations, including the loading and destroying
 */


#include "animations.h"

animation* pharaoh_animations;
animation* enemy1_animations;
animation* enemy2_animations;
animation* mummy_animations;
animation* secret_animations;

animation khopesh_attack_right_animation;
animation khopesh_attack_left_animation;
animation lightning_attack_animation;
animation heart_animation;


enum animation_direction (entity_get_direction)(entity* entity) {
  if(entity->speed_x == 0 && entity->speed_y == 0) return IDLE_;
  if(entity->speed_x > 0 || entity->last_horizontal_direction) return RIGHT;
  return LEFT;
}


int (animations_clean)() {
  for(int i = 0; i < 2; ++i) {
    animation_destroy(pharaoh_animations[i]);
    animation_destroy(enemy1_animations[i]);
    animation_destroy(enemy2_animations[i]);
    animation_destroy(mummy_animations[i]);
  }
    
  animation_destroy(khopesh_attack_right_animation);
  animation_destroy(khopesh_attack_left_animation);
  animation_destroy(lightning_attack_animation);
  animation_destroy(heart_animation);
  animation_destroy(secret_animations[0]);

  free(pharaoh_animations);
  free(enemy1_animations);
  free(enemy2_animations);
  free(mummy_animations);
  free(secret_animations);

  return 0;
}

int (animation_destroy)(animation animation) {
  for(uint32_t i = 0; i < animation.n_frames; ++i){
    free(animation.sprites[i]);
  }
  free(animation.sprites);

  return 0;
}

static int (load_pharaoh_animations)() {
  #include "../resources/Pharaoh Idle Down/pharaoh_idle_down.h"
  #include "../resources/Pharaoh Walk Left/pharaoh_walk_left.h"
  #include "../resources/Pharaoh Walk Right/pharaoh_walk_right.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  pharaoh_animations = (animation*)malloc(sizeof(animation) * 3); //one for each direction
  
  //pharaoh idle down
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 16);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_idle_down_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  sprites[1] = current_image;
  sprites[2] = current_image;
  sprites[3] = current_image;

  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_idle_down_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  sprites[5] = current_image;
  sprites[6] = current_image;
  sprites[7] = current_image;

  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_idle_down_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[8] = current_image;
  sprites[9] = current_image;
  sprites[10] = current_image;
  sprites[11] = current_image;

  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_idle_down_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[12] = current_image;
  sprites[13] = current_image;
  sprites[14] = current_image;
  sprites[15] = current_image;

  pharaoh_animations[IDLE_] = (animation){16, sprites};


  //pharaoh walk left
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_left_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  pharaoh_animations[LEFT] = (animation){8, sprites};


  //pharaoh walk right
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(pharaoh_walk_right_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  pharaoh_animations[RIGHT] = (animation){8, sprites};


  return 0;
}


static int (load_enemy1_animations)() {
  #include "../resources/Enemy 1 Walk Right/enemy1_walk_right.h"
  #include "../resources/Enemy 1 Walk Left/enemy1_walk_left.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  enemy1_animations = (animation*)malloc(sizeof(animation) * 3); //one for each direction

  //enemy1 walk left
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_left_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  enemy1_animations[LEFT] = (animation){8, sprites};

  //enemy1 walk right
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy1_walk_right_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  enemy1_animations[RIGHT] = (animation){8, sprites};
  enemy1_animations[IDLE_] = (animation){8, sprites};

  return 0;
}

static int (load_enemy2_animations)() {
  #include "../resources/Enemy 2 Walk Right/enemy2_walk_right.h"
  #include "../resources/Enemy 2 Walk Left/enemy2_walk_left.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  enemy2_animations = (animation*)malloc(sizeof(animation) * 3); //one for each direction

  //enemy2 walk left
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_left_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  enemy2_animations[LEFT] = (animation){8, sprites};

  //enemy2 walk right
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(enemy2_walk_right_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  enemy2_animations[RIGHT] = (animation){8, sprites};
  enemy2_animations[IDLE_] = (animation){8, sprites};

  return 0;
}

static int (load_mummy_animations)() {
  #include "../resources/Mummy Walk Right/mummy_walk_right.h"
  #include "../resources/Mummy Walk Left/mummy_walk_left.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  mummy_animations = (animation*)malloc(sizeof(animation) * 3); //one for each direction

  //mummy walk left
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_left_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  mummy_animations[LEFT] = (animation){8, sprites};

  //mummy walk right
  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 8);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(mummy_right_7, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[7] = current_image;

  mummy_animations[RIGHT] = (animation){8, sprites};
  mummy_animations[IDLE_] = (animation){8, sprites};

  return 0;
}

static int (load_khopesh_attack_animations)() {
  #include "../resources/Khopesh Attack Right/khopesh_attack_right.h"
  #include "../resources/Khopesh Attack Left/khopesh_attack_left.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 3);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_right_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_right_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_right_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;

  khopesh_attack_right_animation = (animation){3, sprites};


  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 3);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_left_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_left_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(khopesh_attack_left_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;

  khopesh_attack_left_animation = (animation){3, sprites};

  return 0;
}

static int (load_lightning_attack_animations)() {
  #include "../resources/Lightning Attack/lightning_attack.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 7);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_0, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_1, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_2, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[2] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_3, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[3] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_4, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[4] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_5, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[5] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(lightning_attack_6, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[6] = current_image;

  lightning_attack_animation = (animation){7, sprites};

  return 0;
}

static int (load_heart_animation)() {
  #include "../resources/Heart/heart.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 2);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(heart_0_xpm, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(heart_1_xpm, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;

  heart_animation = (animation){2, sprites};

  return 0;
}

static int (load_secret_animation)() {
  #include "../resources/Secret/secret.h"

  xpm_image_t** sprites;
  xpm_image_t* current_image;

  secret_animations = (animation*)malloc(sizeof(animation) * 3); //one for each direction

  sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 2);
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(secret_0_xpm, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[0] = current_image;
  
  current_image = (xpm_image_t*)malloc(sizeof(xpm_image_t));
  if(xpm_load(secret_1_xpm, XPM_8_8_8_8, current_image) == NULL) return 1;
  sprites[1] = current_image;

  secret_animations[RIGHT] = (animation){2, sprites};
  secret_animations[LEFT] = (animation){2, sprites};
  secret_animations[IDLE_] = (animation){2, sprites};

  return 0;
}

int (animations_load)() {
  // Load pharaoh & enemy1 animations
  if(load_pharaoh_animations()) return 1;
  if(load_enemy1_animations()) return 1;
  if(load_enemy2_animations()) return 1;
  if(load_mummy_animations()) return 1;
  if(load_khopesh_attack_animations()) return 1;
  if(load_lightning_attack_animations()) return 1;
  if(load_heart_animation()) return 1;
  if(load_secret_animation()) return 1;
  return 0;
}
