#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "../devices/video/graphics.h"
#include "../img/menu/menu.xpm"
#include "../img/menu/cursor.xpm"
#include "../img/menu/cursorClick.xpm"
#include "../img/menu/StartNotPressed.xpm"
#include "../img/menu/StartPressed.xpm"
#include "../img/menu/multiplayer_not.xpm"
#include "../img/menu/multiplayer.xpm"
#include "../img/menu/wait.xpm"
#include "../img/menu/ExitNotPressed.xpm"
#include "../img/menu/ExitPressed.xpm"
#include "../img/menu/Loss.xpm"
#include "../img/menu/Tie.xpm"
#include "../img/menu/Win.xpm"
#include "../img/game/edit_icon.xpm"
#include "../img/menu/table.xpm"
#include "../img/menu/table_pressed.xpm"
#include "../img/menu/red_cross.xpm"
#include "../img/menu/refrashe.xpm"

#include "../img/game/Hit.xpm"
#include "../img/game/HitPressed.xpm"
#include "../img/game/Stand.xpm"
#include "../img/game/StandPressed.xpm"
#include "../img/game/Back.xpm"

#include "../img/game/6_of_clubs.xpm"
#include "../img/game/6_of_diamonds.xpm"
#include "../img/game/6_of_hearts.xpm"
#include "../img/game/6_of_spades.xpm"

#include "../img/game/7_of_clubs.xpm"
#include "../img/game/7_of_diamonds.xpm"
#include "../img/game/7_of_hearts.xpm"
#include "../img/game/7_of_spades.xpm"

#include "../img/game/8_of_clubs.xpm"
#include "../img/game/8_of_diamonds.xpm"
#include "../img/game/8_of_hearts.xpm"
#include "../img/game/8_of_spades.xpm"

#include "../img/game/9_of_clubs.xpm"
#include "../img/game/9_of_diamonds.xpm"
#include "../img/game/9_of_hearts.xpm"
#include "../img/game/9_of_spades.xpm"

#include "../img/game/10_of_clubs.xpm"
#include "../img/game/10_of_diamonds.xpm"
#include "../img/game/10_of_hearts.xpm"
#include "../img/game/10_of_spades.xpm"

#include "../img/game/jack_of_clubs.xpm"
#include "../img/game/jack_of_diamonds.xpm"
#include "../img/game/jack_of_hearts.xpm"
#include "../img/game/jack_of_spades.xpm"

#include "../img/game/queen_of_clubs.xpm"
#include "../img/game/queen_of_diamonds.xpm"
#include "../img/game/queen_of_hearts.xpm"
#include "../img/game/queen_of_spades.xpm"

#include "../img/game/king_of_clubs.xpm"
#include "../img/game/king_of_diamonds.xpm"
#include "../img/game/king_of_hearts.xpm"
#include "../img/game/king_of_spades.xpm"

#include "../img/game/yellow5coin.xpm"
#include "../img/game/yellow10coin.xpm"
#include "../img/game/yellow25coin.xpm"
#include "../img/game/yellow100coin.xpm"
#include "../img/game/coin5.xpm"
#include "../img/game/coin10.xpm"
#include "../img/game/coin25.xpm"
#include "../img/game/coin100.xpm"
#include "../img/game/open_hand.xpm"
#include "../img/game/closed_hand.xpm"

#include "../img/game/_0.xpm"
#include "../img/game/_1.xpm"
#include "../img/game/_2.xpm"
#include "../img/game/_3.xpm"
#include "../img/game/_4.xpm"
#include "../img/game/_5.xpm"
#include "../img/game/_6.xpm"
#include "../img/game/_7.xpm"
#include "../img/game/_8.xpm"
#include "../img/game/_9.xpm"
#include "../img/game/doubleDot.xpm"

#include "../img/game/a.xpm"
#include "../img/game/b.xpm"
#include "../img/game/c.xpm"
#include "../img/game/d.xpm"
#include "../img/game/e.xpm"
#include "../img/game/f.xpm"
#include "../img/game/g.xpm"
#include "../img/game/h.xpm"
#include "../img/game/i.xpm"
#include "../img/game/j.xpm"
#include "../img/game/k.xpm"
#include "../img/game/l.xpm"
#include "../img/game/m.xpm"
#include "../img/game/n.xpm"
#include "../img/game/o.xpm"
#include "../img/game/p.xpm"
#include "../img/game/q.xpm"
#include "../img/game/r.xpm"
#include "../img/game/s.xpm"
#include "../img/game/t.xpm"
#include "../img/game/u.xpm"
#include "../img/game/v.xpm"
#include "../img/game/w.xpm"
#include "../img/game/x.xpm"
#include "../img/game/y.xpm"
#include "../img/game/z.xpm"


typedef struct {
  int width, height; 
  uint32_t *map; 
} Sprite;

Sprite *background;
Sprite *cursor;
Sprite *cursorClick;
Sprite *hand;
Sprite *handCloused;
Sprite *Exit;
Sprite *ExitPressed;
Sprite *Start;
Sprite *StartPressed;
Sprite *multiplayer_not;
Sprite *multiplayer;
Sprite *wait;
Sprite *Hit;
Sprite *HitPressed;
Sprite *Stand;
Sprite *StandPressed;
Sprite *Loss;
Sprite *Win;
Sprite *Tie;
Sprite *edit_icon;
Sprite *doubleDot;
Sprite *table;
Sprite *table_pressed;
Sprite *red_cross;
Sprite *refrashe;

Sprite *cards[33];
Sprite *coins[8];
Sprite *numbers[10];
Sprite *letters[27]; // include ' ' space 

/**
 @brief Creates a sprite from an XPM image.
This function creates a sprite by loading an XPM image into a
dynamically allocated Sprite structure. It sets the sprite's
height, width, and pixel data (map) according to the XPM image.
 @param sprite The XPM image map to be loaded as a sprite.
 @return A pointer to the created Sprite, or NULL on failure (memory allocation or XPM loading errors).
 @note The complexity of this function is O(1).
*/
Sprite *createSprite(xpm_map_t sprite);

/**
 @brief Destroys a sprite and frees the associated memory.
This function destroys a sprite by freeing its pixel data (map)
and the sprite structure itself. It sets the pointer to the sprite
to NULL after freeing the memory.
 @param sp A pointer to the Sprite to be destroyed.
 @note The complexity of this function is O(1).
*/
void destroy_sprite(Sprite *sp);

/**
 @brief Draws a sprite on the screen at a specified position.
This function draws a sprite on the screen at the given x and y
coordinates. It iterates through the sprite's pixel data and draws
each pixel on the screen, ignoring any pixel with the default color (DEFOULT_COLOR).
 @param sprite A pointer to the Sprite to be drawn.
 @param x The x-coordinate of the top-left corner of the sprite.
 @param y The y-coordinate of the top-left corner of the sprite.
 @return 0 on success, 1 on failure (error encountered while drawing a pixel).
 @note The complexity of this function is O(height * width), where height and width are the dimensions of the sprite.
*/
int drawSprite(Sprite *sp, int x, int y);

/**
 @brief Creates and initializes all Sprite objects needed for the game.
This function creates Sprite objects for menu elements, cards, coins, numbers, and letters.
It initializes each Sprite object by loading the respective xpm data.
*/
void createAllSprites();

/**
 @brief Destroys and frees all Sprite objects created for the game.
This function destroys and frees all Sprite objects created for menu elements,
cards, coins, numbers, and letters.
*/
void destroyAllSprites();

#endif
