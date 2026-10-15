#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>
#include <stdint.h>

// Initializes video mode and maps VRAM. Returns pointer to video memory, or NULL on error. 
void* (vg_init)(uint16_t mode);

// Draws a single pixel at (x, y) with the given color. 
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

// Draws a horizontal line starting at (x, y) with the given length and color. 
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

// Draws a filled rectangle with the top-left corner at (x, y). 
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

// Draws a pixmap loaded from an XPM with the top-left corner at (x, y).
// NOTE: This parses the XPM on every call. For animated sprites, use vg_draw_pixmap instead.
int (vg_draw_xpm)(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);

// --- Double Buffering ---

// Copies the entire back buffer to VRAM in one instant operation (call once per frame)
void (vg_flush)();

// Frees the back buffer allocated by vg_init().
void (vg_cleanup)();

// Copies a full-screen pre-loaded image to the back buffer (use for backgrounds)
void (vg_copy_to_buffer)(uint8_t *src);

// Draws a pre-loaded raw pixmap to the back buffer, skipping transparent pixels (XPM_8_8_8)
int (vg_draw_pixmap)(uint8_t *pixmap, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

// Getters
uint16_t (get_h_res)();
uint16_t (get_v_res)();
uint8_t  (get_bits_per_pixel)();

#endif /* VIDEO_H */
