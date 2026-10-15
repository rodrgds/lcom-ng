#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Maps the physical VRAM into the process virtual address space.
 * Maps two pages to support page-flipping.
 * Must always be called before set_graphic_mode() to pass the LCF checks.
 * @param mode VBE mode whose info (base address, resolution) is used.
 * @return 0 on success, 1 otherwise.
 */
int (map_vram)(uint16_t mode);

/**
 * @brief Invokes the BIOS to switch the graphics card to the given VBE mode.
 * @param mode VBE mode to activate (e.g. 0x115 = 800x600 direct color).
 * @return 0 on success, 1 otherwise.
 */
int (set_graphic_mode)(uint16_t mode);

/**
 * @brief Returns to Minix text mode. Must be called before the program exits.
 * @return 0 on success, 1 otherwise.
 */
int (video_exit)(void);

/**
 * @brief Flips the displayed page using VBE Function 07h.
 * Makes the draw buffer visible and switches the draw target to the other page.
 * Call once per frame after all drawing is done.
 * @return 0 on success, 1 otherwise.
 */
int (flip_page)(void);

/**
 * @brief Returns a pointer to the current draw buffer (the hidden page).
 * All drawing must target this buffer; never write directly to the other page.
 */
uint8_t *(video_get_draw_buffer)(void);

/** Screen dimension and format getters. */
uint16_t (get_h_res)(void);
uint16_t (get_v_res)(void);
unsigned (get_bytes_per_pixel)(void);
unsigned (get_bits_per_pixel)(void);
unsigned (get_bytes_per_scanline)(void);

/**
 * @brief Draws a single pixel into the draw buffer.
 * @return 0 on success, 1 if the pixel is out of bounds.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line of pixels into the draw buffer.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle into the draw buffer.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws an XPM image into the draw buffer.
 * Kept for lab5 compatibility. In the game, load XPMs once at startup via assets_init() instead.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif /* _LCOM_GRAPHICS_H_ */
