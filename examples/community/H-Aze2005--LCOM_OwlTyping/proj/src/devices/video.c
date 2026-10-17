#include "video.h"

static void *video_mem;
static void *double_buffer;
static void *background_buffer;
static void *intermediate_buffer;
static vbe_mode_info_t vmi; // Video Mode Info

unsigned int x_resolution;
unsigned int y_resolution;
unsigned int bytes_pixel;
unsigned int vram_size;

// Start Graphics Mode
int video_init(uint16_t mode) {
    // Get info about mode
    vbe_get_mode_info(mode, &vmi);

    x_resolution = vmi.XResolution;
    y_resolution = vmi.YResolution;
    bytes_pixel = (vmi.BitsPerPixel + 7) / 8;

    struct minix_mem_range mr;
    unsigned int vram_base = vmi.PhysBasePtr;  /* VRAM's physical address */
    vram_size = x_resolution * y_resolution * bytes_pixel; /* VRAM's size */
    int r;

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED) panic("couldn't map video memory");

    double_buffer = malloc(vram_size);
    background_buffer = malloc(vram_size);
    intermediate_buffer = malloc(vram_size);

    // Set video mode
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.ax = 0x4F02;
    reg86.bx = (1<<14) | mode;
    reg86.intno = 0x10;

    if (sys_int86(&reg86) != OK) return 1;

    return 0;
}

int fill_pixel(uint16_t x, uint16_t y, uint32_t color, void* buffer) {
    // Check if pixel position is valid
    if (x > x_resolution || y > y_resolution) return 1;

    uint8_t* pixel_addr = (uint8_t*)buffer + ((y * x_resolution + x) * bytes_pixel);

    // Paint pixel
    memcpy(pixel_addr, &color, bytes_pixel);

    return 0;
}

// Paint the entire screen the same color
int fill_screen(uint32_t color) {
    for (uint16_t x = 0; x < x_resolution; x++) {
        for (uint16_t y = 0; y < y_resolution; y++) {
            if (fill_pixel(x, y, color, double_buffer) != 0) return 1;
        }
    }
    return 0;
}

// Draw xpm image on screen
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, void *buffer) {

    xpm_image_t img;
  
    uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    if (img.width + x > x_resolution || img.height + y > y_resolution) return 1; // Check if image fits screen

    for (int row = 0; row < img.height; row++) {
        for (int col = 0; col < img.width; col++) {
            // Skip transparent pixels
            if ((*colors >> 24) != 0xFF) {
                fill_pixel(col + x, row + y, *colors, buffer);
            }
            colors++; // Move to next color
        }
    }

    return 0;
}

// Draw an image from a xpm
int draw_image(xpm_map_t xpm, uint16_t x, uint16_t y) {
    return draw_xpm(xpm, x, y, double_buffer);
}

int draw_image_to_both(xpm_map_t xpm, uint16_t x, uint16_t y) {
    if(draw_xpm(xpm, x, y, background_buffer))return 1;
    if(draw_xpm(xpm, x, y, double_buffer))return 1;
    return 0;
}

// Load new screen background
int load_background_xpm(xpm_map_t xpm) {
    return draw_xpm(xpm, 0, 0, background_buffer);
}

// Draw in the screen the content of the buffer
void draw_buffer() {
    memcpy(video_mem, double_buffer, vram_size);
}

// Draw background in double buffer
void draw_background() {
    memcpy(double_buffer, background_buffer, vram_size);
}

// Reset the pixels with background content
int redraw_background(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    if (width + x > x_resolution || height + y > y_resolution) return 1; // Check if pixels are valid

    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            uint8_t* pixel_ptr = (uint8_t*) background_buffer + (row * x_resolution + col) * bytes_pixel;
            uint32_t color = pixel_ptr[0] | (pixel_ptr[1] << 8) | (pixel_ptr[2] << 16);
            if (fill_pixel(col, row, color, double_buffer) != 0) return 1;
        }
    }
    return 0;
}

// Reset the pixels with background content in both buffers
int draw_rectangle_background(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    if (width + x > x_resolution || height + y > y_resolution) return 1; // Check if pixels are valid

    uint32_t color = 0x381D2A;

    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            if (fill_pixel(col, row, color, double_buffer) != 0) return 1;
            if (fill_pixel(col, row, color, background_buffer) != 0) return 1;
        }
    }
    return 0;
}
