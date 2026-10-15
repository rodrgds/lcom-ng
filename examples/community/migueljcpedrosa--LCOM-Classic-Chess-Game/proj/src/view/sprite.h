/**
 * @file sprite
 * @brief This file has the implementation of the functions related to the sprite.
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../drivers/video_card/gpu.h"

/**
 * @brief Struct representing a sprite.
 * 
 * Contains the address of the sprite image and its respective xpm_image_t structure.
 */
typedef struct {
  uint8_t* addr;
  xpm_image_t img;
} Sprite;

extern Sprite* black_queen;
extern Sprite* white_queen;
extern Sprite* black_king;
extern Sprite* white_king;
extern Sprite* black_rook;
extern Sprite* white_rook;
extern Sprite* black_bishop;
extern Sprite* white_bishop;
extern Sprite* black_knight;
extern Sprite* white_knight;
extern Sprite* black_pawn;
extern Sprite* white_pawn;

extern Sprite* mouse;

extern Sprite* numbers;
extern Sprite* numbers_small;
extern Sprite* slash;

extern Sprite* alphabet;
extern Sprite* alphabet_small;

extern Sprite* exitButton;
extern Sprite* playButton;
extern Sprite* chessBackground;

/**
 * @brief Function to load all sprites.
 * 
 * This function allocates memory for all the sprites and loads them from xpm files.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (load_sprites)();

/**
 * @brief Function to load all sprites.
 * 
 * This function allocates memory for all the sprites and loads them from xpm files.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
void (destroy_sprites)(); 

/**
 * @brief Function to load a sprite from an xpm file.
 * 
 * @param sprite A pointer to the sprite to be loaded.
 * @param xpm The xpm file from which the sprite should be loaded.
 * 
 * This function allocates memory for a sprite, loads the sprite from the specified xpm file,
 * and assigns its address and xpm_image_t structure to the sprite.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (load_sprite)(Sprite* sprite, xpm_map_t xpm);

#endif
