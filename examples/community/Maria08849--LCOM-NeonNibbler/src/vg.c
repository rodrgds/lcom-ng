#include "vg.h"

/** @defgroup Video_Graphics
 * @{
 *
 * Functions related to the use of the video graphics in the project
 */

static uint8_t *front_buffer;
static uint8_t *back_buffer;
static uint8_t *menu_background;
static uint8_t *game_background;

static uint8_t bitsPerPixel;
static uint8_t bytesPerPixel;
static uint16_t yResolution;
static uint16_t xResolution;

int powerup1 = 0;
int powerup2 = 0;
int collectible1 = 0;
int collectible2 = 0;
int collectible3 = 0;
int collectible4 = 0;
int collectible5 = 0;
int collectible6 = 0;
int collectible7 = 0;
int collectible8 = 0;
int collectible9 = 0;
int collectible10 = 0;
int collectible11 = 0;
int collectible12 = 0;
int collectible13 = 0;
int collectible14 = 0;
int collectible15 = 0;
int collectible16 = 0;

vbe_mode_info_t info;

unsigned int vram_size;

/**
 * @brief Initializes the video graphics mode.
 *
 * @param mode The video mode to set.
 * @return A pointer to the front buffer upon success, or NULL otherwise.
 */
void *(vg_init) (uint16_t mode) {
  vbe_get_mode_info(mode, &info);
  bitsPerPixel = info.BitsPerPixel;
  bytesPerPixel = (bitsPerPixel + 7) / 8;
  xResolution = info.XResolution;
  yResolution = info.YResolution;

  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = 0x10;             /* BIOS video services */
  r86.ax = 0x4F02;              /* Set Video Mode function */
  r86.bx = (1 << 14) | mode;    /* 80x25 text mode */

  if (sys_int86(&r86) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return NULL;
  }

  /* Specify the appropriate register values */
  struct minix_mem_range mr;

  int r;
  vram_size = info.XResolution * info.YResolution * bytesPerPixel;
  /* Make the BIOS call */
  mr.mr_base = (phys_bytes) info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  menu_background = malloc(vram_size);
  game_background = malloc(vram_size);
  back_buffer = malloc(vram_size);
  front_buffer = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (front_buffer == MAP_FAILED)
    panic("couldn't map video memory\n");

  createsprites();

  return (void *) front_buffer;
}

/**
 * @brief Stores the menu background in the buffer.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_store_menu_background)() {
  Sprite *sp;
  rtc_init();
  rtc_time();
  if (isDaytime()) {
    sp = get_day_background_sprite();
  }
  else {
    sp = get_night_background_sprite();
  }
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sp->colors[w + h * width];
      if (current_color == TRANSPARENCY_COLOR_8_8_8_8)
        continue;
      unsigned int index = (xResolution * h + w) * bytesPerPixel;
      if (memcpy(&menu_background[index], &current_color, bytesPerPixel) == NULL)
        return 1;
    }
  }

  if (vg_draw_sprite_in_buffer(menu_background, get_logo_sprite(), xResolution / 2 - 160, yResolution - 725) != 0)
    return 1;

  return 0;
}

/**
 * @brief Stores the game background in the buffer.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_store_game_background)() {
  Sprite *sp = get_game_background_sprite();
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sp->colors[w + h * width];
      if (current_color == TRANSPARENCY_COLOR)
        continue;
      unsigned int index = (xResolution * h + w) * bytesPerPixel;
      if (memcpy(&game_background[index], &current_color, bytesPerPixel) == NULL)
        return 1;
    }
  }
  return 0;
}

/**
 * @brief Frees the allocated buffers.
 */
void(free_buffers)() {
  free(back_buffer);
  free(menu_background);
  free(game_background);
}

