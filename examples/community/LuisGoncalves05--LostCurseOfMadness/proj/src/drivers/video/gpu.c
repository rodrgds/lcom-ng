#include <lcom/lcf.h>
#include <machine/int86.h>
#include <math.h>

#include "gpu.h"

vbe_mode_info_t vg_mode_info;    /**< Video mode information */
uint8_t bytes_per_pixel;         /**< Bytes per pixel */
uint16_t x_res;                  /**< X resolution of the screen */
uint16_t y_res;                  /**< Y resolution of the screen */
uint32_t frame_size;             /**< Size of the framebuffer */
bool frame_start = true;         /**< Indicates if the current frame is the start frame */
uint8_t *main_frame_buffer;      /**< Pointer to the main framebuffer */
uint8_t *secondary_frame_buffer; /**< Pointer to the secondary framebuffer */

int(set_graphic_mode)(uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86_t));
    reg86.intno = BIOS_VIDEOCARD_SERV;
    reg86.ax = VBE_MODE_SET;
    reg86.bx = mode | LINEAR_FRAMEBUFFER;
    sys_int86(&reg86);
    return 0;
}

int(set_frame_buffers)(uint16_t mode) {
    memset(&vg_mode_info, 0, sizeof(vg_mode_info));
    vbe_get_mode_info(mode, &vg_mode_info); // get mode info

    x_res = vg_mode_info.XResolution;                      // get x resolution
    y_res = vg_mode_info.YResolution;                      // get y resolution
    bytes_per_pixel = (vg_mode_info.BitsPerPixel + 7) / 8; // calculate bytes per pixel

    frame_size = vg_mode_info.XResolution * vg_mode_info.YResolution * bytes_per_pixel; // calculate size of framebuffer

    struct minix_mem_range physic_addresses;
    physic_addresses.mr_base = vg_mode_info.PhysBasePtr;                   // início físico do buffer
    physic_addresses.mr_limit = physic_addresses.mr_base + 2 * frame_size; // fim físico do buffer

    sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses); // add memory range to process

    main_frame_buffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, 2 * frame_size); // map memory range to process
    secondary_frame_buffer = main_frame_buffer + frame_size;

    return 0;
}

int(clear_frame_buffer)(uint8_t *frame_buffer, uint16_t color) {
    return memset(frame_buffer, color, frame_size) == NULL;
}

void(set_display_start)() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86_t));
    reg86.intno = BIOS_VIDEOCARD_SERV;
    reg86.ax = VBE_DISPLAY_START_SET;
    reg86.bx = VERTICAL_RETRACE;
    reg86.cx = 0;
    reg86.dx = frame_start ? 0 : y_res;
    sys_int86(&reg86);
}

void(vga_flip_pages)() {
    frame_start = !frame_start;
    set_display_start();
    uint8_t *tmp = main_frame_buffer;
    main_frame_buffer = secondary_frame_buffer;
    secondary_frame_buffer = tmp;
}

uint8_t *(get_position) (uint16_t x, uint16_t y, uint8_t *frame_buffer) {
    return frame_buffer + (x + x_res * y) * bytes_per_pixel;
}

int(vga_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer) {
    return vga_draw_hline(x, y, 1, color, frame_buffer);
}

int(vga_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer) {
    if (x >= x_res || y >= y_res) {
        printf("vga_draw_hline: invalid coordinates/dimensions, x:%u y:%u len:%u.\n", x, y, len);
        return 1;
    }

    uint8_t *address = get_position(x, y, frame_buffer);
    for (uint16_t i = 0; i < len && x + i < x_res; i++, address += bytes_per_pixel) {
        memcpy(address, &color, bytes_per_pixel);
    }

    return 0;
}

int(vga_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer) {
    if (x >= x_res || y >= y_res) {
        printf("vg_draw_rectangle: invalid coordinates/dimensions, x:%u y:%u width:%u height:%u.\n", x, y, width, height);
        return 1;
    }

    int retv = 0;
    for (uint16_t i = 0; i < height && y + i < y_res; i++)
        retv |= vga_draw_hline(x, y + i, width, color, frame_buffer);

    if (retv) {
        printf("vg_draw_rectangle: vg_draw_hline failed.\n");
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Extracts a specific color component from a 32-bit color value.
 *
 * @param color Original color.
 * @param field_position Bit position of the field.
 * @param mask_size Size in bits of the mask.
 * @return Extracted component value.
 */
static inline uint8_t(get_component)(uint32_t color, uint8_t field_position, uint8_t mask_size) {
    return (color >> field_position) & ((1 << mask_size) - 1);
}

uint32_t(direct_mode)(uint16_t i, uint16_t j, uint8_t step, uint32_t first) {
    uint8_t rfp = vg_mode_info.RedFieldPosition;
    uint8_t gfp = vg_mode_info.GreenFieldPosition;
    uint8_t bfp = vg_mode_info.BlueFieldPosition;

    uint8_t rms = vg_mode_info.RedMaskSize;
    uint8_t gms = vg_mode_info.GreenMaskSize;
    uint8_t bms = vg_mode_info.BlueMaskSize;

    uint8_t r = (get_component(first, rfp, rms) + j * step) % (1 << rms);
    uint8_t g = (get_component(first, gfp, gms) + i * step) % (1 << gms);
    uint8_t b = (get_component(first, bfp, bms) + (j + i) * step) % (1 << bms);

    return (r << rfp) | (g << gfp) | (b << bfp);
}

uint32_t(indexed_mode)(uint8_t no_rectangles, uint16_t i, uint16_t j, uint8_t step, uint32_t first) {
    return (first + (i * no_rectangles + j) * step) % (1 << vg_mode_info.BitsPerPixel);
}

int(vga_draw_loaded_xpm)(uint8_t *xpm_data, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *frame_buffer) {
    uint8_t *ptr = get_position(x, y, frame_buffer);
    if (ptr == NULL) {
        fprintf(stderr, "vga_draw_loaded_xpm: get_position failed.\n");
        return 1;
    }

    if (x >= x_res || y >= y_res) {
        printf("vga_draw_loaded_xpm: invalid xpm position, x:%hu, y:%hu.\n", x, y);
        return 1;
    }

    uint16_t line_size = width * bytes_per_pixel;
    uint16_t usable_line_size = line_size;
    if (x + width > x_res)
        usable_line_size = (x_res - x) * bytes_per_pixel;

    uint8_t *data = xpm_data;
    for (int h = 0; h < height && y + h < y_res; h++, ptr += x_res * bytes_per_pixel, data += line_size)
        memcpy(ptr, data, usable_line_size);

    return 0;
}

int(vga_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *frame_buffer) {
    xpm_image_t img;
    uint8_t *data = xpm_load(xpm, XPM_INDEXED, &img);

    if (vga_draw_loaded_xpm(data, x, y, img.width, img.height, frame_buffer))
        return 1;

    return 0;
}
