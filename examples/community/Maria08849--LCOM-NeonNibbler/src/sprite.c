#include "sprite.h"

/** @defgroup Sprite_P
 * @{
 *
 * Functions related to the creation of sprites that are going to be used in the project
 */

Sprite *pacman;
Sprite *powerpacman;
Sprite *upacman;
Sprite *ghost;
Sprite *nightBackground;
Sprite *dayBackground;
Sprite *gameBackground;
Sprite *cDoor;
Sprite *nPlay;
Sprite *sPlay;
Sprite *Logo;
Sprite *collectible;
Sprite *powerup;
Sprite *chars;

extern xpm_row_t const pacman_xpm[];
extern xpm_row_t const powerpacman_xpm[];
extern xpm_row_t const upacman_xpm[];
extern xpm_row_t const ghost_xpm[];
extern xpm_row_t const nightbackground[];
extern xpm_row_t const daybackground[];
extern xpm_row_t const gamebackground[];
extern xpm_row_t const cdoor[];
extern xpm_row_t const nplay[];
extern xpm_row_t const splay[];
extern xpm_row_t const logo[];
extern xpm_row_t const collectible_xpm[];
extern xpm_row_t const powerup_xpm[];
extern xpm_row_t const characters[];

/**
 * @brief Destroys a sprite and frees the associated memory.
 *
 * @param sp The sprite to destroy.
 */
void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->colors)
    free(sp->colors);
  free(sp);
  sp = NULL;
}

/**
 * @brief Creates a sprite from the given XPM image.
 *
 * @param sprite The XPM image data.
 * @return A pointer to the created sprite, or NULL if creation fails.
 */
Sprite *create_sprite(xpm_map_t sprite) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;

  xpm_image_t img;
  sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
  sp->height = img.height;
  sp->width = img.width;

  if (sp->colors == NULL) {
    free(sp);
    return NULL;
  }
  return sp;
}

/**
 * @brief Creates all the sprites used in the game.
 * This function initializes global variables for each sprite.
 */
void(createsprites)() {
  pacman = create_sprite((xpm_map_t) pacman_xpm);
  upacman = create_sprite((xpm_map_t) upacman_xpm);
  powerpacman = create_sprite((xpm_map_t) powerpacman_xpm);
  ghost = create_sprite((xpm_map_t) ghost_xpm);
  nightBackground = create_sprite((xpm_map_t) nightbackground);
  dayBackground = create_sprite((xpm_map_t) daybackground);
  gameBackground = create_sprite((xpm_map_t) gamebackground);
  cDoor = create_sprite((xpm_map_t) cdoor);
  nPlay = create_sprite((xpm_map_t) nplay);
  sPlay = create_sprite((xpm_map_t) splay);
  Logo = create_sprite((xpm_map_t) logo);
  collectible = create_sprite((xpm_map_t) collectible_xpm);
  powerup = create_sprite((xpm_map_t) powerup_xpm);
  chars = create_sprite((xpm_map_t) characters);
}

/**
 * @brief Destroys all the sprites used in the game.
 * This function frees the memory allocated for each sprite.
 */
void(destroy_sprites)() {
  destroy_sprite(pacman);
  destroy_sprite(upacman);
  destroy_sprite(powerpacman);
  destroy_sprite(ghost);
  destroy_sprite(nightBackground);
  destroy_sprite(dayBackground);
  destroy_sprite(gameBackground);
  destroy_sprite(cDoor);
  destroy_sprite(nPlay);
  destroy_sprite(sPlay);
  destroy_sprite(Logo);
  destroy_sprite(collectible);
  destroy_sprite(powerup);
  destroy_sprite(chars);
}

/**
 * @brief Retrieves the pacman sprite.
 *
 * @return A pointer to the pacman sprite.
 */
Sprite *(get_pacman_sprite) () {
  return pacman;
}

/**
 * @brief Retrieves the upacman sprite.
 *
 * @return A pointer to the upacman sprite.
 */
Sprite *(get_upacman_sprite) () {
  return upacman;
}

/**
 * @brief Retrieves the powerpacman sprite.
 *
 * @return A pointer to the powerpacman sprite.
 */
Sprite *(get_powerpacman_sprite) () {
  return powerpacman;
}

/**
 * @brief Retrieves the ghost sprite.
 *
 * @return A pointer to the ghost sprite.
 */
Sprite *(get_ghost_sprite) () {
  return ghost;
}

/**
 * @brief Retrieves the night-themed menu background sprite.
 *
 * @return A pointer to the night-themed menu background sprite.
 */
Sprite *(get_night_background_sprite) () {
  return nightBackground;
}

/**
 * @brief Retrieves the day-themed menu background sprite.
 *
 * @return A pointer to the day-themed menu background sprite.
 */
Sprite *(get_day_background_sprite) () {
  return dayBackground;
}

/**
 * @brief Retrieves the game background sprite.
 *
 * @return A pointer to the game background sprite.
 */
Sprite *(get_game_background_sprite) () {
  return gameBackground;
}

/**
 * @brief Retrieves the closed door sprite.
 *
 * @return A pointer to the closed door sprite.
 */
Sprite *(get_cdoor_sprite) () {
  return cDoor;
}

/**
 * @brief Retrieves the non-selected play button sprite.
 *
 * @return A pointer to the non-selected play button sprite.
 */
Sprite *(get_nplay_sprite) () {
  return nPlay;
}

/**
 * @brief Retrieves the selected play button sprite.
 *
 * @return A pointer to the selected play button sprite.
 */
Sprite *(get_splay_sprite) () {
  return sPlay;
}

/**
 * @brief Retrieves the game logo sprite.
 *
 * @return A pointer to the game logo sprite.
 */
Sprite *(get_logo_sprite) () {
  return Logo;
}

/**
 * @brief Retrieves the collectible sprite.
 *
 * @return A pointer to the collectible sprite.
 */
Sprite *(get_collectible_sprite) () {
  return collectible;
}

/**
 * @brief Retrieves the power-up sprite.
 *
 * @return A pointer to the power-up sprite.
 */
Sprite *(get_powerup_sprite) () {
  return powerup;
}

/**
 * @brief Retrieves the characters sprite.
 *
 * @return A pointer to the characters sprite.
 */
Sprite *(get_chars_sprite) () {
  return chars;
}
