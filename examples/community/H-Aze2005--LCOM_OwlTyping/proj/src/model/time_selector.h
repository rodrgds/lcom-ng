/**
 * @file time_selector.h
 * @brief Time selector model and related operations.
 *
 * Declares the [`TimeSelector`](time_selector.h) structure and functions for creating,
 * destroying, drawing, and managing the state of a time selector widget.
 */

#ifndef _TIME_SELECTOR_H_
#define _TIME_SELECTOR_H_

#include "images/time_picker/30s.xpm"
#include "images/time_picker/60s.xpm"
#include "images/time_picker/120s.xpm"
#include "devices/video.h"

/**
 * @enum SelectorState
 * @brief Represents the available time selection options.
 *
 * - S30: 30 seconds selected.
 * - S60: 60 seconds selected.
 * - S120: 120 seconds selected.
 */
typedef enum {
    S30,
    S60,
    S120
} SelectorState;

/**
 * @struct TimeSelector
 * @brief Represents the state and position of the time selector widget.
 *
 * @var TimeSelector::x
 * X coordinate of the selector.
 * @var TimeSelector::y
 * Y coordinate of the selector.
 * @var TimeSelector::state
 * Current selected time option.
 */
typedef struct {
    int16_t x;
    int16_t y;
    SelectorState state;
} TimeSelector;

/**
 * @brief Constructs a new TimeSelector at the specified position.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @return Pointer to the newly created TimeSelector, or NULL on failure.
 */
TimeSelector *construct_TimeSelector(int16_t x, int16_t y);

/**
 * @brief Frees the memory allocated for the TimeSelector.
 * @param timeSelector Pointer to the TimeSelector to destroy.
 */
void destroy_TimeSelector(TimeSelector *timeSelector);

/**
 * @brief Draws the TimeSelector on the screen.
 * @param timeSelector Pointer to the TimeSelector to draw.
 */
void draw_TimeSelector(TimeSelector *timeSelector);

/**
 * @brief Sets the state of the TimeSelector.
 * @param timeSelector Pointer to the TimeSelector.
 * @param state New state to set.
 */
void set_time_selector_state(TimeSelector *timeSelector, SelectorState state);

/**
 * @brief Gets the currently selected time in seconds.
 * @param timeSelector Pointer to the TimeSelector.
 * @return Selected time in seconds (e.g., 30, 60, or 120).
 */
int get_selected_time(TimeSelector *timeSelector);

#endif
