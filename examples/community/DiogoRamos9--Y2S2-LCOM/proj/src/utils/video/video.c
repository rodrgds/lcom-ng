#include <lcom/lcf.h>

#include "VBE_macros.h"
#include "video.h"
#include "../timer/timer.h"
#include "../keyboardMouse/keyboard.h"
#include "../keyboardMouse/i8042.h"

extern uint8_t scancode;

vbe_mode_info_t mode_info;

uint8_t *first_buffer;
uint8_t *double_buffer;

int (vg_set_mode)(uint16_t mode) {
    reg86_t r86;
        
    memset(&r86, 0, sizeof(r86));
    
    r86.intno = VIDEO_CARD;
    r86.ah = VBE_AH;
    r86.al = GRAPHICS_MODE;
    r86.bx = mode | LINEAR_MODE;

    if (sys_int86(&r86) != OK) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int (config_frame_buffer)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;
    
    int r, vram_size = mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel;
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    uint8_t *video_mem;
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) {
        panic("couldn’t map video memory");
        return 1;
    }
    first_buffer = video_mem;

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (color == 0) return 0;
    if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;

    // Calculate bytes per pixel (divide by 8) and the index in bytes
    int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    int index =  bytesPerPixel * (y * mode_info.XResolution + x);

    // Transform color to the correct format
    uint32_t new_color;
    if (transform_color_little_endian(color, &new_color) != 0) return 1;

    // Copy to memory
    memcpy(&double_buffer[index], &color, bytesPerPixel);
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color) != 0) return 1;
    }

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color) != 0) return 1;
    }

    return 0;
}

int (transform_color)(uint32_t color, uint32_t *new_color) {
    if (new_color == NULL) return 1;
    uint8_t red, green, blue;
    switch (mode_info.BitsPerPixel) {
        // 1024x768
        case 8: {
            *new_color = color & 0x0000000F;
            return 0;
        }
        // 640x480
        case 15: {
            red = (color & 0x00007C00) >> 8;
            green = (color & 0x000003E0) >> 4;
            blue = (color & 0x0000001F);
            *new_color = red | green | blue;
            return 0;
        }
        // 800x600
        case 24: {
            red = (color & 0x00FF0000) >> 16;
            green = (color & 0x0000FF00) >> 8;
            blue = (color & 0x000000FF);
            *new_color = red | green | blue;
            return 0;
        }
        // 1280x1024
        case 16 : {
            red = (color & 0x0000F800) >> 8;
            green = (color & 0x000007E0) >> 3;
            blue = (color & 0x0000001F);
            *new_color = red | green | blue;
            return 0;
        }
        // 1152x864
        case 32: {
            red = (color & 0x00FF0000) >> 16;
            green = (color & 0x0000FF00) >> 8;
            blue = (color & 0x000000FF);
            *new_color = red | green | blue;
            return 0;
        }
        default:
            return 1;
    }
    return 1;
}

int transform_color_little_endian(uint32_t color, uint32_t* new_color) {
    if (new_color == NULL) return 1;
    uint8_t red, green, blue;
    red = color & 0x000000FF;
    green = (color & 0x0000FF00) >> 8;
    blue = (color & 0x00FF0000) >> 16;
    *new_color = red | green | blue;
    return 0;
}

uint32_t (index_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
    return (((first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel)) != 0) ? 1 : 0;
}

uint32_t (directed_mode) (uint32_t red, uint32_t green, uint32_t blue) {
    if (((red << mode_info.RedFieldPosition) | (blue << mode_info.BlueFieldPosition) | (green << mode_info.GreenFieldPosition)) != 0) return 1;
    return 0;
}

uint32_t (Red)(int j, uint8_t step, uint32_t first) {
    return (((step * j + R(first)) % (BIT(mode_info.RedMaskSize))) != 0) ? 1 : 0;
}

uint32_t (Green)(int i, uint8_t step, uint32_t first) {
    return (((i * step + G(first)) % (BIT(mode_info.GreenMaskSize))) != 0) ? 1 : 0;
}

