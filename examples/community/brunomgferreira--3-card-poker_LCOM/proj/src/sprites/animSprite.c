
#include <lcom/lcf.h>
#include "animSprite.h"

AnimSprite *(create_animSprite)(Sprite *sprites[], int x, int y, int xspeed, int yspeed) {
    if (sprites == NULL) {
        return NULL;
    }

    // Allocate space for the AnimSprite object
    AnimSprite *animSprite = (AnimSprite *) malloc(sizeof(AnimSprite));

    if (animSprite == NULL) {
        return NULL;
    }

    // Count the number of sprites and copy the pointers to the map
    uint32_t num_sprites = 0;
    while (sprites[num_sprites] != NULL && num_sprites < 5) {
        animSprite->map[num_sprites] = sprites[num_sprites];
        num_sprites++;
    }

    // Initialize the remaining fields of AnimSprite
    animSprite->num_sprites = num_sprites;
    animSprite->curr_sprite = 0; // Start with the first sprite
    animSprite->x = x;
    animSprite->y = y;
    animSprite->xspeed = xspeed;
    animSprite->yspeed = yspeed;
    animSprite->xScale = 1;
    animSprite->yScale = 1;

    return animSprite;
}

void (destroy_animSprite)(AnimSprite *animsp) {
    if (animsp == NULL)
        return;
    
    free(animsp);

    animsp = NULL;
}

void (animSprite_horizontal_flip)(AnimSprite *animsp, uint32_t first, uint32_t last, double speed) {
  if (animsp == NULL) return;

  animsp->yScale = 1;
  animsp->yScale = 1;

  Sprite *first_sp = animsp->map[first];
  Sprite *last_sp = animsp->map[last];

  if(first_sp == NULL) return;
  if(last_sp == NULL) return;

  set_sprite_position(first_sp, animsp->x, animsp->y);
  set_sprite_position(last_sp, animsp->x, animsp->y);

  if (animsp->curr_sprite == last) {
      draw_scaled_sprite(last_sp, animsp->xScale, animsp->yScale);
      animsp->xScale += speed;
      if (animsp->xScale >= 1) {
          animsp->curr_sprite = first;
          animsp->xScale = 1;
      }
  } else {
      draw_scaled_sprite(first_sp, animsp->xScale, animsp->yScale);
      animsp->xScale -= speed;
      if (animsp->xScale <= 0) {
          animsp->curr_sprite = last;
          animsp->xScale = 0;
      }
  }
}

void (set_animSprite_position)(AnimSprite *animsp, int x, int y) {
  if (animsp == NULL) return;
  animsp->x = x;
  animsp->y = y;
}

void (set_animSprite_speed)(AnimSprite *animsp, int xspeed, int yspeed) {
  if (animsp == NULL) return;
  animsp->xspeed = xspeed;
  animsp->yspeed = yspeed;
}

void (draw_animSprite)(AnimSprite *animsp, uint32_t i) {
  if (animsp == NULL) return;

  Sprite *sp = animsp->map[i];

  if (sp == NULL) return;

  set_sprite_position(sp, animsp->x, animsp->y);
  draw_sprite(sp);
}

void (move_animSprite)(AnimSprite *animSp, int destx, int desty) {
  if (animSp == NULL)
    return;

  animSp->x += animSp->xspeed;
  animSp->y += animSp->yspeed;

  if (animSp->xspeed < 0) animSp->x = animSp->x >= destx ? animSp->x : destx;
  else animSp->x = animSp->x <= destx ? animSp->x : destx;

  if (animSp->yspeed < 0) animSp->y = animSp->y >= desty ? animSp->y : desty;
  else animSp->y = animSp->y <= desty ? animSp->y : desty;

  return;
}

void (create_animSprites)() {
    Sprite *coins[] = { coin_sprite, coin_sprite, NULL };
    Sprite *coins_shadow[] = { coin_shadow_sprite, coin_shadow_sprite, NULL };
    Sprite *sprites[] = { card_back, card_back, NULL };
    
    coin_1 = create_animSprite(coins, 0, 0, 0, 0);
    coin_2 = create_animSprite(coins, 0, 0, 0, 0);
    coin_shadow_1 = create_animSprite(coins_shadow, 0, 0, 0, 0);
    coin_shadow_2 = create_animSprite(coins_shadow, 0, 0, 0, 0);

    player_card_1 = create_animSprite(sprites, 0, 0, -8, 16);
    player_card_2 = create_animSprite(sprites, 0, 0, -8, 16);
    player_card_3 = create_animSprite(sprites, 0, 0, -8, 16);

    dealer_card_1 = create_animSprite(sprites, 0, 0, -8, 16);
    dealer_card_2 = create_animSprite(sprites, 0, 0, -8, 16);
    dealer_card_3 = create_animSprite(sprites, 0, 0, -8, 16);
}

void (destroy_animSprites)() {
    destroy_animSprite(coin_1);
    destroy_animSprite(coin_2);
    destroy_animSprite(coin_shadow_1);
    destroy_animSprite(coin_shadow_2);

    destroy_animSprite(player_card_1);
    destroy_animSprite(player_card_2);
    destroy_animSprite(player_card_3);

    destroy_animSprite(dealer_card_1);
    destroy_animSprite(dealer_card_2);
    destroy_animSprite(dealer_card_3);
}
