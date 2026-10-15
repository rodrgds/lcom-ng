#ifndef _LCOM_ANIMATION_H_
#define _LCOM_ANIMATION_H_

#include "../../data_structures/queue/queue.h"
#include "../sprite/sprite.h"
#include <lcom/lcf.h>

/**
 * @defgroup animation Animation
 * @ingroup models
 * @{
 * 
 * @brief Animation module for handling sprite animations.
 */

/**
 * @struct animation_t
 * @brief Structure to represent an animation.
 */
typedef struct {
  queue_t *frames;     /**< @brief Queue of sprite frames. */
  queue_t *rotate;     /**< @brief Queue of rotation flags for each frame. */
  queue_t *x;          /**< @brief Queue of x coordinates for each frame. */
  queue_t *y;          /**< @brief Queue of y coordinates for each frame. */
  uint32_t frame_index; /**< @brief Current frame index. */
  void (*on_end)(void *); /**< @brief Callback function to be called when the animation ends. */
} animation_t;

/**
 * @brief Creates an animation.
 * 
 * @details Allocates memory for a new animation and initializes its frame queues.
 * 
 * @param frame_amount The number of frames in the animation.
 * @param on_end The callback function to be called when the animation ends.
 * @return Pointer to the created animation, or NULL if allocation fails.
 */
animation_t *animation_create(uint32_t frame_amount, void (*on_end)(void *));

/**
 * @brief Adds a frame to the animation.
 * 
 * @details Adds a new frame with the specified sprite, coordinates, and rotation flag to the animation.
 * 
 * @param animation Pointer to the animation.
 * @param sprite Pointer to the sprite to be added.
 * @param x The x coordinate of the frame.
 * @param y The y coordinate of the frame.
 * @param rotate The rotation flag for the frame.
 * @return 0 on success, 1 on failure.
 */
int animation_add_frame(animation_t *animation, sprite_t *sprite, uint32_t x, uint32_t y, bool rotate);


/**
 * @brief Draws the current frame of the animation.
 * 
 * @details Draws the current frame of the animation at its specified coordinates.
 * 
 * @param animation Pointer to the animation.
 * @return 0 on success, 1 on failure.
 */
int animation_draw(animation_t *animation);

/**
 * @brief Runs the animation.
 * 
 * @details Advances the animation by one frame. If the animation reaches its end, the on_end callback is called.
 * 
 * @param animation Double pointer to the animation.
 * @param ptr Pointer to be passed to the on_end callback function.
 */
void animation_run(animation_t **animation, void *ptr);

/**
 * @brief Checks if the animation is running.
 * 
 * @param animation Pointer to the animation.
 * @return 1 if the animation is running, 0 otherwise.
 */
int  animation_running(animation_t *animation);

/**
 * @brief Destroys the animation.
 * 
 * @details Frees the memory allocated for the animation and its frame queues.
 * @param animation Pointer to the animation to be destroyed.
 */
void animation_destroy(animation_t *animation);

/** @} */

#endif /* _LCOM_EV_ANIMATION_H_ */
