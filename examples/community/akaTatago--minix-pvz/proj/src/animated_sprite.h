#include <lcom/lcf.h>

#define MAX_FRAMES_PER_SPRITE 16


/**
 * @brief Struct used to store animated sprites
 */
struct animated_sprite
{  
    uint8_t frames; /**< Total number of frames in the animation. */
    uint8_t current_frame;

    uint8_t ticks_per_frame;
    uint16_t tick_counter;

    xpm_image_t imgs[MAX_FRAMES_PER_SPRITE]; 
    uint8_t * pixmaps[MAX_FRAMES_PER_SPRITE];
};

/**
 * @brief Takes in an array of xpm sprites and initializes the animated_sprite struct with those sprites
 * 
 * @param sprite The animated_sprite to be initialized
 * @param num_frames How many sprited will be passed
 * @param num_frames How many times the animated sprite needs to be "ticked" to advance to the next frame
 * @param xpm_maps An array of char * representing each pixmap
 * @param type The xpm image type
 * @return int 0 if successful, 1 otherwise
 */
int init_animated_sprite(struct animated_sprite *sprite, uint8_t num_frames, uint8_t ticks_per_frame, char **xpm_maps[], enum xpm_image_type type);

/**
 * @brief Get the animated sprite's current image object
 * 
 * @param sprite The animated sprite
 * @return xpm_image_t The current xpm_image_t object based on current_frame
 */
xpm_image_t get_animated_sprite_image(const struct animated_sprite* sprite);

/**
 * @brief Get the animated sprite's current pixmap
 * 
 * @param sprite The animated sprite
 * @return xpm_image_t The current pixmap based on current_frame
 */
uint8_t* get_animated_sprite_pixmap(const struct animated_sprite* sprite);

/**
 * @brief Ticks the animated sprite's animation
 * 
 * This increments the animated sprite's tick counter by one
 * After "ticks_per_frame" ticks, the animation advances to the next frame
 * 
 * @param sprite The animated sprite
 */
void tick_animated_sprite(struct animated_sprite* sprite);