/**
 * @brief Copies the menu background buffer to the back buffer.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_copy_menu_background_buffer)() {
  if (memcpy(back_buffer, menu_background, vram_size - 1) == NULL)
    return 1;
  return 0;
}

/**
 * @brief Copies the game background buffer to the back buffer.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_copy_game_background_buffer)() {
  if (memcpy(back_buffer, game_background, vram_size - 1) == NULL)
    return 1;
  return 0;
}

/**
 * @brief Copies the back buffer to the front buffer.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_copy_back_buffer)() {
  if (memcpy(front_buffer, back_buffer, vram_size - 1) == NULL)
    return 1;
  return 0;
}

/**
 * @brief Draws a pixel at the specified position in the back buffer.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x > xResolution || y > yResolution) {
    return 0;
  }

  unsigned int index = (xResolution * y + x) * bytesPerPixel;

  if (memcpy(&back_buffer[index], &color, bytesPerPixel) == NULL)
    return 1;
  return 0;
}

/**
 * @brief Draws a pixel at the specified position in the specified buffer.
 *
 * @param buffer The buffer to draw the pixel in.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_pixel_in_buffer)(uint8_t *buffer, uint16_t x, uint16_t y, uint32_t color) {
  if (x > xResolution || y > yResolution) {
    return 0;
  }

  unsigned int index = (xResolution * y + x) * bytesPerPixel;

  if (memcpy(&buffer[index], &color, bytesPerPixel) == NULL)
    return 1;
  return 0;
}

/**
 * @brief Fills the screen with the specified color.
 *
 * @param color The color to fill the screen with.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_fill_screen)(uint32_t color) {
  uint32_t pixel_color = color;
  for (int y = 0; y < yResolution; y++) {
    for (int x = 0; x < xResolution; x++) {
      if (vg_draw_pixel(x, y, pixel_color) != 0) {
        printf("Error filling screen\n");
        vg_exit();
        return 1;
      }
    }
  }
  return 0;
}

/**
 * @brief Draws a sprite at the specified position in the back buffer.
 *
 * @param sp The sprite to draw.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_sprite)(Sprite *sp, uint16_t x, uint16_t y) {
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sp->colors[w + h * width];
      if (current_color == TRANSPARENCY_COLOR || current_color == TRANSPARENCY_COLOR_8_8_8_8)
        continue;
      if (vg_draw_pixel(x + w, y + h, current_color) != 0)
        return 1;
    }
  }
  return 0;
}

/**
 * @brief Draws a sprite in the specified buffer at the specified position.
 *
 * @param buffer The buffer to draw the sprite in.
 * @param sp The sprite to draw.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_sprite_in_buffer)(uint8_t *buffer, Sprite *sp, uint16_t x, uint16_t y) {
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sp->colors[w + h * width];
      if (current_color == TRANSPARENCY_COLOR || current_color == TRANSPARENCY_COLOR_8_8_8_8)
        continue;
      if (vg_draw_pixel_in_buffer(buffer, x + w, y + h, current_color) != 0)
        return 1;
    }
  }
  return 0;
}

/**
 * @brief Draws a part of a sprite in the back buffer at the specified position.
 *
 * @param sp The sprite to draw.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @param xi The starting x-coordinate of the part to draw.
 * @param yi The starting y-coordinate of the part to draw.
 * @param xf The ending x-coordinate of the part to draw.
 * @param yf The ending y-coordinate of the part to draw.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_part_of_sprite)(Sprite *sp, uint16_t x, uint16_t y, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf) {
  uint32_t current_color;
  for (int h = yi; h < yf; h++) {
    for (int w = xi; w < xf; w++) {
      current_color = sp->colors[w + h * sp->width];
      if (current_color == TRANSPARENCY_COLOR || current_color == TRANSPARENCY_COLOR_8_8_8_8)
        continue;
      if (vg_draw_pixel(x - xi + w, y - yi + h, current_color) != 0)
        return 1;
    }
  }
  return 0;
}

/**
 * @brief Draws the ghost score at the specified position.
 *
 * @param x The x-coordinate of the top-left corner of the ghost score.
 * @param y The y-coordinate of the top-left corner of the ghost score.
 * @param ghost_score The value of the ghost score.
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_ghost_score)(uint16_t x, uint16_t y, uint8_t ghost_score) {
  if (vg_draw_g(x, y) != 0)
    return 1;
  x += 20;
  if (vg_draw_h(x, y) != 0)
    return 1;
  x += 15;
  if (vg_draw_o(x, y) != 0)
    return 1;
  x += 19;
  if (vg_draw_s(x, y) != 0)
    return 1;
  x += 15;
  if (vg_draw_t(x, y - 1) != 0)
    return 1;
  x += 25;
  if (vg_draw_s(x, y) != 0)
    return 1;
  x += 15;
  if (vg_draw_c(x, y) != 0)
    return 1;
  x += 17;
  if (vg_draw_o(x, y) != 0)
    return 1;
  x += 19;
  if (vg_draw_r(x, y) != 0)
    return 1;
  x += 16;
  if (vg_draw_e(x, y) != 0)
    return 1;
  x += 15;
  if (vg_draw_colon(x, y) != 0)
    return 1;
  x += 13;
  if (ghost_score == 0) {
    if (vg_draw_0(x, y) != 0)
      return 1;
  }
  if (ghost_score == 1) {
    if (vg_draw_1(x, y) != 0)
      return 1;
  }
  else if (ghost_score == 2) {
    if (vg_draw_2(x, y) != 0)
      return 1;
  }
  return 0;
}

/**
 * @brief Draws the menu that doesn't have the play button selected.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_menu_nplay)() {
  if (vg_copy_menu_background_buffer() != 0)
    return 1;
  if (vg_draw_sprite(get_nplay_sprite(), xResolution / 2 - 75, yResolution - 425) != 0)
    return 1;
  if (vg_draw_sprite(get_cdoor_sprite(), xResolution - 1 - 133, yResolution - 1 - 30 - 150) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the menu where the play button is selected.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_menu_splay)() {
  if (vg_copy_menu_background_buffer() != 0)
    return 1;
  if (vg_draw_sprite(get_splay_sprite(), xResolution / 2 - 75, yResolution - 425) != 0)
    return 1;
  if (vg_draw_sprite(get_cdoor_sprite(), xResolution - 1 - 133, yResolution - 1 - 30 - 150) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the collectibles on the screen.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_collectibles)() {
  if (collectible1 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 70, 50) != 0)
      return 1;
  }
  if (collectible2 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 70 - 25, 50) != 0)
      return 1;
  }
  if (collectible3 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 70, 225) != 0)
      return 1;
  }
  if (collectible4 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 70 - 25, 225) != 0)
      return 1;
  }
  if (collectible5 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 500, 50) != 0)
      return 1;
  }
  if (collectible6 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 500 - 25, 50) != 0)
      return 1;
  }
  if (collectible7 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 500, 565) != 0)
      return 1;
  }
  if (collectible8 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 500 - 25, 565) != 0)
      return 1;
  }
  if (collectible9 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 70, 790) != 0)
      return 1;
  }
  if (collectible10 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 70 - 25, 790) != 0)
      return 1;
  }
  if (collectible11 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 70, 565) != 0)
      return 1;
  }
  if (collectible12 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 70 - 25, 565) != 0)
      return 1;
  }
  if (collectible13 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 70, 395) != 0)
      return 1;
  }
  if (collectible14 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 70 - 25, 395) != 0)
      return 1;
  }
  if (collectible15 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), 500, 720) != 0)
      return 1;
  }
  if (collectible16 == 0) {
    if (vg_draw_sprite(get_collectible_sprite(), xResolution - 500 - 25, 720) != 0)
      return 1;
  }
  return 0;
}

/**
 * @brief Draws the power-ups on the screen.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(vg_draw_powerups)() {
  if (powerup1 == 0) {
    if (vg_draw_sprite(get_powerup_sprite(), 255, 130) != 0)
      return 1;
  }
  if (powerup2 == 0) {
    if (vg_draw_sprite(get_powerup_sprite(), xResolution - 255 - 48, yResolution - 130 - 48 - 59) != 0)
      return 1;
  }
  return 0;
}

/**
 * @brief Checks if the sprite will collide with the background at the specified position.
 *
 * @param sprite The sprite to check for collision.
 * @param nextX The next X position of the sprite.
 * @param nextY The next Y position of the sprite.
 * @return True if the sprite will collide with the background, false otherwise.
 */
