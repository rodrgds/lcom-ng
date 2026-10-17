#include "v_card.h"

static void *video_mem;       // Process (virtual) address to which VRAM is mapped
static uint8_t *back_buffer;  // Back buffer in RAM
static vbe_mode_info_t vmi_p; // VBE mode info
static unsigned int vram_size;         // Total VRAM size in bytes
static unsigned int bytes_per_pixel;   // Cached for use in all draw calls


int(change_mode)(uint16_t mode) {
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); //clean struct values

    reg86.intno = 0x10;          //video interrupt
    reg86.ax = 0x4F02;           //set VBE mode
    
    reg86.bx = (mode | BIT(14)) & ~BIT(15); //ensure linear frame buffer && avoid cleaning screen between mode changes

    if (sys_int86(&reg86) != OK) {   //send instruction
        printf("\tvg_init(): sys_int86() failed \n");
        return 1;
    }

    return 0;
}

void *(vg_init)(uint16_t mode) {
    //get vbe mode information
    if (vbe_get_mode_info(mode, &vmi_p) != 0) {
        printf("vbe_get_mode_info failed\n");
        return NULL;
    }

    //map physical memory to virtual memory
    int r;
    struct minix_mem_range mr;
    unsigned int vram_base = vmi_p.PhysBasePtr;
    bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
    vram_size = vmi_p.XResolution * vmi_p.YResolution * bytes_per_pixel;

    //allow memory mapping
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    }

    //map memory
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
    }

    // Allocate the back buffer in regular RAM
    back_buffer = (uint8_t *) malloc(vram_size);
    if (back_buffer == NULL) {
        printf("vg_init: failed to allocate back buffer\n");
        return NULL;
    }
    memset(back_buffer, 0, vram_size);

    //change vbe mode
    if (change_mode(mode) != 0) {
        free(back_buffer);
        back_buffer = NULL;
        return NULL;
    }

    return video_mem;
}

// int (vg_exit)() {
//     if (back_buffer != NULL) {
//         free(back_buffer);
//         back_buffer = NULL;
//     }
//     return 0;
// }

// Copy the back buffer to VRAM — call once at the end of every frame
void (vg_flip)() {
    memcpy(video_mem, back_buffer, vram_size);
}

// Clear the back buffer to a given color (typically 0x000000)
void (vg_clear)(uint32_t color) {
    if (bytes_per_pixel == 1) {
        memset(back_buffer, (uint8_t)color, vram_size);
    } else {
        // Fill pixel by pixel
        unsigned int num_pixels = vmi_p.XResolution * vmi_p.YResolution;
        for (unsigned int i = 0; i < num_pixels; i++) {
            memcpy(back_buffer + i * bytes_per_pixel, &color, bytes_per_pixel);
        }
    }
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    //check out of bounds
    if (x >= vmi_p.XResolution || y >= vmi_p.YResolution) {
        return 0; 
    }

    unsigned int index = (vmi_p.XResolution * y + x) * bytes_per_pixel;

    //moving the exact number of bytes needed
    memcpy(back_buffer + index, &color, bytes_per_pixel);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
        // Clamp length to screen width
    if (x >= vmi_p.XResolution || y >= vmi_p.YResolution) return 0;
    if (x + len > vmi_p.XResolution) len = vmi_p.XResolution - x;

    unsigned int index = (vmi_p.XResolution * y + x) * bytes_per_pixel;

    for (uint16_t i = 0; i < len; i++) {
        memcpy(back_buffer + index + i * bytes_per_pixel, &color, bytes_per_pixel); // Write the whole line in one pass: faster than calling vg_draw_pixel per pixel
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    if (x >= vmi_p.XResolution || y >= vmi_p.YResolution) return 0;
    if (y + height > vmi_p.YResolution) height = vmi_p.YResolution - y;

    for (uint16_t i = 0; i < height; i++) {
        vg_draw_hline(x, y + i, width, color);
    }
    return 0;
}

int (vg_draw_xpm)(xpm_image_t img, uint8_t *pixmap, uint16_t x, uint16_t y) {
    
    uint32_t *colors = (uint32_t *)pixmap;

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            uint32_t color = colors[i * img.width + j];
            // For XPM_INDEXED mode, each pixel in the array represents a 1-byte color index
            if (color != xpm_transparency_color(img.type)) {
                if (vg_draw_pixel(x + j, y + i, color) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

