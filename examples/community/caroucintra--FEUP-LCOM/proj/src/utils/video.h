
#define VBE_OK 0x00
#define BIOS_SERVICE 0x10
#define VBE_FUNCTION 0x4F
#define GET_VBE_CTRL_INFO 0x00
#define GET_VBE_MODE 0x01
#define SET_VBE_MODE 0x02
#define INDEXED_COLOR 0x105
#define DIRECT_COLOR_MODE  0x14C 

int(set_graphics_mode)(uint16_t mode);

int(map_VRAM)(uint16_t mode);

int(init_vg)(uint16_t mode);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

void change_buffer();

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (draw_rectangle_matrix)(uint16_t mode, uint16_t no_rectangles, uint32_t first, uint8_t step);

int (draw_pixmap)(uint8_t *map, xpm_image_t img, uint16_t x, uint16_t y);

int (reset_terminal)();

unsigned get_hres();

unsigned get_vres();

unsigned get_bitsperpixel();

unsigned get_bytesperpixel();
