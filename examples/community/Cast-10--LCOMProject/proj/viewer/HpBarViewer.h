#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "video.h"

/** @brief Height of the HP bar in pixels. */
#define HP_BAR_HEIGHT 6
/** @brief Gap in pixels between the entity's top edge and the bottom of the HP bar. */
#define HP_BAR_OFFSET 3

/**
 * @brief Colour palette used when drawing an HP bar.
 */
typedef struct {
    uint32_t high_color;   /**< Fill colour when HP is above 60% of maximum. */
    uint32_t mid_color;    /**< Fill colour when HP is between 25% and 60% of maximum. */
    uint32_t low_color;    /**< Fill colour when HP is below 25% of maximum. */
    uint32_t empty_color;  /**< Colour of the depleted portion of the bar. */
    uint32_t border_color; /**< Colour of the bar border. */
} HpBarViewer;

/**
 * @brief Allocates and initialises an HpBarViewer with default colours.
 * @return Pointer to the new HpBarViewer, or NULL on allocation failure.
 */
HpBarViewer *createHpBarViewer(void);

/**
 * @brief Frees an HpBarViewer previously created with createHpBarViewer().
 * @param viewer HpBarViewer to destroy.
 */
void destroyHpBarViewer(HpBarViewer *viewer);

/**
 * @brief Draws a coloured HP bar above the given position.
 * The bar is HP_BAR_HEIGHT pixels tall and positioned HP_BAR_OFFSET pixels above (x, y).
 * @param viewer HpBarViewer supplying the colour palette.
 * @param x Screen X coordinate of the bar's left edge.
 * @param y Screen Y coordinate of the entity's top edge (bar is drawn above this).
 * @param width Width of the bar in pixels.
 * @param current_hp Current HP value.
 * @param max_hp Maximum HP value.
 */
void hpBarViewerDraw(HpBarViewer *viewer, int x, int y, int width, int current_hp, int max_hp);
