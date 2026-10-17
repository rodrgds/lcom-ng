#include <lcom/lcf.h>
#include <math.h>
#include "video_card.h"
#include "video_card_macros.h"


vbe_mode_info_t vmi_p;
void *video_mem;
void *aux_buffer;
struct minix_mem_range mr;
unsigned int vram_base;
unsigned int vram_size;
uint8_t pixel_bytes;
static bool  flag = false;
static int collision = NO_COLLISION;

int (change_mode) (uint16_t mode){
    reg86_t reg;

    memset(&reg, 0, sizeof(reg)); /// Put everything at 0

    reg.bx = BIT(14) | mode;
    reg.al = 0x02;
    reg.ah = 0x4F;
    reg.intno = 0x10;
    if (sys_int86(&reg) != 0 ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return -1;
    }

    /// If func is supported
    if (reg.al != 0x4F){
        printf("Function not supported.\n");
        return -1;
    }

    /// Check if func completed successfully
    if (reg.ah != 0x00){
        printf("Function call failed");
        return -1;
    }
    return 0;
}

int (vg_start) (uint16_t mode){

    // Get the info about the mode
    memset(&vmi_p, 0, sizeof(vmi_p));
    if (vbe_get_mode_info(mode, &vmi_p) != 0) return -1;

    // Initialize the variables
    vram_base = vmi_p.PhysBasePtr;
    pixel_bytes = vmi_p.BitsPerPixel / 8;
    if (vmi_p.BitsPerPixel % 8 != 0) pixel_bytes++;
    vram_size = vmi_p.XResolution * vmi_p.YResolution * pixel_bytes;

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    int r;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return -1;
    }

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED){
        panic("couldn't map video memory");
        return -1;
    }
    memset(video_mem, 0, vram_size); // Clear the vram


    aux_buffer = malloc(vram_size);
    if (aux_buffer == NULL) {
        panic("couldn't allocate auxiliary buffer");
        return -1;
    }
    memset(aux_buffer, 0, vram_size);

    if (change_mode(mode) != 0) return -1;
    return 0;
}

void (ajust_color) (uint32_t* color){
    if (pixel_bytes == 1 || pixel_bytes == 3 || pixel_bytes == 4) return; // Nothing needs to be done

    // Only the 2 bytes per pixel modes left
    uint8_t rsvd, r, g, b;
    rsvd = (*color >> 24) & ((1 << vmi_p.RsvdMaskSize) - 1); // Extract reserved component and adjust
    r = (*color >> 16) & ((1 << vmi_p.RedMaskSize) - 1); // Extract red component and adjust
    g = (*color >> 8) & ((1 << vmi_p.GreenMaskSize) - 1); // Extract green component and adjust
    b = (*color) & ((1 << vmi_p.BlueMaskSize) - 1); // Extract blue component and adjust

    // Combine adjusted components into the final color value
    *color = (rsvd << vmi_p.RsvdFieldPosition) | (r << vmi_p.RedFieldPosition) | (g << vmi_p.GreenFieldPosition) | (b << vmi_p.BlueFieldPosition);
}

int (vg_draw_pixel) (uint16_t pos_x, uint16_t pos_y, uint32_t color){
    if (pos_x >= vmi_p.XResolution || pos_y >= vmi_p.YResolution) return 0;

    if (color == TRANSPARENT_COLOR) return 0;

    // Get the initial position to write
    uint8_t* ptr = (uint8_t*) aux_buffer; 
    ptr += (vmi_p.XResolution * pos_y * pixel_bytes + pos_x * pixel_bytes);

    // Ajust the color
    //ajust_color(&color);

    if(flag){
        if (*(uint32_t*)ptr == BLUE){
            collision = FINISH_LINE;
        }
    else if(*(uint32_t*)ptr == GREEN || *(uint32_t*)ptr == PURPLE){
            collision = CRASH;
        }
    }
    // write the color
    if (memcpy((void*)(ptr), (const void*) &color, pixel_bytes) == NULL) return -1;
    return 0;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (uint16_t i = 0; i < len; i++){
        if (vg_draw_pixel(x + i, y, color) != 0) return -1;
    }
    return 0;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for (uint16_t i = 0; i < height; i++){
        if (vg_draw_hline(x, y + i, width, color)) return -1;
    }
    return 0;
}