bool(will_collide_with_background)(Sprite *sprite, uint16_t nextX, uint16_t nextY) {
  if (nextX < 0 || nextX >= xResolution || nextY < 0 || nextY >= yResolution) {
    return true;
  }
  for (uint16_t h = nextY; h < sprite->height + nextY; h++) {
    for (uint16_t w = nextX; w < sprite->width + nextX; w++) {
      uint32_t current_color = game_background[(w + h * xResolution) * bytesPerPixel];

      if (current_color != TRANSPARENCY_COLOR && current_color != TRANSPARENCY_COLOR_8_8_8_8) {
        return true;
      }
    }
  }

  return false;
}

/**
 * @brief Checks if the sprite will collide with a power-up at the specified position.
 *
 * @param sprite The sprite to check for collision.
 * @param x The X position of the sprite.
 * @param y The Y position of the sprite.
 * @return True if the sprite will collide with a power-up, false otherwise.
 */
bool(will_collide_with_powerup)(Sprite *sprite, uint16_t x, uint16_t y) {
  if ((x < 255 + 50 && x > 255 && y < 130 + 48 && y > 130) && powerup1 == 0) {
    powerup1 = 1;
    return true;
  }
  else if ((x > xResolution - 255 - 48 && x < xResolution - 255 - 48 + 50 && y > yResolution - 130 - 48 - 59 && y < yResolution - 130 - 48 - 59 + 50) && powerup2 == 0) {
    powerup2 = 1;
    return true;
  }
  else {
    return false;
  }
}

