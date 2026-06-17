#include "enemy_card.h"

#include <stdio.h>
#include <string.h>

#include "video_gr_ext.h"
#include "colors.h"
#include "panel.h"
#include "scene.h"
#include "sprite_utils.h"

#define ENEMY_CARD_WIDTH 72
#define ENEMY_CARD_HEIGHT 79
#define ENEMY_CARD_BORDER_THICKNESS 2

static int scene_draw_enemy_card_border(vg_sprite_t *sprite, uint32_t border_color)
{
  const uint16_t t = ENEMY_CARD_BORDER_THICKNESS;
  return sprite_utils_fill_rect(sprite, 0, 0, ENEMY_CARD_WIDTH, t, border_color) != 0 ||
         sprite_utils_fill_rect(sprite, 0, (uint16_t)(ENEMY_CARD_HEIGHT - t), ENEMY_CARD_WIDTH, t, border_color) != 0 ||
         sprite_utils_fill_rect(sprite, 0, 0, t, ENEMY_CARD_HEIGHT, border_color) != 0 ||
         sprite_utils_fill_rect(sprite, (uint16_t)(ENEMY_CARD_WIDTH - t), 0, t, ENEMY_CARD_HEIGHT, border_color);
}

static int scene_build_enemy_card_background(vg_sprite_t *sprite, uint32_t border_color)
{
  if (sprite == NULL)
    return 1;

  if (sprite->pixels == NULL)
  {
    if (sprite_utils_create(sprite, ENEMY_CARD_WIDTH, ENEMY_CARD_HEIGHT) != 0)
    {
      printf("scene_build_enemy_card_background(): failed to create %ux%u sprite\n",
             (unsigned)ENEMY_CARD_WIDTH, (unsigned)ENEMY_CARD_HEIGHT);
      return 1;
    }

    if (panel_draw_to_sprite(sprite, 0, 0, ENEMY_CARD_WIDTH, ENEMY_CARD_HEIGHT) != 0)
    {
      printf("scene_build_enemy_card_background(): failed to draw panel\n");
      return 1;
    }
  }

  if (border_color == 0)
    return 0;

  if (scene_draw_enemy_card_border(sprite, border_color) != 0)
  {
    printf("scene_build_enemy_card_background(): failed to draw border\n");
    return 1;
  }

  return 0;
}

static int scene_copy_enemy_card_background(vg_sprite_t *dst, const vg_sprite_t *src,
                                             uint32_t border_color)
{
  if (dst == NULL || src == NULL || src->pixels == NULL)
    return 1;

  if (dst->pixels == NULL)
  {
    if (sprite_utils_create(dst, ENEMY_CARD_WIDTH, ENEMY_CARD_HEIGHT) != 0)
    {
      printf("scene_copy_enemy_card_background(): failed to create %ux%u sprite\n",
             (unsigned)ENEMY_CARD_WIDTH, (unsigned)ENEMY_CARD_HEIGHT);
      return 1;
    }
  }

  size_t bytes = (size_t)src->width * src->height * src->bytes_per_pixel;
  memcpy(dst->pixels, src->pixels, bytes);

  if (border_color != 0 && scene_draw_enemy_card_border(dst, border_color) != 0)
    return 1;

  return 0;
}

int scene_build_enemy_card_backgrounds(scene_t *scene)
{
  if (scene == NULL)
    return 1;

  if (scene_build_enemy_card_background(&scene->enemy_card_bg_normal, 0) != 0)
  {
    printf("scene_build_enemy_card_backgrounds(): failed to build normal background\n");
    return 1;
  }

  if (scene_copy_enemy_card_background(&scene->enemy_card_bg_hover,
                                        &scene->enemy_card_bg_normal, COLOR_GRASS_LIGHT) != 0)
  {
    printf("scene_build_enemy_card_backgrounds(): failed to build hover background\n");
    vg_destroy_sprite(&scene->enemy_card_bg_normal);
    return 1;
  }

  if (scene_copy_enemy_card_background(&scene->enemy_card_bg_active,
                                        &scene->enemy_card_bg_normal, COLOR_TEXT_SOFT) != 0)
  {
    printf("scene_build_enemy_card_backgrounds(): failed to build active background\n");
    vg_destroy_sprite(&scene->enemy_card_bg_normal);
    vg_destroy_sprite(&scene->enemy_card_bg_hover);
    return 1;
  }

  return 0;
}