int (draw_matrix) (uint8_t no_rectangles, uint32_t first, uint8_t step){
    uint16_t x_size = vmi_p.XResolution / no_rectangles , y_size = vmi_p.YResolution / no_rectangles;
    uint32_t color = first;
    for (uint8_t x = 0; x < no_rectangles; x++){
        uint16_t pos_x = x * x_size;
        for (uint8_t y = 0; y < no_rectangles; y++){
            uint16_t pos_y = y * y_size;
            if (vg_draw_rectangle(pos_x, pos_y, x_size, y_size, color) != 0) return -1;

            if (pixel_bytes != 1){
                uint8_t r = (((uint8_t) first >> 16) + y * step) % (1 << vmi_p.RedMaskSize);
	            uint8_t g = (((uint8_t) first >> 8) + x * step) % (1 << vmi_p.GreenMaskSize);
	            uint8_t b = ((uint8_t) first + (y + x) * step) % (1 << vmi_p.BlueMaskSize);
                color = (r << 16) | (g << 8) | (b);
            }
            else{
                color = (first + (x * no_rectangles + y) * step) % (1 << vmi_p.BitsPerPixel);
            }
        }
        // Draw the black stripe on the right
        uint16_t x1 = no_rectangles * x_size, y = x * y_size;
        if (vg_draw_rectangle(x1, y, vmi_p.XResolution - x1, y_size, 0x000000) != 0) return -1;
    }
    // Draw the black stripe on the bottom
    uint16_t y = no_rectangles * y_size, x = 0;
    if (vg_draw_rectangle(x, y, vmi_p.XResolution, vmi_p.YResolution - y, 0x000000) != 0) return -1;
    return 0;
}

int (clear_xpm) (sprite* xpm){
    //uint8_t color = (uint8_t) xpm_transparency_color(info->type);
    uint32_t color = 0;
    for (uint16_t x = 0; x < xpm->info.width; x++){
        for (uint16_t y = 0; y < xpm->info.height; y++){
            if (vg_draw_pixel(xpm->x + x, xpm->y + y, color) != 0) return -1;
        }
    }
    return 0;
}

int (show_xpm) (sprite* xpm){
    if (xpm->angle != 0){
        rotate(xpm);
        return 0;
    }
    uint8_t* ptr = (uint8_t *) xpm->info.bytes;
    for (uint16_t x = 0; x < xpm->info.width; x++){
        for (uint16_t y = 0; y < xpm->info.height; y++){
            // just needs to get the 8 bits stored in the pixmap, because it is using index mode (1 byte per pixel)
            uint32_t color = * (uint32_t*) (ptr + (y * xpm->info.width + x) * pixel_bytes);
            if (vg_draw_pixel(xpm->x + x, xpm->y + y, color) != 0) return -1;
        }
    }
    return 0;
}

int (show_xpm1) (xpm_image_t *info, uint16_t pos_x, uint16_t pos_y){
    for (uint16_t x = 0; x < info->width; x++){
        for (uint16_t y = 0; y < info->height; y++){
            // just needs to get the 8 bits stored in the pixmap, because it is using index mode (1 byte per pixel)
            uint32_t color = * (uint32_t*) (info->bytes + (y * info->width + x) * pixel_bytes);
            if (vg_draw_pixel(pos_x + x, pos_y + y, color) != 0) return -1;
        }
    }
    return 0;
}



int (show_xpm_at_position) (sprite* xpm, uint16_t posx, uint16_t posy){
    if(collision) collision = NO_COLLISION;
    flag = true;

    if (xpm->angle != 0){
        rotate_at_position(xpm, posx, posy);
        flag = false;
        return 0;
    }

    for (uint16_t x = 0; x < xpm->info.width; x++){
        for (uint16_t y = 0; y < xpm->info.height; y++){
            // just needs to get the 8 bits stored in the pixmap, because it is using index mode (1 byte per pixel)
            uint32_t color = * (uint32_t*) (xpm->info.bytes + (y * xpm->info.width + x) * pixel_bytes);
            if (vg_draw_pixel(posx + x, posy + y, color) != 0) return -1;
        }
    }

    flag = false;
    return 0;
}

void (rotate_at_position) (sprite* xpm, uint16_t posx, uint16_t posy){
    // Center of the image
    uint16_t cx = xpm->info.width / 2;
    uint16_t cy = xpm->info.height / 2;
    
    double radians = M_PI * xpm->angle / 180.0;
    uint8_t* ptr = (uint8_t *) xpm->info.bytes;
    for (uint16_t x = 0; x < xpm->info.width; x++) {
        for (uint16_t y = 0; y < xpm->info.height; y++) {
            int16_t dx = x - cx;
            int16_t dy = y - cy;
            
            // New coordinates
            int16_t new_x = dx * cos(radians) - dy * sin(radians) + cx;
            int16_t new_y = dx * sin(radians) + dy * cos(radians) + cy;
            
            vg_draw_pixel(new_x + posx, new_y + posy, *(uint32_t*)(ptr + (y * xpm->info.width + x) * pixel_bytes));
        }
    }
}

