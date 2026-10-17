#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "graphics.h"

vbe_mode_info_t vmi_p;
static uint8_t* video_mem;
uint8_t* buffer, *background_buffer;
static uint16_t hres, vres, bytes_per_pixel;
size_t frames;
xpm_map_t charxpm[12]={(xpm_map_t)n0_xpm,(xpm_map_t)n1_xpm,(xpm_map_t)n2_xpm,(xpm_map_t)n3_xpm,(xpm_map_t)n4_xpm,(xpm_map_t)n5_xpm,(xpm_map_t)n6_xpm,(xpm_map_t)n7_xpm,(xpm_map_t)n8_xpm,(xpm_map_t)n9_xpm, (xpm_map_t)slash_xpm, (xpm_map_t)colon_xpm};

int (video_set_mode)(uint16_t mode) {
    
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = VBE_VID_INT;
    r.ax = VBE_SET_MODE;
    r.bx = mode | VBE_LINEAR_BUFFER;

    if (sys_int86(&r) != F_OK) {
        printf("sys_int86() failed\n");
        return 1;
    }

    return 0;
}

int (text_mode)() {
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = VBE_VID_INT;
    r.ah = BIOS_SET_MODE;
    r.al = TEXT_MODE;

    if (sys_int86(&r) != F_OK) {
        printf("sys_int86() failed\n");
        return 1;
    }

    return 0;
}

int (video_mode)() {
    if (video_set_mode(VBE_VIDEO_MODE_DC) != 0) {
        printf("video_set_mode() failed\n");
        return 1;
    }

    return map_memory(VBE_VIDEO_MODE_DC);
}

int (map_memory)(uint16_t mode) {
    memset(&vmi_p, 0, sizeof(vmi_p));
    vbe_get_mode_info(mode, &vmi_p);

    hres = vmi_p.XResolution;
    vres = vmi_p.YResolution;
    bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
    size_t frame_size = hres * vres * bytes_per_pixel;

    int r;
    struct minix_mem_range mr;
    mr.mr_base = vmi_p.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_size;


    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    }

    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, frame_size);
    buffer = (uint8_t*)malloc(frame_size);
    background_buffer= (uint8_t*)malloc(frame_size);
    if (buffer == NULL) {
        printf("Memory allocation failed. Exiting...");
        return 1;
    }


    frames=frame_size;

    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
    }

    return 0;
}

void (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
     if (color == xpm_transparency_color(XPM_8_8_8_8)) return;
    if (x < 0 || y < 0 || x >= hres || y >= vres) return;
    size_t i = (hres * y + x) * bytes_per_pixel;
     memcpy(&buffer[i], &color, bytes_per_pixel);
}

uint16_t(get_hres)() {
    return hres;
}

uint16_t(get_vres)() {
    return vres;
}
void (clear_buffer)(){
    memset(buffer, 0, frames); 

}
void (draw_buffer)(){
    memcpy(video_mem, buffer, frames);

}
int(draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;
    uint32_t *color=(uint32_t*)xpm_load(xpm, XPM_8_8_8_8, &img);
    if (color==NULL) {
        printf("Error drawing xpm!\n");
        return 1;
    }
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
         
            video_draw_pixel(x+j,y+i,*color);
            color++;
        }
    }
    return 0;
}
void (draw_background)(){
    memcpy(background_buffer, buffer, frames);

}
int (replace_with_background)(uint16_t x, uint16_t y){
     if(x>hres || y>vres){ 
        printf("Position outside the screen\n");
        return 1;
    }
    size_t i = (hres * y + x) * bytes_per_pixel;
   
    memcpy(&buffer[i], &background_buffer[i], bytes_per_pixel);
    return 0;

}
void (clear_background)(){
    memset(background_buffer, 0, frames); 
    memset(buffer, 0, frames); 
    memset(video_mem, 0, frames); 
    draw_buffer();
}

uint32_t *(load_img)(xpm_map_t pic){
    xpm_image_t img;
    uint32_t* colormap = (uint32_t*)xpm_load(pic, XPM_8_8_8_8, &img);
    return colormap;

}

int(draw_number)(uint16_t x, uint16_t y, uint32_t number){

    if (number>9 ){ 
        printf("Invalid number in drawn_number\n");
        return 1;
    }

    for(int i=0; i<54;i++){
        for(int j=0; j<54;j++){
            replace_with_background(x+j,y+i);
        }
    }

    if(draw_xpm(charxpm[number],x,y)){ 
        printf("Could not load image corresponding to the number %d\n", number);
        return 1;
    }

    return 0;
}

int (draw_special_char)(uint16_t x, uint16_t y, uint32_t special_char){

    if (special_char>11 || special_char <10) return 1;

    for(int i=0; i<54;i++){
        for(int j=0; j<54;j++){
            replace_with_background(x+j,y+i);
        }
    }

    if(draw_xpm(charxpm[special_char],x,y)) return 1;

    return 0;
}
