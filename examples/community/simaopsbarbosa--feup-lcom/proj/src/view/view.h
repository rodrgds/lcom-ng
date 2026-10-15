/**
 * @file view.h
 * @brief Interface for rendering and drawing game views and menus.
 */

#ifndef _LCOM_VIEW_H_
#define _LCOM_VIEW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "utils/sprite.h"
#include "controllers/video/video.h"
#include "model/state.h"
#include "model/modes.h"
#include "xpm/background.h"
#include "xpm/buttons.h"
#include "xpm/logo.h"
#include "xpm/cursor.h"

/// Pointer to the main frame buffer.
extern uint8_t *main_frame_buffer;

/// Pointer to the secondary frame buffer.
extern uint8_t *secondary_frame_buffer;

/// Pointer to the buffer currently used for drawing.
extern uint8_t *drawing_frame_buffer;

/// Total size of the frame buffer.
extern uint32_t frame_buffer_size;

/// Sprite of the background image.
extern struct Sprite background;

/// Sprite of the logo image.
extern struct Sprite logo;

/// Sprite of a normal button.
extern struct Sprite button;

/// Sprite of a hovered button.
extern struct Sprite button_hovered;

/// Sprite of the cursor.
extern struct Sprite cursor;

/**
 * @brief Initializes the frame buffers for double buffering.
 * 
 * Allocates memory for the secondary buffer and sets the appropriate buffer pointers.
 * 
 * @param mode Video mode to be set for the frame buffer.
 * @return 0 on success and 1 on failure.
 */
int set_frame_buffers(uint16_t mode);

/**
 * @brief Swaps the drawing buffer to the screen by copying it to the main frame buffer.
 */
void swap_buffers();

/**
 * @brief Initializes all the sprites by loading their XPM.
 */
void view_init();

/**
 * @brief Frees the memory used by all sprites.
 */
void destroy_xpms();

#endif /* _LCOM_VIEW_H_ */