void (rotate) (sprite* xpm){
    // Center of the image
    uint16_t cx = xpm->info.width / 2;
    uint16_t cy = xpm->info.height / 2;
    
    double radians = M_PI * xpm->angle / 180.0;
    uint8_t* ptr = (uint8_t *) xpm->info.bytes;
    for (uint16_t x = 0; x < xpm->info.width; x++) {
        for (uint16_t y = 0; y < xpm->info.height; y++) {
            int16_t dx = x - cx;
            int16_t dy = y - cy;
            
            // New coordinates
            int16_t new_x = dx * cos(radians) - dy * sin(radians) + cx;
            int16_t new_y = dx * sin(radians) + dy * cos(radians) + cy;
            
            vg_draw_pixel(new_x + xpm->x, new_y + xpm->y, *(uint32_t*)(ptr + (y * xpm->info.width + x) * pixel_bytes));
        }
    }
}

int (move_mouse_cursor) (mouse_sprite* cursor, struct packet* pp){
    /// Calculate next position
    int16_t next_x = (int16_t)cursor->x + (int16_t) pp->delta_x;
    int16_t next_y = (int16_t)cursor->y + (-1 * (int16_t) pp->delta_y); /// Y movement is inverted

    if (next_x < 0) next_x = 0;
    else if(next_x >= vmi_p.XResolution) next_x = vmi_p.XResolution - 1;

    if (next_y < 0) next_y = 0;
    else if((next_y) >= vmi_p.YResolution) next_y = vmi_p.YResolution - 1;

    cursor->x = next_x;
    cursor->y = next_y;
    return 0;
}

void (clean_screen) (){
    memset(aux_buffer, 0, vram_size);
}

void (copy_to_video_mem) (){
    memcpy(video_mem, aux_buffer, vram_size);
}

int (show_mouse_cursor) (mouse_sprite* cursor){
    for (uint16_t x = 0; x < cursor->info[cursor->index].width; x++){
        for (uint16_t y = 0; y < cursor->info[cursor->index].height; y++){
            // just needs to get the 8 bits stored in the pixmap, because it is using index mode (1 byte per pixel)
            uint32_t color = * (uint32_t*)(cursor->info[cursor->index].bytes + (y * cursor->info[cursor->index].width + x) * pixel_bytes);
            if (vg_draw_pixel(cursor->x + x, cursor->y + y, color) != 0) return -1;
        }
    }
    return 0;
}

int (clear_mouse_cursor) (mouse_sprite* cursor){
    uint32_t color = 0;
    for (uint16_t x = 0; x < cursor->info[cursor->index].width; x++){
        for (uint16_t y = 0; y < cursor->info[cursor->index].height; y++){
            if (vg_draw_pixel(cursor->x + x, cursor->y + y, color) != 0) return -1;
        }
    }
    return 0;
}

int (show_race) (sprite* race, uint16_t car_x_pos, uint16_t car_y_pos){
    uint16_t start_x = (car_x_pos > 200) ? car_x_pos - 200 : 0;
    uint16_t end_x = (car_x_pos + 200 >= race->info.width) ? race->info.width - 1 : car_x_pos + 200;

    uint16_t start_y = (car_y_pos > 200) ? car_y_pos - 200 : 0;
    uint16_t end_y = (car_y_pos + 200 >= race->info.height) ? race->info.height - 1 : car_y_pos + 200;
    uint8_t* ptr = (uint8_t*) race->info.bytes;
    for (uint16_t x = start_x; x <= end_x; x++){
        for (uint16_t y = start_y; y <= end_y; y++){
            // just needs to get the 8 bits stored in the pixmap, because it is using index mode (1 byte per pixel)
            uint32_t color = *(uint32_t*)(ptr + (y * race->info.width + x) * pixel_bytes);
            if (vg_draw_pixel(race->x + x - car_x_pos, race->y + y - car_y_pos, color) != 0) return -1;
        }
    }
    return 0;
}

int (car_collision(sprite* car)){
    return collision;
}

void (load_background)(sprite* background){
     memcpy(aux_buffer,background->info.bytes,background->info.width * background->info.height * pixel_bytes);
}

