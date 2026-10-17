#include "sprite.h"

Sprite *(create_sprite)(xpm_map_t xpm) {

  Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));
  if (sprite == NULL)
    return NULL;

  xpm_image_t img;
  sprite->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
  sprite->height = img.height;
  sprite->width = img.width;

  if (sprite->map == NULL) {
    free(sprite);
    return NULL;
  }

  return sprite;
}

int (draw_sprite)(Sprite *sprite, int x, int y) {
  uint32_t *aux = sprite->map;
  int xBeginning = x - (sprite->width / 2);
  int yBeginning = y - (sprite->height / 2);

  for (int i = 0; i < sprite->height; i++) {
    for (int j = 0; j < sprite->width; j++) {
      if (*aux == TRANSPARENT) {
        aux++;
        continue;
      }

      if (vg_draw_pixel(xBeginning + j, yBeginning + i, *aux))
        return EXIT_FAILURE;
      aux++;
    }
  }
  return EXIT_SUCCESS;
}

void loadMenu() {
  singleplayer = create_sprite(singleplayer_xpm);
  multiplayer = create_sprite(multiplayer_xpm);
  quit = create_sprite(quit_xpm);
}

void (loadGameOver)() {
  GameOver = (create_sprite)(gameover_xpm);
  GameOverESC = (create_sprite)(gameoverEsc_xpm);
}

int (load_sprites)() {
  loadMenu();
  aim = (create_sprite)(aim_xpm);
  dayDesert = (create_sprite)(dayDesert_xpm);
  nightDesert = (create_sprite)(nightDesert_xpm);
  target = (create_sprite)(target_xpm);
  dynamiteIcon = (create_sprite)(dynamite_xpm);
  clockIcon = (create_sprite)(clock_xpm);
  scoreSprite = (create_sprite)(score_xpm);
  waitMessage = (create_sprite)(wait_xpm);
  title = (create_sprite)(title_xpm);
  lost = (create_sprite)(lost_xpm);
  win = (create_sprite)(win_xpm);
  for (int i = 0; i < 10; i++) {
    numbers[i] = (create_sprite)(numbers_xpm[i]);
  }
  for (int i = 0; i < 3; i++) {
    fall[i] = (create_sprite)(fall_xpm[i]);
  }
  for (int i = 0; i < 5; i++) {
    explosion[i] = (create_sprite)(explosion_xpm[i]);
  }
  dots = (create_sprite)(dots_xpm);
  (loadGameOver)();
  return EXIT_SUCCESS;
}

int (delete_sprites)() {
  if (aim != NULL) {
    free(aim->map);
    free(aim);
  }
  if (dayDesert != NULL) {
    free(dayDesert->map);
    free(dayDesert);
  }
  if (nightDesert != NULL) {
    free(nightDesert->map);
    free(nightDesert);
  }
  if (target != NULL) {
    free(target->map);
    free(target);
  }
  if (dynamiteIcon != NULL) {
    free(dynamiteIcon->map);
    free(dynamiteIcon);
  }
  if (clockIcon != NULL) {
    free(clockIcon->map);
    free(clockIcon);
  }
  if (scoreSprite != NULL) {
    free(scoreSprite->map);
    free(scoreSprite);
  }
  for (int i = 0; i < 10; i++) {
    if (numbers[i] != NULL) {
      free(numbers[i]->map);
      free(numbers[i]);
    }
  }
  for (int i = 0; i < 3; i++) {
    if (fall[i] != NULL) {
      free(fall[i]->map);
      free(fall[i]);
    }
  }
  for (int i = 0; i < 5; i++) {
    if (explosion[i] != NULL) {
      free(explosion[i]->map);
      free(explosion[i]);
    }
  }
  if(lost != NULL) {
    free(lost->map);
    free(lost);
  }
  if(win != NULL) {
    free(win->map);
    free(win);
  }
  if (GameOver != NULL) {
    free(GameOver->map);
    free(GameOver);
  }
  if (GameOverESC != NULL) {
    free(GameOverESC->map);
    free(GameOverESC);
  }
  if (dots != NULL) {
    free(dots->map);
    free(dots);
  }
  if (singleplayer != NULL) {
    free(singleplayer->map);
    free(singleplayer);
  }
  if (multiplayer != NULL) {
    free(multiplayer->map);
    free(multiplayer);
  }

  if (quit != NULL) {
    free(quit->map);
    free(quit);
  }
  return EXIT_SUCCESS;
}
