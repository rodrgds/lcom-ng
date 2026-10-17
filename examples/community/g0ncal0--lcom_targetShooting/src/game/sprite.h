#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../devices/graphics/graphics.h"
#include "../resources/font/font.xpm"
#include "../resources/font/gameover.xpm"
#include "../resources/font/gameoverEsc.xpm"
#include "../resources/font/multiplayer.xpm"
#include "../resources/font/numbers.xpm"
#include "../resources/font/quit.xpm"
#include "../resources/font/score.xpm"
#include "../resources/font/singleplayer.xpm"
#include "../resources/font/lost.xpm"
#include "../resources/font/win.xpm"
#include "../resources/font/wait.xpm"
#include "../resources/font/title.xpm"
#include "../resources/sprites/aim.xpm"
#include "../resources/sprites/clock.xpm"
#include "../resources/sprites/dayDesert.xpm"
#include "../resources/sprites/dynamite.xpm"
#include "../resources/sprites/explosion.xpm"
#include "../resources/sprites/fall.xpm"
#include "../resources/sprites/nightDesert.xpm"
#include "../resources/sprites/target.xpm"

#define TRANSPARENT 0xFF000000

/**
 * @brief Structure representing a sprite.
 */
typedef struct Sprite {
  uint16_t height; /** The height of the sprite. */
  uint16_t width;  /** The width of the sprite. */
  uint32_t *map;   /** The pixel map of the sprite. */
} Sprite;

Sprite *dayDesert;    /** Pointer to the day desert sprite. */
Sprite *nightDesert;  /** Pointer to the night desert sprite. */
Sprite *aim;          /** Pointer to the aim sprite. */
Sprite *target;       /** Pointer to the target sprite. */
Sprite *fall[3];      /** Array of pointers to the fall sprites. */
Sprite *dynamiteIcon; /** Pointer to the dynamite icon sprite. */
Sprite *explosion[5]; /** Array of pointers to the explosion sprites. */
Sprite *clockIcon;    /** Pointer to the clock icon sprite. */
Sprite *scoreSprite;  /** Pointer to the score sprite. */
Sprite *numbers[10];  /** Array of pointers to the number sprites. */
Sprite *singleplayer; /** Pointer to the singleplayer sprite. */
Sprite *multiplayer;  /** Pointer to the multiplayer sprite. */
Sprite *quit;         /** Pointer to the quit sprite. */
Sprite *lost;         /** Pointer to the lost sprite. */
Sprite *win;          /** Pointer to the win sprite. */
Sprite *dots;         /** Pointer to the dots sprite. */
Sprite *title;        /** Pointer to the title sprite. */
Sprite *waitMessage;  /** Pointer to the wait Message sprite. */
Sprite *GameOver;     /** Pointer to the Game Over sprite. */
Sprite *GameOverESC;  /** Pointer to the Game Over ESC sprite. */

/**
 * @brief Creates a sprite from an XPM map.
 * @param xpm The XPM map representing the sprite.
 * @return A pointer to the created sprite.
 */
Sprite *(create_sprite) (xpm_map_t xpm);

/**
 * @brief Draws a sprite on the screen.
 * @param sprite The sprite to be drawn.
 * @param x The x-coordinate of the sprite.
 * @param y The y-coordinate of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int(draw_sprite)(Sprite *sprite, int x, int y);

/**
 * @brief Loads the menu sprites.
 */
void(loadMenu)();

/**
 * @brief Loads the game over sprites.
 */
void(loadGameOver)();

/**
 * @brief Loads all the sprites.
 * @return 0 upon success, non-zero otherwise.
 */
int(load_sprites)();

/**
 * @brief Deletes all the loaded sprites.
 * @return 0 upon success, non-zero otherwise.
 */
int(delete_sprites)();

#endif