/**
 * @brief Checks if the sprite will collide with a collectible at the specified position.
 *
 * @param sprite The sprite to check for collision.
 * @param x The X position of the sprite.
 * @param y The Y position of the sprite.
 * @return True if the sprite will collide with a collectible, false otherwise.
 */
bool(will_collide_with_collectible)(Sprite *sprite, uint16_t x, uint16_t y) {
  if (x < 70 + 50 && x > 70 && y < 50 + 50 && y > 50) {
    collectible1 = 1;
    return true;
  }
  if (x < xResolution - 70 - 25 + 50 && x > xResolution - 70 - 25 && y < 50 + 50 && y > 50) {
    collectible2 = 1;
    return true;
  }
  if (x < 70 + 50 && x > 70 && y < 225 + 50 && y > 225) {
    collectible3 = 1;
    return true;
  }
  if (x < xResolution - 70 - 25 + 50 && x > xResolution - 70 - 25 && y < 225 + 50 && y > 225) {
    collectible4 = 1;
    return true;
  }
  if (x < 500 + 50 && x > 500 && y < 50 + 50 && y > 50) {
    collectible5 = 1;
    return true;
  }
  if (x < xResolution - 500 - 25 + 50 && x > xResolution - 500 - 25 && y < 50 + 50 && y > 50) {
    collectible6 = 1;
    return true;
  }
  if (x < 500 + 50 && x > 500 && y < 565 + 50 && y > 565) {
    collectible7 = 1;
    return true;
  }
  if (x < xResolution - 500 - 25 + 50 && x > xResolution - 500 - 25 && y < 565 + 50 && y > 565) {
    collectible8 = 1;
    return true;
  }
  if (x < 70 + 50 && x > 70 && y < 790 + 50 && y > 790) {
    collectible9 = 1;
    return true;
  }
  if (x < xResolution - 70 - 25 + 50 && x > xResolution - 70 - 25 && y < 790 + 50 && y > 790) {
    collectible10 = 1;
    return true;
  }
  if (x < 70 + 50 && x > 70 && y < 565 + 50 && y > 565) {
    collectible11 = 1;
    return true;
  }
  if (x < xResolution - 70 - 25 + 50 && x > xResolution - 70 - 25 && y < 565 + 50 && y > 565) {
    collectible12 = 1;
    return true;
  }
  if (x < 70 + 50 && x > 70 && y < 395 + 50 && y > 395) {
    collectible13 = 1;
    return true;
  }
  if (x < xResolution - 70 - 25 + 50 && x > xResolution - 70 - 25 && y < 395 + 50 && y > 395) {
    collectible14 = 1;
    return true;
  }
  if (x < 500 + 50 && x > 500 && y < 720 + 50 && y > 720) {
    collectible15 = 1;
    return true;
  }
  if (x < xResolution - 500 - 25 + 50 && x > xResolution - 500 - 25 && y < 720 + 50 && y > 720) {
    collectible16 = 1;
    return true;
  }
  return false;
}