uint32_t (Blue)(int j, int i, uint8_t step, uint32_t first) {
    return (((step * (j + i) + B(first)) % (BIT(mode_info.BlueMaskSize))) != 0) ? 1 : 0;
}

uint32_t (R)(int first) {
  return (((BIT(mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition)) != 0) ? 1 : 0;
}

uint32_t (G)(int first) {
  return (((BIT(mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition)) != 0) ? 1 : 0;
}

uint32_t (B)(int first) {
  return (((BIT(mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition)) != 0) ? 1 : 0;
}

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
    uint16_t width = mode_info.XResolution / no_rectangles;
    uint16_t height = mode_info.YResolution / no_rectangles;
    for (int i = 0; i < no_rectangles; i++) {
        for (int j = 0; j < no_rectangles; j++) {
            uint32_t color;
            switch (mode_info.BitsPerPixel) {
                case 15: case 16: case 24: case 32: {   // Direct mode
                    uint32_t red = Red(j, step, first);
                    uint32_t green = Green(i, step, first);
                    uint32_t blue = Blue(j, i, step, first);
                    color = directed_mode(red, green, blue);
                    break;
                }
                case 8: {   // Indexed mode
                    color = index_mode(j, i, step, first, no_rectangles);
                    break;
                }
                default:
                    return 1;
            }
            if(vg_draw_rectangle(j * width, i * height, width, height, color) != 0) return 1;
        }
    }
    
    return 0;
}

int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm) {
    // We need to fill the xpm_load function with its parameters: xpm_map_t map, enum xpm_image_type type, xpm_image_t *img
    xpm_image_t image;
    uint32_t *color_map = (uint32_t*) xpm_load(xpm, XPM_8_8_8_8, &image);
    int index = 0;
    if (color_map == NULL) return 1;
    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            if (vg_draw_pixel(i + x, j + y, color_map[index]) != 0) return 1;
            index++;
        }
    }

    return 0;
}

int (move_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    int ipc_status;
    message msg;
    uint8_t timer_irq_set, keyboard_irq_set;

    if (keyboard_subscribe_int(&timer_irq_set)) return 1;
    if (timer_subscribe_int(&keyboard_irq_set)) return 1;
    if (timer_set_frequency(0, fr_rate)) return 1;   

    bool isHorizontal = xi < xf;
    if (isHorizontal && yi != yf) return 1;
    else return 1;

    while ((xi < xf || yi < yf) && scancode != ESC_BREAK) {

        if(driver_receive(ANY, &msg, &ipc_status) != 0) return 1;

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                if (msg.m_notify.interrupts & timer_irq_set) {
                    if (draw_xpm(xi, yi, xpm) != 0) return 1;
                    if (isHorizontal) {
                        xi += speed;
                        if (xi > xf) xi = xf;
                    }
                    else {
                        yi += speed;
                        if (yi > yf) yi = yf;
                    }
                }
                
                if (msg.m_notify.interrupts & keyboard_irq_set) {
                    kbc_ih();
                }
            }
        }
    }

    if (timer_unsubscribe_int() != 0) return 1;
    if (keyboard_unsubscribe_int() != 0) return 1;
    
    return 0;
}

uint8_t *get_video_mem() {
    return first_buffer;
}

// Buffer matters

void allocate_buffers() {
    double_buffer = (uint8_t *) malloc(mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel / 8));
    memset(double_buffer, 0, mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel / 8));
}

void deallocate_buffers() {
    memset(first_buffer, 0, mode_info.YResolution * mode_info.XResolution *(( mode_info.BitsPerPixel+7) / 8));
    memset(double_buffer, 0, mode_info.YResolution * mode_info.XResolution *(( mode_info.BitsPerPixel+7) / 8));
    free(first_buffer);
    free(double_buffer);
}

void swap_buffers() {
    memcpy(first_buffer, double_buffer, mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel / 8));
}
