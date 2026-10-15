#include "video.h"
#include <string.h>
#include <stdlib.h>

static char *video_mem;               // Virtual address to which VRAM is mapped; char is used because it occupies 1 byte and facilitates pointer arithmetic 
static char *back_buffer;             // Secondary buffer in system RAM for double buffering
static vbe_mode_info_t vmi;           // VBE mode info 
static unsigned bytes_per_pixel;      // Number of bytes per pixel 
static unsigned int frame_size;       // Total bytes per frame

uint16_t (get_h_res)()         { return vmi.XResolution; }
uint16_t (get_v_res)()         { return vmi.YResolution; }
uint8_t  (get_bits_per_pixel)() { return vmi.BitsPerPixel; }

void* (vg_init)(uint16_t mode) {
    int r;
    struct minix_mem_range mr;

    memset(&vmi, 0, sizeof(vmi));
    if (vbe_get_mode_info(mode, &vmi) != 0) {
        return NULL;
    }

    bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8; // Make sure to round up
    frame_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

    unsigned int vram_base = vmi.PhysBasePtr; // Physical address of VRAM

    // Map VRAM
    mr.mr_base  = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + frame_size;

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return NULL;
    }

    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, frame_size);
    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }

    // Allocate the secondary buffer (back buffer) in system RAM
    back_buffer = (char *) malloc(frame_size);
    if (back_buffer == NULL) {
        panic("couldn't allocate back buffer");
        return NULL;
    }
    memset(back_buffer, 0, frame_size);

    // Set video mode via BIOS call
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.intno = 0x10;
    reg.ah = 0x4F;
    reg.al = 0x02;
    reg.bx = mode | BIT(14); // enables BIT(14)

    if (sys_int86(&reg) != 0) {
        vg_cleanup();
        return NULL;
    }

    return video_mem;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= vmi.XResolution || y >= vmi.YResolution)
        return 1;

    // Address of the pixel in the BACK BUFFER (system RAM), not VRAM
    char *pixel = back_buffer + (y * vmi.XResolution + x) * bytes_per_pixel;

    // Paint the pixel
    memcpy(pixel, &color, bytes_per_pixel);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color) != 0)
            return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t row = 0; row < height; row++) {
        if (vg_draw_hline(x, y + row, width, color) != 0)
            return 1;
    }
    return 0;
}

int (vg_draw_xpm)(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y) {
    xpm_image_t img;
    // Load the XPM image into a pixmap
    uint8_t *pixmap = xpm_load(xpm, type, &img);
    if (pixmap == NULL)
        return 1;
 
    uint8_t bpp = bytes_per_pixel;
 
    // Iterate through each row and column of the image
    for (uint16_t row = 0; row < img.height; row++) {
        for (uint16_t col = 0; col < img.width; col++) {
            uint32_t color = 0;
            // Get the color of the current pixel from the pixmap
            memcpy(&color, pixmap + (row * img.width + col) * bpp, bpp);
            
            // Draw the pixel on the screen ONLY if it's not the transparent color
            if (color != xpm_transparency_color(type)) {
                if (vg_draw_pixel(x + col, y + row, color) != 0) {
                    free(pixmap);
                    return 1;
                }
            }
        }
    }
    free(pixmap);
    return 0;
}

// --- Double Buffering Functions ---

void (vg_flush)() {
    // Writes from back_buffer (system RAM) to video_mem (VRAM) in one instant operation
    memcpy(video_mem, back_buffer, frame_size);
}

void (vg_cleanup)() {
    free(back_buffer);
    back_buffer = NULL;
}

void (vg_copy_to_buffer)(uint8_t *src) {
    memcpy(back_buffer, src, frame_size);
}

int (vg_draw_pixmap)(uint8_t *pixmap, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
    uint32_t transparent = xpm_transparency_color(XPM_8_8_8);

    for (uint16_t row = 0; row < height; row++) {
        uint16_t dy = y + row;
        if (dy >= vmi.YResolution) break;

        for (uint16_t col = 0; col < width; col++) {
            uint16_t dx = x + col;
            if (dx >= vmi.XResolution) break;

            uint32_t color = 0;
            memcpy(&color, pixmap + (row * width + col) * bytes_per_pixel, bytes_per_pixel);

            // Only draw non-transparent pixels
            if (color != transparent) {
                memcpy(back_buffer + (dy * vmi.XResolution + dx) * bytes_per_pixel, &color, bytes_per_pixel);
            }
        }
    }
    return 0;
}
