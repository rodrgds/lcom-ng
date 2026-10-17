#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "stdlib.h"
#include "smart_vector.h"
#include "Utilities.h"

/** @defgroup sprite sprite
 * @{
 * Defines necessary structs and methods related to sprites
 */

typedef int animationFrame;
defineSmartVector(animationFrame);

/**
 * @brief A struct that contains a static or animated sprite
 * */
typedef struct spr{
	int x,y; ///<current sprite position
	int xCenter,yCenter;///<sprite center coordinates
	void* ptr;///<pointer to sprite
	char isStatic;///<indicates if the sprite is static or not (0 if not static)
} Sprite;

typedef Sprite* spritePtr;

/**
 * @brief A struct that contains data necessary for a static sprite
 * */
typedef struct {
	//int x,y; ///<current sprite position
  unsigned int index;///<pixmap index on pixmaps array
} StaticSprite;

/**
 * @brief A struct that contains data necessary for an animated sprite
 * */
typedef struct {
  //int x,y; ///<current sprite position
  unsigned int currentIndex;///<index of the current animationIndex to use
  animationFrame_SV* animation;///<animation used pixmaps indexes (should define the array somewhere, using malloc here creates too much repeated data unnecessarily)

} AnimatedSprite;

/**
 * @brief creates a static sprite
 */
spritePtr createStaticSprite(int x, int y,int xC,int yC,int pixmapIndex);
/**
 * @brief creates an animated sprite
 */
spritePtr createAnimatedSprite(int x, int y,int xC,int yC,int beginIndex,animationFrame_SV* animation );

/**@brief Draws given sprite directly on screen */
void draw_sprite_onScreen(Sprite *sp);

/**@brief Draws given sprite on buffer */
void draw_sprite_onBuffer(Sprite *sp);

void initSpritesDrawList();
/**
 * @brief destroys a sprite and free mem
 */
void deleteSprite(Sprite *spPtr);
/** * @brief remove from the list of sprites to be draw
 * does not free memory used by sprite
 */
void removeSpriteFromDrawList(Sprite *spPtr);
/** * @brief adds a sprite to drawList */
void addSprite2DrawList(Sprite *spPtr);
void drawSprites();

/**
 * @brief updates current animation frame being displayed
 */
void animatedSprite_playAnimation(Sprite *as);

void freeDrawList();

#endif

/** @} end of sprite */
