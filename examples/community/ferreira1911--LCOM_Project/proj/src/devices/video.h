#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>

/* Parâmetros Importantes reg86_t */

#define AH_TEXT_MODE    0x00            /**< @brief AH Text mode */
#define Al_TEXT_MODE    0x03            /**< @brief AL Text mode */
#define AX_TEXT_MODE    0x0003          /**< @brief AX Text mode */

#define AH_GRAPHIC_MODE 0x4F            /**< @brief AH Graphic mode */
#define AL_GRAPHIC_MODE 0x02            /**< @brief AL Graphic mode */
#define AX_GRAPHIC_MODE 0x4F02          /**< @brief AX Graphic mode */

#define INTNO 0x10                      /**< @brief BIOS video services */

#define LINEAR_FB BIT(14)               /**< @brief Linear Frame Buffer Mode bit */


int (vbe_set_mode)(uint16_t mode);
int (frame_buffer_init)(uint16_t mode);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (color_pixel)(uint8_t* pixel_ptr, uint32_t color);
int (vg_draw_xpm)(uint16_t x, uint16_t y, const xpm_image_t *img);
int (vg_clear_screen)();
void (swap_buffers)();

uint32_t (calculate_indexed_color)(uint8_t row, uint8_t col, uint32_t first, uint8_t step, uint8_t bits_per_pixel, uint8_t no_rectangles);
uint32_t (calculate_direct_color)(uint8_t row, uint8_t col, uint32_t first, uint8_t step, vbe_mode_info_t vmi);


#endif /* VIDEO_H */

