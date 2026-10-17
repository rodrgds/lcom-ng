#include <lcom/lcf.h>

#include "video.h"

uint8_t *front_buffer, *back_buffer;
video_mode_info mode_info;

int (video_get_mode_info)(uint16_t mode){
    vbe_mode_info_t vbe_info;

    int flag = vbe_get_mode_info(mode, &vbe_info);
    if (flag) return flag;

    // parse the information
    mode_info.mode = mode;
    mode_info.memory_model = vbe_info.MemoryModel;

    mode_info.bits_per_pixel = (phys_bytes) vbe_info.BitsPerPixel;
    mode_info.bytes_per_pixel = (mode_info.bits_per_pixel + 7) / 8; // rounding by excess

    mode_info.x_res = vbe_info.XResolution;
    mode_info.y_res = vbe_info.YResolution;

    mode_info.physical_base = vbe_info.PhysBasePtr;
    mode_info.physical_size = mode_info.x_res * mode_info.y_res * mode_info.bytes_per_pixel;

    mode_info.red_begin = vbe_info.RedFieldPosition;
    mode_info.green_begin = vbe_info.GreenFieldPosition;
    mode_info.blue_begin = vbe_info.BlueFieldPosition;

    mode_info.red_end = mode_info.red_begin + vbe_info.RedMaskSize;
    mode_info.green_end = mode_info.green_begin + vbe_info.GreenMaskSize;
    mode_info.blue_end = mode_info.blue_begin + vbe_info.BlueMaskSize;

    // create the color masks
    mode_info.red_mask = (BIT(vbe_info.RedMaskSize) - 1) << mode_info.red_begin;
    mode_info.green_mask = (BIT(vbe_info.GreenMaskSize) - 1) << mode_info.green_begin;
    mode_info.blue_mask = (BIT(vbe_info.BlueMaskSize) - 1) << mode_info.blue_begin;
    
    return 0;
}

int (video_start)(uint16_t mode){
    int flag = video_get_mode_info(mode);
    if (flag) return flag;

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) mode_info.physical_base;	
    mr.mr_limit = mode_info.physical_base + mode_info.physical_size;

    // allow memory mapping
    flag = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
    if (flag){
        panic("sys_privctl (ADD_MEM) failed: %d\n", flag);
        return flag;
    }
    
    // map the video memory to the address space of the process
    front_buffer = vm_map_phys(SELF, (void*) mr.mr_base, mode_info.physical_size);
    back_buffer = malloc(mode_info.physical_size);

    if ((front_buffer == NULL) || (back_buffer == NULL)){
        perror("Couldn't map video memory!");
        return -1;
    }

    // change the video mode
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ah = VBE_FUNCTION_CALL;
    reg86.al = VBE_SET_MODE;
    reg86.bx = mode | BIT(14);

    flag = sys_int86(&reg86);
    if (flag)
        printf("Initializing graphics mode 0x%x failed!\n", mode);

    return flag;
}

int (video_stop)(){
    return vg_exit();
}

int video_switch(){
    memcpy(front_buffer, back_buffer, mode_info.physical_size);
    return (front_buffer == NULL);
}

int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x >= mode_info.x_res || y >= mode_info.y_res)
        return 1;

    uint32_t pixel_index = (y * mode_info.x_res + x) * mode_info.bytes_per_pixel;

    memcpy(&back_buffer[pixel_index], &color, mode_info.bytes_per_pixel);
    return (back_buffer == NULL);
}

int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (x >= mode_info.x_res || y >= mode_info.y_res)
        return 1;

    if (x + len >= mode_info.x_res)
        len = mode_info.x_res - x;

    uint32_t pixel_index = (y * mode_info.x_res + x) * mode_info.bytes_per_pixel;

    while (len--){
        memcpy(&back_buffer[pixel_index], &color, mode_info.bytes_per_pixel);
        pixel_index += mode_info.bytes_per_pixel;

        int flag = (back_buffer == NULL);
        if (flag) return flag;
    }
    
    return 0;
}

int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (x >= mode_info.x_res || y >= mode_info.y_res)
        return 1;

    if (y + len >= mode_info.y_res)
        len = mode_info.y_res - y;

    uint32_t pixel_index = (y * mode_info.x_res + x) * mode_info.bytes_per_pixel;

    while (len--){
        memcpy(&back_buffer[pixel_index], &color, mode_info.bytes_per_pixel);
        pixel_index += mode_info.x_res * mode_info.bytes_per_pixel;
        
        int flag = (back_buffer == NULL);
        if (flag) return flag;
    }
    
    return 0;
}

int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    if (y + height >= mode_info.y_res)
        height = mode_info.y_res - y;
    
    for (uint16_t i = 0; i < height; ++i){
        int flag = video_draw_row(x, y + i, width, color);
        if (flag) return flag;
    }

    return 0;
}

int (video_draw_sprite)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* colors){
    // check if any of the components overflow
    if (x + width >= mode_info.x_res)
        width = mode_info.x_res - x;

    if (y + height >= mode_info.y_res)
        height = mode_info.y_res - y;

    for (int y_ = y; y_ < y + height; y_+=2){
        for (int x_ = x; x_ < x + width; x_+=2){
            if (!*colors){
                colors++;
                continue;
            }

            video_draw_pixel(x_, y_ + 1, *colors);
            video_draw_pixel(x_ + 1, y_, *colors);
            video_draw_pixel(x_, y_, *colors);
            video_draw_pixel(x_ + 1, y_ + 1, *colors++);
        }
    }

    return 0;
}
