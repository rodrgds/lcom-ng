#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>

/**
 * @brief Get the RGB info from a color in direct mode
 */
typedef struct rgb_direct {
  struct rgb {
    /**
     * @brief Size of the mask in bits
     */
    uint8_t mask_size;
    /**
     * @brief Position of the mask in the color
     */
    uint8_t field_position;
  } red, green, blue;
} rgb_direct_t;

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;
static uint8_t *double_buffer;

static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint8_t bytes_per_pixel;
static uint32_t frame_size;

static rgb_direct_t rgb_direct;

/**
 * @brief Set the graphics mode
 * 
 * @param mode Graphics mode to set
 * @return 0 on success, 1 otherwise
 */
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Map VRAM to the process address space
 * 
 * @param mode Graphics mode
 * @return 0 on success, 1 otherwise
 */
int (map_linear_frame_buffer)(uint16_t mode);

/**
 * @brief Exit the graphics mode
 * 
 * @return 0 on success, 1 otherwise
 */
int (exit_graphics_mode)();

/**
 * @brief Move the double buffer to the video memory
 * 
 * @return 0 on success, 1 otherwise
 */
int (move_buffer_to_video)();

/**
 * @brief Get the horizontal resolution
 * 
 * @return Horizontal resolution
 */
uint16_t (get_h_res)();

/**
 * @brief Get the vertical resolution
 * 
 * @return Vertical resolution
 */
uint16_t (get_v_res)();

/**
 * @brief Get the number of bits per pixel
 * 
 * @return Number of bits per pixel
 */
uint8_t (get_bits_per_pixel)();

/**
 * @brief Check if the graphics mode is direct
 * 
 * @return true if the graphics mode is direct
 */
bool (is_direct_mode)();

/**
 * @brief Get the RGB info from a color in direct mode
 * 
 * @return RGB info
 */
rgb_direct_t (get_rgb_direct)();

// implemented vg_draw_hline() in graphics.c
// implemented vg_draw_rectangle() in graphics.c

/**
 * @brief Set the color of a pixel
 */
int (set_pixel_color)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Normalize the red component of a color in direct mode
 * 
 * @param color Color to normalize
 * @return Normalized red component
 */
uint32_t (R)(uint32_t color);

/**
 * @brief Normalize the green component of a color in direct mode
 * 
 * @param color Color to normalize
 * @return Normalized green component
 */
uint32_t (G)(uint32_t color);

/**
 * @brief Normalize the blue component of a color in direct mode
 * 
 * @param color Color to normalize
 * @return Normalized blue component
 */
uint32_t (B)(uint32_t color);

/**
 * @brief Draw a sprite on the screen
 * 
 * @param img Sprite to draw
 * @param sprite Sprite data
 * @param x X coordinate of the sprite
 * @param y Y coordinate of the sprite
 * @return 0 on success, 1 otherwise
 */
int (draw_xpm)(xpm_image_t img, uint8_t* sprite, uint16_t x, uint16_t y);

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

#endif // _LCOM_GRAPHICS_H_
