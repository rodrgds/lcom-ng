#ifndef _BGS_H_
#define _BGS_H_

#include "src/Model/Position.h"
#include "src/include.h"

/**
 * @file BGS.h
 * @brief Declares the BG enum used to represent different background states.
 *
 * Defines background states for visual transitions
 * day, night, sunrise, and sunset.
 */

/**
 * @enum BG
 * @brief Represents the current background state of the scene.
 *
 * Used to change the appearance of the background depending on the time of day.
 */


typedef enum{
    BG_DAY,
    BG_NIGHT,
    BG_SET,
    BG_RISE
}BG;

#endif
