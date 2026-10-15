#include "graphics.h"

vbe_mode_info_t info;
uint8_t *video_buffer;
uint8_t *write_buffer;

int (set_graphic_mode)(uint16_t mode) {
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02;
    r.bx = BIT(14) | mode;
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}


int (set_buffer)(uint16_t mode) {

    memset(&info, 0, sizeof(info));
    if(vbe_get_mode_info(mode, &info) != 0)
        return 1;


    int r;
    struct minix_mem_range mr;
    unsigned int vram_base = info.PhysBasePtr;
    unsigned int vram_size = info.XResolution * info.YResolution * (info.BitsPerPixel + 7) / 8;

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }
        

    video_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_buffer == MAP_FAILED){
        panic("couldn’t map video memory");
        return 1;
    }
    
    return 0;
}

int(allocate_write_buffer)(){
    uint32_t buffer_size = info.XResolution * info.YResolution * (info.BitsPerPixel + 7) / 8;
    write_buffer = (uint8_t*) malloc(buffer_size);
    if(write_buffer == NULL){
        return 1;
    }
    memset(write_buffer, 0, buffer_size);
    return 0;
}

void (switch_buffers)(){
    uint32_t buffer_size = info.XResolution * info.YResolution * (info.BitsPerPixel + 7) / 8;
    memcpy(video_buffer, write_buffer, buffer_size);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if(x > info.XResolution || y > info.YResolution ||y<0 ||x <0) 
        return 0;
    if(color==0xAFFFFF || color==0xB1FFFF || color==0XB9FBFB || color==0x9ADDDD  || color==0x080707 || color == 0x282727 || color==0xA6F1F1 || color==0xA3ECEC)
        return 0;
  
    unsigned bpp = (info.BitsPerPixel + 7) / 8;

    unsigned int idx = (info.XResolution * y + x) * bpp;
    uint32_t new_color;
    fix_color(color, &new_color);
    memcpy(&write_buffer[idx], &new_color, bpp);

    return 0;

}



void (fix_color)(uint32_t color, uint32_t *normalized_color){
    uint8_t red, green, blue;
    uint32_t new_color = 0;

    
    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = color & 0xFF;

    switch (info.BitsPerPixel) {
        case 15:
            new_color = ((red >> 3) << 10) | ((green >> 3) << 5) | (blue >> 3);
            break;
        case 16:
            new_color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            break;
        case 24:
            new_color = (red << 16) | (green << 8) | blue;
            break;
        case 32:
            new_color = (red << 16) | (green << 8) | blue;
            break;
        default:
            new_color = 0;
            break;
    }

    *normalized_color = new_color;
}



int (draw_xpm)(xpm_map_t xmap, uint16_t x, uint16_t y){

  xpm_image_t img;
  uint8_t *map;

  uint16_t 	width, height;
  map = xpm_load(xmap, XPM_8_8_8_8, &img);

  width = img.width;
  height = img.height;

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(vg_draw_pixel(x + j, y + i, map[i * width + j]) != 0)
        return 1;
    }
  }

  return 0;
}

