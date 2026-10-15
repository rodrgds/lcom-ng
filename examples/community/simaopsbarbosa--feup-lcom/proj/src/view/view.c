#include <lcom/lcf.h>
#include "view.h"

uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;

extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern char last_key_pressed[16];
extern int cursor_x, cursor_y;
extern char last_mouse_button[32];

int set_frame_buffers(uint16_t mode) {
    if (get_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    
    secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    drawing_frame_buffer = secondary_frame_buffer;
    
    return 0;
}

void swap_buffers() {
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
}


// -------- draw --------

struct Sprite background;
struct Sprite logo;
struct Sprite button;
struct Sprite button_hovered;
struct Sprite cursor;

void view_init() {
    background.data = (uint32_t *)xpm_load((xpm_row_t *)background_xpm, XPM_8_8_8_8, &background.img);
    logo.data = (uint32_t *)xpm_load((xpm_row_t *)logo_xpm, XPM_8_8_8_8, &logo.img);
    button.data = (uint32_t *)xpm_load((xpm_row_t *)button_xpm, XPM_8_8_8_8, &button.img);
    button_hovered.data = (uint32_t *)xpm_load((xpm_row_t *)button_hovered_xpm, XPM_8_8_8_8, &button_hovered.img);
    cursor.data = (uint32_t *)xpm_load((xpm_row_t *)cursor_xpm, XPM_8_8_8_8, &cursor.img);
}

void destroy_xpms() {
    free(background.data);
    free(logo.data);
    free(button.data);
    free(button_hovered.data);
    free(cursor.data);
}
