#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "sprite/AnimSprite.h"
#include "HpBarViewer.h"

#include "../model/champion/Champion.h"

/**
 * @brief Renderer for a champion, managing its animation clips and HP bar.
 */
typedef struct {
    AnimSprite **animations;  /**< Array of NUM_ANIMS animation clips. */
    int numAnimations;        /**< Total number of animation clips in the array. */
    int currentAnimation;     /**< Index of the clip currently playing. */
    HpBarViewer *hpBar;       /**< HP bar renderer. */
} ChampionViewer;

/**
 * @brief Allocates a ChampionViewer and associates the provided animation clips.
 * @param animations Array of AnimSprite pointers (ownership is not transferred).
 * @param numAnimations Number of elements in animations.
 * @return Pointer to the new ChampionViewer, or NULL on failure.
 */
ChampionViewer *createChampionViewer(AnimSprite **animations, int numAnimations);

/**
 * @brief Frees a ChampionViewer and its HP bar (does not free the animation clips).
 * @param viewer ChampionViewer to destroy.
 */
void destroyChampionViewer(ChampionViewer *viewer);

/**
 * @brief Selects the correct animation clip based on the champion's state and direction,
 * advances the frame counter, and returns whether the animation looped this tick.
 * @param viewer ChampionViewer to update.
 * @param champion Champion whose state drives the animation selection.
 * @return true if the current animation completed a full loop this tick.
 */
bool championViewerUpdate(ChampionViewer *viewer, Champion *champion);

/**
 * @brief Draws the champion's current animation frame and HP bar at the champion's position.
 * @param viewer ChampionViewer supplying sprites and the HP bar.
 * @param champion Champion providing the world position and HP values.
 * @return 0 on success, non-zero on error.
 */
int championDraw(ChampionViewer *viewer, Champion *champion);