/**
 * @brief Verifies if two sprites have collided at the specified positions.
 *
 * @param sp1 The first sprite to check for collision.
 * @param sp2 The second sprite to check for collision.
 * @param x1 The X position of the first sprite.
 * @param y1 The Y position of the first sprite.
 * @param x2 The X position of the second sprite.
 * @param y2 The Y position of the second sprite.
 * @return True if the sprites have collided, false otherwise.
 */
bool(sprites_collided)(Sprite *sp1, Sprite *sp2, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  // collided on x axis && y axis
  if (((x1 <= x2 && x1 + sp1->width >= x2) || (x2 <= x1 && x2 + sp2->width >= x1)) && ((y1 <= y2 && y1 + sp1->height >= y2) || (y2 <= y1 && y2 + sp2->height >= y1))) {
    return true;
  }

  return false;
}

/**
 * @brief Checks if the game is over by verifying the state of collectible variables.
 *
 * @return True if all collectibles are set to 1, indicating the game is over. False otherwise.
 */
bool(gameOver)() {
  if (collectible1 == 1 && collectible2 == 1 && collectible3 == 1 && collectible4 == 1 && collectible5 == 1 && collectible6 == 1 && collectible7 == 1 && collectible8 == 1 && collectible9 == 1 && collectible10 == 1 && collectible11 == 1 && collectible12 == 1 && collectible13 == 1 && collectible14 == 1 && collectible15 == 1 && collectible16 == 1) {
    return true;
  }
  return false;
}

/**
 * @brief Retrieves the number of bits per pixel.
 *
 * @return The number of bits per pixel.
 */
uint8_t(getBitsPerPixel)() {
  return bitsPerPixel;
}

/**
 * @brief Retrieves the number of bytes per pixel.
 *
 * @return The number of bytes per pixel.
 */
uint8_t(getBytesPerPixel)() {
  return bytesPerPixel;
}

/**
 * @brief Retrieves the Y resolution.
 *
 * @return The Y resolution.
 */
uint16_t(getYResolution)() {
  return yResolution;
}

/**
 * @brief Retrieves the X resolution.
 *
 * @return The X resolution.
 */
uint16_t(getXResolution)() {
  return xResolution;
}

/**
 * @brief Draws the character ':' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_colon)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 41, 103, 45, 120) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'g' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_g)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 80, 153, 98, 170) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'h' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_h)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 121, 153, 136, 170) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'o' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_o)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 160, 178, 178, 195) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 's' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_s)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 80, 203, 95, 220) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 't' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_t)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 120, 202, 134, 220) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'c' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_c)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 160, 128, 177, 145) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'e' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_e)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 1, 153, 15, 170) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character 'r' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_r)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 41, 203, 57, 220) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character '0' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_0)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 120, 53, 133, 70) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character '1' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_1)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 162, 53, 170, 70) != 0)
    return 1;
  return 0;
}

/**
 * @brief Draws the character '2' at the specified position.
 *
 * @param x The X position.
 * @param y The Y position.
 * @return 1 if successful, 0 otherwise.
 */
int(vg_draw_2)(uint16_t x, uint16_t y) {
  if (vg_draw_part_of_sprite(get_chars_sprite(), x, y, 200, 53, 213, 70) != 0)
    return 1;
  return 0;
}
