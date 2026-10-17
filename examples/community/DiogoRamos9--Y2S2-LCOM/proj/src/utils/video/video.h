#include <lcom/lcf.h>

int (vg_set_mode)(uint16_t mode);

int (config_frame_buffer)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (transform_color)(uint32_t color, uint32_t *new_color);

int transform_color_little_endian(uint32_t color, uint32_t *new_color);

uint32_t (directed_mode) (uint32_t red, uint32_t green, uint32_t blue);

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles);

uint32_t (Red)(int j, uint8_t step, uint32_t first);

uint32_t (Green)(int i, uint8_t step, uint32_t first);

uint32_t (Blue)(int j, int i, uint8_t step, uint32_t first);

uint32_t (R)(int first);

uint32_t (G)(int first);

uint32_t (B)(int first);

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm);

int (move_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate);

uint8_t *get_video_mem();

void allocate_buffers();

void deallocate_buffers();

void swap_buffers();
