#ifndef GPU_H
#define GPU_H

#include <lcom/lcf.h>
#include <stdint.h>

/** @name VBE BIOS Function Codes */
/** @{ */
#define VBE_CONTROLLER_INFO 0x4F00   /**< Request VBE controller information. */
#define VBE_MODE_INFO 0x4F01         /**< Request VBE mode information. */
#define VBE_MODE_SET 0x4F02          /**< Set a VBE graphics mode. */
#define VBE_DISPLAY_START_SET 0x4F07 /**< Set a vram region to display. */
#define BIOS_VIDEO_MODE_SET 0x0003   /**< Reset to Minix default text mode. */
#define BIOS_VIDEOCARD_SERV 0x10     /**< BIOS video services. */
/** @} */

/** @name Supported VBE Modes */
/** @{ */
#define VBE_MODE1 0x105 /**< 1024x768, 256 colors (8 bpp, indexed color). */
#define VBE_MODE2 0x110 /**< 640x480, 15 bpp (direct color, RGB 5:5:5). */
#define VBE_MODE3 0x115 /**< 800x600, 24 bpp (direct color, RGB 8:8:8). */
#define VBE_MODE4 0x11A /**< 1280x1024, 16 bpp (direct color, RGB 5:6:5). */
#define VBE_MODE5 0x14C /**< 1152x864, 32 bpp (direct color, RGB 8:8:8). */
#define VBE_MODE6 0x107 /**< 1280x1024, 256 colors (8 bpp, indexed color). */
/** @} */

/** @name VBE Return Status Codes */
/** @{ */
#define CALL_FAIL 0x01            /**< Function call failed. */
#define UNSUPPORTED_FUNCTION 0x02 /**< Function not supported by hardware. */
#define INVALID_FUNCTION 0x03     /**< Invalid function call. */
/** @} */

#define LINEAR_FRAMEBUFFER BIT(14) /**< Enable linear framebuffer mode. */
#define VERTICAL_RETRACE 0x0080    /**< Set display start during vertical retrace. */
#define VBE 0x10                   /**< BIOS video interrupt number. */
#define DIRECT_MODE 6              /**< Direct color memory model identifier. */
#define BACKGROUND_COLOR 0         /**< Default background color. */

extern uint32_t interrupt_counter; /**< Timer interrupt counter. */

/**
 * @brief Sets the VBE graphics mode.
 *
 * @param mode VBE mode number to set (e.g., VBE_MODE1).
 * @return 0 on success, non-zero on failure.
 */
int(set_graphic_mode)(uint16_t mode);

/**
 * @brief Initializes the frame buffers.
 *
 * @param mode VBE mode to be used.
 * @return 0 on success, non-zero on failure.
 */
int(set_frame_buffers)(uint16_t mode);

/**
 * @brief Returns a pointer to the video memory location of a specific pixel.
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param frame_buffer Pointer to the frame buffer.
 * @return Pointer to the memory location for pixel (x, y).
 */
uint8_t *(get_position) (uint16_t x, uint16_t y, uint8_t *frame_buffer);

/**
 * @brief Draws a single pixel of a specified color at (x, y).
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a horizontal line of a specified color.
 *
 * @param x X-coordinate of the starting point.
 * @param y Y-coordinate of the line.
 * @param len Length of the line.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a filled rectangle of a specified color.
 *
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Calculates the color for a rectangle in direct color mode.
 *
 * @param i Row index.
 * @param j Column index.
 * @param step Color step value.
 * @param first Base color.
 * @return Computed color value.
 */
uint32_t(direct_mode)(uint16_t i, uint16_t j, uint8_t step, uint32_t first);

/**
 * @brief Calculates the color for a rectangle in indexed color mode.
 *
 * @param no_rectangles Total rectangles per row/column.
 * @param i Row index.
 * @param j Column index.
 * @param step Color increment step.
 * @param first Initial color value.
 * @return Computed color.
 */
uint32_t(indexed_mode)(uint8_t no_rectangles, uint16_t i, uint16_t j, uint8_t step, uint32_t first);

/**
 * @brief Draws an XPM image at the specified coordinates.
 *
 * @param xpm XPM map to be drawn.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *frame_buffer);

/**
 * @brief Draws a preloaded XPM image at the specified coordinates.
 *
 * @param xpm_data Pointer to pixel data.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param width Image width.
 * @param height Image height.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_loaded_xpm)(uint8_t *xpm_data, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *frame_buffer);

/**
 * @brief Clears the given frame buffer with the specified color.
 *
 * @param frame_buffer Pointer to the frame buffer.
 * @param color Color used to clear (format depends on color depth).
 * @return 0 on success, non-zero on failure.
 */
int(clear_frame_buffer)(uint8_t *frame_buffer, uint16_t color);

/**
 * @brief Sets the video memory display start to point to a new buffer (page flipping).
 */
void(set_display_start)();

/**
 * @brief Swaps the main and secondary frame buffers.
 */
void(vga_flip_pages)();

#endif /* GPU_H */
